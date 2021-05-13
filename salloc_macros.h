#ifndef __SALLOC_MACROS_H__
#define __SALLOC_MACROS_H__

#if !defined(__SALLOC_INTERNAL_H__)
#  error "Do not include `salloc_macros.h` directly."
#endif

#ifndef __SALLOC_TYPES_H__
#  include "salloc_types.h"
#endif

#ifndef __is_salloc_vec_accessors_defined__
#  define __is_salloc_vec_accessors_defined__ 1

/** __sva prefix stands as shortcut for __salloc_vector_accessor **/

#  define __sva_alloc_buff        (0)
#  define __sva_alloc_buff_len    (1)
#  define __sva_alloc_prev_offset (2)
#  define __sva_alloc_curr_offset (3)

#  define __sva_start    0
#  define __sva_end      1
#  define __sva_cursor   2
#  define __sva_capacity 3

#  define __sva_get_start(v)    ((v)[__sva_start])
#  define __sva_get_end(v)      ((v)[__sva_end])
#  define __sva_get_cursor(v)   ((v)[__sva_cursor])
#  define __sva_get_capacity(v) ((v)[__sva_capacity])

#endif

/**
 * Most convenient type conversions
 */
#ifndef __is_salloc_casts_defined__
#  define __is_salloc_casts_defined__ 1

/** __s2c prefix stands as shortcut for __salloc_to_cast **/

#  define __s2c_vptr(x)   ((void *)(x))
#  define __s2c_ui8ptr(x) ((uint8_t *)(x))
#  define __s2c_uiptr(x)  ((uintptr_t)(x))

#  define __s2c_sptr(x)  ((sptr_t)(x))
#  define __s2c_scptr(x) ((scptr_t)(x))
#  define __s2c_chunk(x) ((salloc_chunk_t *)(__s2c_vptr(x)))

#  define __s2c_t(x)  typeof(x)
#  define __s2c_tx(v) typeof((v)[0])

#endif

/**
 * For work with memory chunks as with bi-directional implicit free-list
 */
#ifndef __is_salloc_chunks_defined__
#  define __is_salloc_chunks_defined__ 1

/** __sc prefix stands as shortcut for __salloc_chunk **/

#  define __sc_align_default __s2c_uiptr(alignof(void *) * 2)
#  define __sc_align_bits    (__sc_align_default - __s2c_uiptr(1))
#  define __sc_align(x) \
    (((x) % __sc_align_default) ? ((x) + ((~(x)&__sc_align_bits) + 1UL)) : (x))
#  define __sc_align_size(x) __s2c_uiptr(__sc_align(x))

#  define __sc_fl_size       __s2c_uiptr(sizeof(salloc_chunk_t)) // fl stands for free-list
#  define __sc_flbd_size     (__sc_fl_size * 2) // bd stands for bi-directional
#  define __sc_wflbd_size(x) (__s2c_uiptr(x) + __sc_flbd_size) // w stands for with

#  define __sc_ptr_get_chunk(x) __s2c_chunk(__s2c_sptr(x) - __sc_fl_size)
#  define __sc_chunk_get_ptr(x) (__s2c_sptr(x) + __sc_fl_size)

#  define __sc_busy     1
#  define __sc_not_busy 0

#  define __sc_get_busy(x) (__s2c_chunk(x)->busy)
#  define __sc_get_size(x) (__s2c_chunk(x)->size)
#  define __sc_is_free(x)  !__sc_get_busy(x)

#endif

#endif /* __SALLOC_MACROS_H__ */
