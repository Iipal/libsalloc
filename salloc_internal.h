#ifndef __SALLOC_INTERNAL_H__
#define __SALLOC_INTERNAL_H__

#if !defined(__SALLOC_H__)
#  error "Do not include `salloc_internal.h` directly."
#endif

#include "salloc_macros.h"
#include "salloc_types.h"

/**
 * Function Definitions:
 */

libsalloc_attr_veccall_const static inline salloc_t
    salloc_new(register const void * const restrict buff,
               register const size_t                buff_length) {
  const uint8_t * const restrict buff_end = (__s2c_ui8ptr(buff) + (uintptr_t)buff_length);
  const salloc_t out = __s2c_slc_map_buff(out.vec, buff, buff_end, buff, buff_length);

  return out;
}

libsalloc_attr_flatten_veccall static inline void
    salloc_delete(register salloc_t * const restrict __s) {
  __s->addr.cursor = __s->addr.start;
}

libsalloc_attr_veccall static inline void
    salloc_trace(register salloc_t * const restrict __s) {
  uint8_t * restrict iptr   = __s->addr.start;
  uint8_t * restrict cursor = __s->addr.cursor;

  printf("start: %p; end: %p; cursor: %p; capacity: %zu;\n",
         __s->addr.start,
         __s->addr.end,
         __s->addr.cursor,
         __s->addr.capacity);

  while (iptr && iptr < cursor) {
    salloc_chunk_t * c = __s2c_chunk(iptr);

    printf("start: %zu-%d-%p\n", c->size, c->inuse, c);
    printf(" data: (size_t)`%zu` | (char*)`%s`\n",
           __s2c_size(iptr + __sc_fl_size),
           (iptr + __sc_fl_size));

    iptr += c->size + __sc_fl_size;
    c = __s2c_chunk(iptr);

    printf("  end: %zu-%d-%p\n\n", c->size, c->inuse, c);
    iptr += __sc_fl_size;
  }
}

libsalloc_attr_flatten_veccall static inline void *
    __salloc_move_cursor(register salloc_t * const restrict __s,
                         register const size_t              size) {
  register const uintptr_t chunk_size = __sc_wflbd_size(size);
  register salloc_vec4_t   mv         = __s2c_slcptr_vec(__s);

#ifndef SALLOC_UNSAFE_MAPPING
  if (__sc_valid_end(mv, chunk_size)) {
#endif
    register __s2c_tx(mv) out = __sva_get_cursor(mv);
    // __sc_set(out, size);

    register const salloc_chunk_t chunk_data = {size, 1, 0};
    register salloc_chunk_t *     chunk      = (void *)out;
    register salloc_chunk_t *     chunk_end  = __s2c_chunk(__sc_fl_shift(out, size));

    *chunk     = chunk_data;
    *chunk_end = chunk_data;

    register const __s2c_tx(mv) new_cursor = __sc_shift(out, chunk_size);

    const salloc_t s = __s2c_slc_map_buff(s.vec,
                                          __sva_get_start(mv),
                                          __sva_get_end(mv),
                                          new_cursor,
                                          __sva_get_capacity(mv));

    *__s = s;
    return __sc_chunk_get_ptr(out);
#ifndef SALLOC_UNSAFE_MAPPING
  } else {
    return NULL;
  }
#endif
}

libsalloc_attr_veccall_overload static inline void *
    salloc(register salloc_t * const restrict __s, register const size_t __size) {
#ifndef SALLOC_UNSAFE
  if (!__size || !__s || !__s->addr.capacity) {
    return nullptr;
  }
#endif

  register const size_t    aligned_size = __sc_align_size(__size);
  register void * restrict out          = NULL;

  /**
   *  If cursor is not at the start of buffer - trying to find the best available block
  if (s->s.start != s->s.cursor) {

  }

  If there is no "best" available - creating new by moving the cursor ptr
  if (!out) {
  */
  out = __salloc_move_cursor(__s, aligned_size);
  // }

  return out;
}

libsalloc_attr_flatten_veccall_overload static inline void *
    salloc(register salloc_t * const restrict __s,
           register const size_t              __size,
           register const size_t              __nmemb) {
  return salloc(__s, __nmemb * __size);
}

libsalloc_attr_veccall_overload static inline void
    sfree(register salloc_t * const restrict __s, register void * restrict __ptr) {
  (void)__s;
  sfree(__ptr);
  // TODO: Implement correct freeing of __ptr and optimizing space in __s.
}

