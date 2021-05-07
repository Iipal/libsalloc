#ifndef __SALLOC_INTERNAL_H__
#define __SALLOC_INTERNAL_H__

#include "salloc_macros.h"
#include "salloc_types.h"

/**
 * Function Definitions:
 */

libsalloc_attr_veccall_const static inline salloc_mem_t
    salloc_init(register const void * const restrict buff,
                register const size_t                buff_length) {
  return salloc_init_fast(buff, buff_length);
}

libsalloc_attr_veccall_const static inline void *
    __move_cursor(register salloc_mem_t * const restrict mem,
                  register const size_t                  size) {
  const size_t  chunk_size = size + __chunk_free_list_bd_size;
  salloc_vec4_t memv       = mem->vec;
  typeof(memv[0]) out      = 0;

  if (__chunk_valid_end(memv, chunk_size)) {
    out = memv[__salloc_vec_mem_cursor];
    memv[__salloc_vec_mem_cursor] += chunk_size;
    __chunk_set_size(out, size);
    __chunk_set_inuse(out, size);
  }

  return (void *)out;
}

libsalloc_attr_veccall_overload static inline void *
    salloc(register salloc_mem_t * const restrict mem, register const size_t size) {
#ifdef SALLOC_UNSAFE
  if (!mem || !mem->addr.capacity) {
    return nullptr;
  }
#endif

  register const size_t aligned_size = __chunk_align_size(size);
  void * restrict       out          = NULL;

  /**
   *  If cursor is not at the start of buffer - trying to find the best available block
  if (mem->addr.start != mem->addr.cursor) {

  }

  If there is no "best" available - creating new by moving the cursor ptr
  if (!out) {
  */
  out = __move_cursor(mem, aligned_size);
  // }

  if (out) {
    return __chunk_get_ptr(out);
  }
  return out;
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
  return _salloc(allocator, size, __chunk_align_default);
}

libsalloc_attr_veccall_overload static inline void *
    _salloc(register salloc_vec4_t * const restrict allocator,
            register const size_t                   size,
            register const size_t                   align) {
  const salloc_vec4_t in_allocator = *allocator;
  const typeof(in_allocator[0]) current_pointer =
      in_allocator[__salloc_vec_alloc_buff] +
      in_allocator[__salloc_vec_alloc_curr_offset];
  const uintptr_t relative_offset = _salloc_align_forward(current_pointer, align) -
                                    in_allocator[__salloc_vec_alloc_buff];
  const uintptr_t current_offset = (typeof(size))relative_offset + size;

  if (current_offset <=
      (typeof(current_offset))in_allocator[__salloc_vec_alloc_buff_len]) {
    const typeof(in_allocator) out_allocator = {
        in_allocator[__salloc_vec_alloc_buff],
        in_allocator[__salloc_vec_alloc_buff_len],
        (typeof(in_allocator[__salloc_vec_alloc_prev_offset]))relative_offset,
        (typeof(in_allocator[__salloc_vec_alloc_curr_offset]))current_offset};

    *allocator = out_allocator;

    return __extension__((void *)in_allocator[__salloc_vec_alloc_buff] + relative_offset);
  } else {
    return nullptr;
  }
}

libsalloc_attr_flatten_veccall_overload static inline void *
    _srealloc(register salloc_vec4_t * const restrict allocator,
              register void * const restrict          old_memory,
              register const size_t                   old_size,
              register const size_t                   new_size) {
  return _srealloc(allocator, old_memory, old_size, new_size, __chunk_align_default);
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
  } else if ((in_allocator[__salloc_vec_alloc_buff] <=
              (typeof(in_allocator[0]))old_memory) &&
             ((typeof(in_allocator[0]))old_memory <
              (in_allocator[__salloc_vec_alloc_buff] +
               in_allocator[__salloc_vec_alloc_buff_len]))) {
    if ((in_allocator[__salloc_vec_alloc_buff] +
         in_allocator[__salloc_vec_alloc_prev_offset]) ==
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
      in_allocator[__salloc_vec_alloc_buff],
      in_allocator[__salloc_vec_alloc_buff_len],
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
