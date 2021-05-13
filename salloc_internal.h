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

__sattr_veccall_const static inline salloc_t
    salloc_new(register const void * const restrict buff,
               register const __s_size_t            buff_length) {
  __s_ptr_t buff_end = (__s2c_ptr(buff) + (__s_uintptr_t)buff_length);
  salloc_t  out      = (salloc_t){__s2c_ptr(buff), buff_end, __s2c_ptr(buff)};

  return out;
}

__sattr_flatten_veccall static inline void
    salloc_delete(register salloc_t * const restrict __s) {
  __s->cursor = __s->start;
}

__sattr_veccall static inline void salloc_trace(register salloc_t * const restrict __s) {
#ifndef SALLOC_DEBUG
  (void)__s;
#else
  __s_ptr_t        iptr     = __s->start;
  __s_ptr_t        cursor   = __s->cursor;
  const __s_size_t capacity = __s->end - __s->start;

  printf("%-6zu [%p ... %p] at %p\n", capacity, __s->start, __s->end, __s->cursor);

  __s_size_t blocks_count = 0;
  while (iptr < cursor) {
    __s_chunk_t * c     = __s2c_chunk(iptr);
    __s_chunk_t * c_end = __s2c_chunk(iptr + c->size + __sc_fl_size);

    printf("%d %-4zu [%p ... %p] %d %-4zu\n",
           c->busy,
           c->size,
           c,
           c_end,
           c_end->busy,
           c_end->size);

    iptr = __s2c_ptr(c_end) + __sc_fl_size;
    ++blocks_count;
  }

  const __s_uintptr_t mapped_memory = __s2c_uiptr(__s->cursor - __s->start);
  printf("total: %zu (%zu)\n",
         mapped_memory - (blocks_count * __sc_flbd_size),
         mapped_memory);
#endif
}

__sattr_flatten_veccall static inline void *
    __salloc_move_cursor(register salloc_t * const restrict __s,
                         register const __s_uintptr_t       size) {
  register const __s_uintptr_t chunk_size = size + __sc_flbd_size;

  if ((__s->cursor + chunk_size) <= __s->end) {
    register __s_ptr_t out = __s->cursor;

    register __s_chunk_t *     header  = __s2c_chunk(out);
    register __s_chunk_t *     footer  = __s2c_chunk(out + size + __sc_fl_size);
    register const __s_chunk_t payload = {size, 1, 0};

    *header = payload;
    *footer = payload;

    register __s_ptr_t new_cursor = out + chunk_size;
    __s->cursor                   = new_cursor;

    return __sc_chunk_get_ptr(out);
  } else {
    return NULL;
  }
}
__sattr_veccall_overload static inline void *
    salloc(register salloc_t * const restrict __s, register const __s_size_t __size) {
  register const __s_uintptr_t aligned_size = __sc_align_size(__size);
  register void * restrict     out          = NULL;

  /**
   * TODO: Searching for a best available non busy memory chunk for allocation
   */

  out = __salloc_move_cursor(__s, aligned_size);

  return out;
}
__sattr_flatten_veccall_overload static inline void *
    salloc(register salloc_t * const restrict __s,
           register const __s_size_t          __size,
           register const __s_size_t          __nmemb) {
  const __s_size_t __arr_size = __nmemb * __size;
  return salloc(__s, __arr_size);
}

__sattr_flatten_veccall static inline void __sfree_frag(register __s_ptr_t  __iptr,
                                                        register __s_cptr_t __baseptr) {
  const __s_uintptr_t __base_size = __sc_get_size(__baseptr);
  const __s_uintptr_t __iptr_size = __sc_get_size(__iptr);
  const __s_uintptr_t __frag_size = __base_size + __sc_flbd_size + __iptr_size;

  __s_chunk_t *     __iptr_header  = __s2c_chunk(__iptr);
  __s_chunk_t *     __iptr_footer  = __s2c_chunk(__iptr + __frag_size + __sc_fl_size);
  const __s_chunk_t __iptr_payload = {__frag_size, __sc_not_busy};

  *__iptr_header = __iptr_payload;
  *__iptr_footer = __iptr_payload;
}
__sattr_flatten_veccall static inline __s_uint8_t
    __sfree_shrink_cursor(register salloc_t * const restrict __s,
                          register __s_cptr_t                __ptr) {
  __s_uint8_t is_moved = 0;

  if (__s->cursor <= (__ptr + __sc_get_size(__ptr) + __sc_flbd_size)) {
    __s->cursor = __ptr;
    is_moved    = 1;
  }

  return is_moved;
}
__sattr_veccall_overload static inline void sfree(register salloc_t * const restrict __s,
                                                  register void * restrict __ptr) {
  sfree(__ptr);

  const __s_cptr_t __s_start  = __s->start;
  const __s_cptr_t __s_cursor = __s->cursor;

  __s_chunk_t * __ptr_chunk = __sc_ptr_get_chunk(__ptr);
  __s_ptr_t     __iptr      = __s2c_ptr(__ptr_chunk);

  /**
   * Left-side memory fragmentation.
   * Before:
   * [busy] [while_free_chunks] [free_chunk1] [__ptr chunk] [right_side_mem]
   * After:
   * [busy] [   new_big_damn_ass_fragmented_memory_chunk  ] [right_side_mem]
   */
  if (__iptr > __s_start) {
    __s_ptr_t __baseptr = __iptr;
    __iptr = __baseptr - __sc_flbd_size - __sc_get_size(__baseptr - __sc_fl_size);

    while (__iptr >= __s_start && __sc_is_free(__iptr)) {
      __sfree_frag(__iptr, __baseptr);

      __s_ptr_t __prev_ptr = __iptr - __sc_fl_size;
      if (__s_start <= __prev_ptr) {
        __s_chunk_t * __prev_chunk = __s2c_chunk(__prev_ptr);
        __prev_ptr                 = __prev_ptr - __prev_chunk->size - __sc_fl_size;
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
   * [left_side_mem] [ left_side_fragmented_chunk ] [free_chunk1] [busy]
   * After:
   * [left_side_mem] [ new_big_damn_ass_fragmented_memory_chunk ] [busy]
   */
  {
    __s_ptr_t __baseptr = __iptr;
    __iptr              = __baseptr + __sc_get_size(__baseptr) + __sc_flbd_size;

    while (__iptr < __s_cursor && __sc_is_free(__iptr)) {
      __sfree_frag(__baseptr, __iptr);
      __iptr += __sc_get_size(__iptr) + __sc_flbd_size;
    }

    __sfree_shrink_cursor(__s, __baseptr);
  }
}

__sattr_flatten_veccall_overload static inline void
    sfree(register void * restrict __ptr) {
  __s_chunk_t *                header     = __sc_ptr_get_chunk(__ptr);
  register const __s_uintptr_t chunk_size = header->size;
  __s_chunk_t *                footer     = __s2c_chunk(__ptr + chunk_size);
  const __s_chunk_t            payload    = {chunk_size, __sc_not_busy};

  *header = payload;
  *footer = payload;
}

#endif /* __SALLOC_INTERNAL_H__ */
