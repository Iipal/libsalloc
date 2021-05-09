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
    salloc_init(register const void * const restrict buff,
                register const size_t                buff_length) {
  const uint8_t * const restrict buff_end = (__s2c_ui8ptr(buff) + (uintptr_t)buff_length);
  const salloc_t out = __s2c_slc_map_buff(out.vec, buff, buff_end, buff, buff_length);

  return out;
}

libsalloc_attr_veccall_const static inline void *
    __salloc_move_cursor(register salloc_t * const restrict mem,
                         register const size_t              size) {
  register const size_t  chunk_size = __sc_wflbd_size(size);
  register salloc_vec4_t mv         = __s2c_slcptr_vec(mem);
  register __s2c_tx(mv) out         = 0;

#ifndef SALLOC_UNSAFE_MAPPING
  if (__sc_valid_end(mv, chunk_size)) {
#endif
    out                      = __sva_get_cursor(mv);
    salloc_chunk_map_t * map = (void *)out;
    salloc_chunk_map_t * map_end =
        (void *)((uint8_t *)out + (uintptr_t)size + (uintptr_t)sizeof(uintptr_t));

    *map     = (salloc_chunk_map_t){size, __sc_mask_inuse};
    *map_end = (salloc_chunk_map_t){size, __sc_mask_inuse};

    {
      // (*((uintptr_t *)((uint8_t *)out))) &= ~((uintptr_t)(0xFFFFFFFFFFFFFFF0UL));
      // (*((uintptr_t *)((uint8_t *)out))) |= size;
      // // (*((uintptr_t *)(((uint8_t *)out) + (size) + (sizeof(uintptr_t))))) &=
      // // ~((uintptr_t)(0xFFFFFFFFFFFFFFF8UL));
      // // s_data__ __data2__
      // (*((uintptr_t *)((out) + (size) + (sizeof(uintptr_t))))) |= size;
    }

    register const __s2c_tx(mv) new_cursor = __sva_move_cursor(mv, chunk_size);

    mem->addr.cursor = __s2c_ui8ptr(new_cursor);
    return (void *)out;
#ifndef SALLOC_UNSAFE_MAPPING
  } else {
    return NULL;
  }
#endif
}

libsalloc_attr_veccall_overload static inline void *
    salloc(register salloc_t * const restrict mem, register const size_t size) {
#ifndef SALLOC_UNSAFE
  if (!mem || !mem->mem.capacity) {
    return nullptr;
  }
#endif

  register const size_t    aligned_size = __sc_align_size(size);
  register void * restrict out          = NULL;

  /**
   *  If cursor is not at the start of buffer - trying to find the best available block
  if (mem->mem.start != mem->mem.cursor) {

  }

  If there is no "best" available - creating new by moving the cursor ptr
  if (!out) {
  */
  out = __salloc_move_cursor(mem, aligned_size);
  // }

  if (out) {
    return __sc_get_ptr(out);
  }
  return out;
}

#include <string.h>

libsalloc_attr_veccall_const static inline void
    sfree(register salloc_t * const restrict mem) {
  const salloc_t _m       = *mem;
  const __s2c_t(*mem) out = __s2c_slc_map_buff(_m.vec,
                                               __sva_get_start(_m.vec),
                                               __sva_get_end(_m.vec),
                                               __sva_get_start(_m.vec),
                                               __sva_get_capacity(_m.vec));

  *mem = out;
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