libsalloc_attr_flatten_veccall_overload static inline void
    sfree(register void * restrict __ptr) {
  salloc_chunk_t * restrict chunk                = __sc_ptr_get_chunk(__ptr);
  register const __s2c_t(chunk->size) chunk_size = chunk->size;
  salloc_chunk_t * restrict chunk_end   = __s2c_chunk(__sc_shift(__ptr, chunk_size));
  const salloc_chunk_t      chunk_freed = {chunk_size, __sc_vn_inuse};

  *chunk     = chunk_freed;
  *chunk_end = chunk_freed;
}

/**
 * Prev Definitions:
 */

libsalloc_attr_veccall_const static inline salloc_vec4_t
    _salloc_init(register const void * const restrict buff,
                 register const size_t                buff_length) {
  const salloc_allocator_t allocator libsalloc_attr_munused;

  return (typeof(allocator.vector)){(typeof(allocator.vector[0]))buff,
                                    (typeof(allocator.vector[0]))buff_length};
}

libsalloc_attr_veccall_overload static inline void *
    _salloc(register salloc_vec4_t * restrict allocator, register const size_t size) {
  return _salloc(allocator, size, __sc_align_default);
}

libsalloc_attr_veccall_overload static inline void *
    _salloc(register salloc_vec4_t * const restrict allocator,
            register const size_t                   size,
            register const size_t                   align) {
  const salloc_vec4_t in_allocator = *allocator;
  const typeof(in_allocator[0]) current_pointer =
      in_allocator[__sva_alloc_buff] + in_allocator[__sva_alloc_curr_offset];
  const uintptr_t relative_offset =
      _salloc_align_forward(current_pointer, align) - in_allocator[__sva_alloc_buff];
  const uintptr_t current_offset = (typeof(size))relative_offset + size;

  if (current_offset <= (typeof(current_offset))in_allocator[__sva_alloc_buff_len]) {
    const typeof(in_allocator)
        out_allocator = {in_allocator[__sva_alloc_buff],
                         in_allocator[__sva_alloc_buff_len],
                         (typeof(in_allocator[__sva_alloc_prev_offset]))relative_offset,
                         (typeof(in_allocator[__sva_alloc_curr_offset]))current_offset};

    *allocator = out_allocator;

    return __extension__((void *)in_allocator[__sva_alloc_buff] + relative_offset);
  } else {
    return nullptr;
  }
}

libsalloc_attr_flatten_veccall_overload static inline void *
    _srealloc(register salloc_vec4_t * const restrict allocator,
              register void * const restrict          old_memory,
              register const size_t                   old_size,
              register const size_t                   new_size) {
  return _srealloc(allocator, old_memory, old_size, new_size, __sc_align_default);
}

libsalloc_attr_flatten_veccall_overload static inline void *
    _srealloc(register salloc_vec4_t * const restrict allocator,
              register void * const restrict          old_memory,
              register const size_t                   old_size,
              register const size_t                   new_size,
              register const size_t                   align) {
  const salloc_vec4_t in_allocator = *allocator;

  if ((old_memory == nullptr) || (old_size == 0)) {
    return __extension__ _salloc(allocator, new_size, align);
  } else if ((in_allocator[__sva_alloc_buff] <= (typeof(in_allocator[0]))old_memory) &&
             ((typeof(in_allocator[0]))old_memory <
              (in_allocator[__sva_alloc_buff] + in_allocator[__sva_alloc_buff_len]))) {
    if ((in_allocator[__sva_alloc_buff] + in_allocator[__sva_alloc_prev_offset]) ==
        (typeof(in_allocator[0]))old_memory) {
      return old_memory;
    } else {
      return _salloc(allocator, new_size, align);
    }
  } else {
    return nullptr;
  }
}

libsalloc_attr_veccall_overload static inline void
    _sfree(register salloc_vec4_t * const restrict allocator) {
  const salloc_vec4_t in_allocator       = *allocator;
  const typeof(*allocator) out_allocator = {
      in_allocator[__sva_alloc_buff],
      in_allocator[__sva_alloc_buff_len],
      0,
      0 /* reset all offsets */
  };

  *allocator = out_allocator;
}

libsalloc_attr_veccall_const static inline uintptr_t
    _salloc_align_forward(register const uintptr_t pointer, register const size_t align) {
  register const uintptr_t modulo = pointer % (typeof(pointer))align;

  if (modulo != 0) {
    return (pointer + (typeof(pointer))align - modulo);
  } else {
    return (pointer);
  }
}

#endif /* __SALLOC_INTERNAL_H__ */
