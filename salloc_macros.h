#ifndef __SALLOC_MACROS_H__
#define __SALLOC_MACROS_H__

#ifndef alignof
#  include "salloc_types.h"
#endif

/**
 * named indexes for salloc_allocator_t.vector
 */
#define __salloc_vec_alloc_buff        (0)
#define __salloc_vec_alloc_buff_len    (1)
#define __salloc_vec_alloc_prev_offset (2)
#define __salloc_vec_alloc_curr_offset (3)

/**
 * named indexes for salloc_mem_t.vec
 */
#define __salloc_vec_mem_cursor   (0)
#define __salloc_vec_mem_start    (1)
#define __salloc_vec_mem_end      (2)
#define __salloc_vec_mem_capacity (3)

#define __chunk_align_default (alignof(void *) * 2)
#define __chunk_align_bits    ((uintptr_t)(__chunk_align_default - 1ul))
#define __chunk_align(x) \
  (((x) % __chunk_align_default) ? ((x) + ((~(x)&__chunk_align_bits) + 1UL)) : (x))
#define __chunk_align_size(x)    ((size_t)__chunk_align(x))
#define __chunk_align_forward(x) ((uintptr_t)__chunk_align(x))

#define __chunk_free_list_size    (sizeof(uintptr_t))
#define __chunk_free_list_bd_size (sizeof(uintptr_t) * 2) // bd stands for bi-directional

#define __ptr_get_chunk(x) ((x)-__chunk_free_list_size)
#define __chunk_get_ptr(x) ((x) + __chunk_free_list_size)

#define __chunk_mask_inuse ((uintptr_t)(0x0000000000000001UL))
#define __chunk_mask_size  ((uintptr_t)(0xFFFFFFFFFFFFFFF8UL))

#define __chunk_get_value(x) (*((uintptr_t *)(x)))

#define __chunk_clear_inuse(x) (__chunk_get_value(x) &= ~__chunk_mask_inuse)
#define __chunk_clear_size(x)  (__chunk_get_value(x) &= ~__chunk_mask_size)
#define __chunk_clear_value(x) (__chunk_get_value(x) ^= __chunk_get_value(x))

#define __chunk_is_inuse(x) !(__chunk_get_value(x) & __chunk_mask_size)
#define __chunk_get_size(x) (__chunk_get_value(x) & __chunk_mask_size)

#define __chunk_set_inuse(x, _size) \
  { \
    __chunk_clear_inuse(x); \
    __chunk_get_value(x) |= __chunk_mask_inuse; \
    __chunk_clear_inuse((x) + (_size) + __chunk_free_list_size); \
    __chunk_get_value((x) + (_size) + __chunk_free_list_size) |= __chunk_mask_inuse; \
  }
#define __chunk_set_size(x, _size) \
  { \
    __chunk_clear_size(x); \
    __chunk_get_value(x) |= _size; \
    __chunk_clear_size((x) + (_size) + __chunk_free_list_size); \
    __chunk_get_value((x) + (_size) + __chunk_free_list_size) |= _size; \
  }

#define __chunk_unset_free(x, _size) \
  { \
    __chunk_clear_inuse(x); \
    __chunk_clear_inuse((x) + (_size) + __chunk_free_list_size); \
  }
#define __chunk_unset_size(x, _size) \
  { \
    __chunk_clear_size(x); \
    __chunk_clear_size((x) + (_size) + __chunk_free_list_size); \
  }

#define __chunk_valid_start(memv, _offset) \
  ((memv)[__salloc_vec_mem_cursor] - (_offset) >= (memv)[__salloc_vec_mem_start])
#define __chunk_valid_end(memv, _offset) \
  ((memv)[__salloc_vec_mem_cursor] + (_offset) <= (memv)[__salloc_vec_mem_end])

#define salloc_init_fast(buff, buff_length) \
  (salloc_mem_t) { \
    .vec = { \
      (typeof(((salloc_mem_t *)nullptr)->vec[0]))(buff), \
      (typeof(((salloc_mem_t *)nullptr)->vec[0]))(buff), \
      (typeof(((salloc_mem_t *)nullptr)->vec[0]))((buff) + (buff_length)), \
      (typeof(((salloc_mem_t *)nullptr)->vec[0]))(buff_length) \
    } \
  }

#endif /* __SALLOC_MACROS_H__ */
