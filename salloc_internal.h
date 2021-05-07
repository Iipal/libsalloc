#ifndef __SALLOC_INTERNAL_H__
#define __SALLOC_INTERNAL_H__

#include <stddef.h> /* for size_t only */
#include <stdint.h>

#pragma clang diagnostic ignored "-Wgcc-compat"

#include "salloc_attrs.h"
#include "salloc_base_types.h"

/**
 * Some helper macroses defines
 */

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
#define __chunk_align_size(_size) \
  ((size_t)(((_size) % __chunk_align_default) \
                ? ((_size) + ((~(_size)&__chunk_align_bits) + 1UL)) \
                : (_size)))

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
  do { \
    __chunk_clear_inuse(x); \
    __chunk_get_value(x) |= __chunk_mask_inuse; \
    __chunk_clear_inuse((x) + (_size) + __chunk_free_list_size); \
    __chunk_get_value((x) + (_size) + __chunk_free_list_size) |= __chunk_mask_inuse; \
  } while (0)
#define __chunk_set_size(x, _size) \
  do { \
    __chunk_clear_size(x); \
    __chunk_get_value(x) |= _size; \
    __chunk_clear_size((x) + (_size) + __chunk_free_list_size); \
    __chunk_get_value((x) + (_size) + __chunk_free_list_size) |= _size; \
  } while (0)

#define __chunk_unset_free(x, _size) \
  do { \
    __chunk_clear_inuse(x); \
    __chunk_clear_inuse((x) + (_size) + __chunk_free_list_size); \
  } while (0)
#define __chunk_unset_size(x, _size) \
  do { \
    __chunk_clear_size(x); \
    __chunk_clear_size((x) + (_size) + __chunk_free_list_size); \
  } while (0)

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

/**
 * Custom types definition
 */

typedef struct s_salloc_mem_addr_t {
  /* cursor to the end of available space \ start of each new chunk */
  void * const restrict cursor;
  void * const restrict start;
  void * const restrict end;
  const size_t          capacity;
} libsalloc_attr_alignof(salloc_vec4_t) salloc_mem_addr_t;

typedef union {
  salloc_mem_addr_t addr;
  salloc_vec4_t     vec;
} salloc_mem_t libsalloc_attr_transparent;

typedef struct s_salloc {
  unsigned char * restrict buff;
  size_t                   buff_length;
  uintptr_t                previous_offset;
  uintptr_t                current_offset;
} libsalloc_attr_alignof(salloc_vec4_t) salloc_t;

typedef union {
  salloc_t      data;
  salloc_vec4_t vector;
} salloc_allocator_t libsalloc_attr_transparent;

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
  const size_t        chunk_size = size + __chunk_free_list_bd_size;
  const salloc_vec4_t memv       = mem->vec;
  typeof(memv[0]) out            = nullptr;

  if (__chunk_valid_end(memv, chunk_size)) {
    out = memv[__salloc_vec_mem_cursor];
    memv[__salloc_vec_mem_cursor] += chunk_size;
    __chunk_set_size(out, size);
    __chunk_set_inuse(out, size);
  }

  return out;
}

libsalloc_attr_veccall_overload static inline void *
    salloc(register salloc_mem_t * const restrict mem, register const size_t size) {
#ifdef SALLOC_UNSAFE
  if (!mem || !mem->addr.capacity) {
    return nullptr;
  }
#endif

  register const size_t aligned_size = __salloc_align_size(size);
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
  const uintptr_t relative_offset = salloc_align_forward(current_pointer, align) -
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
    srealloc(register salloc_vec4_t * const restrict allocator,
             register void * const restrict          old_memory,
             register const size_t                   old_size,
             register const size_t                   new_size) {
  return srealloc(allocator, old_memory, old_size, new_size, __chunk_align_default);
}

libsalloc_attr_flatten_veccall_overload static inline void *
    srealloc(register salloc_vec4_t * const restrict allocator,
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
    sfree(register salloc_vec4_t * const restrict allocator) {
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
    salloc_align_forward(register const uintptr_t pointer, register const size_t align) {
  register const uintptr_t modulo = pointer % (typeof(pointer))align;

  if (modulo != 0) {
    return (pointer + (typeof(pointer))align - modulo);
  } else {
    return (pointer);
  }
}

#endif /* __SALLOC_INTERNAL_H__ */
