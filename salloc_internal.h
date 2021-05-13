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
  const scptr_t  buff_end = (__s2c_sptr(buff) + (uintptr_t)buff_length);
  const salloc_t out      = (salloc_t){
      buff,
      buff_end,
      buff,
      buff_length,
  };

  return out;
}

libsalloc_attr_flatten_veccall static inline void
    salloc_delete(register salloc_t * const restrict __s) {
  __s->cursor = __s->start;
}

libsalloc_attr_veccall static inline void
    salloc_trace(register salloc_t * const restrict __s) {
  sptr_t iptr   = __s->start;
  sptr_t cursor = __s->cursor;

  printf("%-6zu [%p ... %p] at %p\n", __s->capacity, __s->start, __s->end, __s->cursor);

  size_t blocks_count = 0;
  while (iptr < cursor) {
    salloc_chunk_t * c     = __s2c_chunk(iptr);
    salloc_chunk_t * c_end = __s2c_chunk(iptr + c->size + __sc_fl_size);

    printf("%d %-4zu [%p ... %p] %d %-4zu\n",
           c->busy,
           c->size,
           c,
           c_end,
           c_end->busy,
           c_end->size);

    iptr = __s2c_sptr(c_end) + __sc_fl_size;
    ++blocks_count;
  }

  const uintptr_t mapped_memory = __s2c_uiptr(__s->cursor - __s->start);
  printf("total: %zu (%zu)\n",
         mapped_memory - (blocks_count * __sc_flbd_size),
         mapped_memory);
}

libsalloc_attr_flatten_veccall static inline void *
    __salloc_move_cursor(register salloc_t * const restrict __s,
                         register const uintptr_t           size) {
  register const uintptr_t chunk_size = __sc_wflbd_size(size);

#ifndef SALLOC_UNSAFE_MAPPING
  if ((__s->cursor + chunk_size) <= __s->end) {
#endif
    register sptr_t out = __s->cursor;

    register salloc_chunk_t *     chunk      = __s2c_chunk(out);
    register salloc_chunk_t *     chunk_end  = __s2c_chunk(out + size + __sc_fl_size);
    register const salloc_chunk_t chunk_data = {size, 1, 0};

    *chunk     = chunk_data;
    *chunk_end = chunk_data;

    register sptr_t new_cursor = out + chunk_size;
    __s->cursor                = new_cursor;

    return __sc_chunk_get_ptr(out);
#ifndef SALLOC_UNSAFE_MAPPING
  } else {
    return NULL;
  }
#endif
}
libsalloc_attr_veccall_overload static inline void *
    salloc(register salloc_t * const restrict __s, register const size_t __size) {
  register const uintptr_t aligned_size = __sc_align_size(__size);
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

libsalloc_attr_flatten_veccall static inline void
    __sfree_frag(register sptr_t __iptr, register sptr_t __baseptr) {
  const uintptr_t __base_size = __sc_get_size(__baseptr);
  const uintptr_t __iptr_size = __sc_get_size(__iptr);
  const uintptr_t __frag_size = __base_size + __sc_flbd_size + __iptr_size;

  salloc_chunk_t *     __iptr_start    = __s2c_chunk(__iptr);
  salloc_chunk_t *     __iptr_end      = __s2c_chunk(__iptr + __frag_size + __sc_fl_size);
  const salloc_chunk_t __iptr_new_data = {__frag_size, __sc_not_busy};

  *__iptr_start = __iptr_new_data;
  *__iptr_end   = __iptr_new_data;
}
libsalloc_attr_flatten_veccall static inline bool
    __sfree_shrink_cursor(register salloc_t * const restrict __s, register sptr_t __ptr) {
  bool is_moved = false;

  if (__s->cursor <= (__ptr + __sc_get_size(__ptr) + __sc_flbd_size)) {
    __s->cursor = __ptr;
    is_moved    = true;
  }

  return is_moved;
}
libsalloc_attr_veccall_overload static inline void
    sfree(register salloc_t * const restrict __s, register void * restrict __ptr) {
  sfree(__ptr);

  const scptr_t __s_start  = __s->start;
  const scptr_t __s_cursor = __s->cursor;

  salloc_chunk_t * __ptr_chunk = __sc_ptr_get_chunk(__ptr);
  sptr_t           __iptr      = __s2c_sptr(__ptr_chunk);

  /**
   * Left-side memory fragmentation.
   * Before:
   * [inuse] [while_free_chunks] [free_chunk1] [__ptr chunk] [right_side_mem]
   * After:
   * [inuse] [   new_big_damn_ass_fragmented_memory_chunk  ] [right_side_mem]
   */
  if (__iptr > __s_start) {
    sptr_t __baseptr = __iptr;
    __iptr = __baseptr - __sc_flbd_size - __sc_get_size(__baseptr - __sc_fl_size);

    while (__iptr >= __s_start && __sc_is_free(__iptr)) {
      __sfree_frag(__iptr, __baseptr);

      sptr_t __prev_ptr = __iptr - __sc_fl_size;
      if (__s_start <= __prev_ptr) {
        salloc_chunk_t * __prev_chunk = __s2c_chunk(__prev_ptr);
        __prev_ptr                    = __prev_ptr - __prev_chunk->size - __sc_fl_size;
      }

      __baseptr = __iptr;
      __iptr    = __prev_ptr;
    }

    __iptr = __baseptr;
  }

  if (__sfree_shrink_cursor(__s, __iptr)) {
    /**
     * If cursors was moved after left-side fragmentation than means right-side has no
     * available memory at all
     */
    return;
  }

  /**
   * Right-side memory fragmentation.
   * Before:
   * [left_side_mem] [ left-side_fragmented_chunk ] [free_chunk1] [inuse]
   * After:
   * [left_side_mem] [ new_big_damn_ass_fragmented_memory_chunk ] [inuse]
   */
  {
    sptr_t __baseptr = __iptr;
    __iptr           = __baseptr + __sc_get_size(__baseptr) + __sc_flbd_size;

    while (__iptr < __s_cursor && __sc_is_free(__iptr)) {
      __sfree_frag(__baseptr, __iptr);
      __iptr += __sc_get_size(__iptr) + __sc_flbd_size;
    }

    __sfree_shrink_cursor(__s, __baseptr);
  }
}

libsalloc_attr_flatten_veccall_overload static inline void
    sfree(register void * restrict __ptr) {
  salloc_chunk_t *         chunk       = __sc_ptr_get_chunk(__ptr);
  register const uintptr_t chunk_size  = chunk->size;
  salloc_chunk_t *         chunk_end   = __s2c_chunk(__ptr + chunk_size);
  const salloc_chunk_t     chunk_freed = {chunk_size, __sc_not_busy};

  *chunk     = chunk_freed;
  *chunk_end = chunk_freed;
}

#endif /* __SALLOC_INTERNAL_H__ */
