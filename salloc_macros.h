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

#  define __sva_move_cursor(v, s) ((__s2c_t(v))(__s2c_ui8ptr(v) + __s2c_uiptr(s)))

#  define __sva_map(v) \
    { __sva_get_start(v), __sva_get_end(v), __sva_get_cursor(v), __sva_get_capacity(v) }
#  define __sva_map_buff(v, s, e, c, l) \
    { (__s2c_tx(v))(s), (__s2c_tx(v))(e), (__s2c_tx(v))(c), (__s2c_tx(v))(l) }

#endif

/**
 * Most convenient type conversions
 */
#ifndef __is_salloc_casts_defined__
#  define __is_salloc_casts_defined__ 1

/** __s2c prefix stands as shortcut for __salloc_too_cast **/

#  define __s2c_vptr(x)    ((void *)(x))
#  define __s2c_ui8ptr(x)  ((uint8_t *)(x))
#  define __s2c_uiptr(x)   ((uintptr_t)(x))
#  define __s2c_size(x)    ((size_t)(x))
#  define __s2c_chunk(x)   ((salloc_chunk_t *)((void *)(x)))
#  define __s2c_vec4(x)    ((salloc_vec4_t)(x))
#  define __s2c_vec4ptr(x) ((salloc_vec4_t *)(x))
#  define __s2c_slc(x)     ((salloc_t)(x))
#  define __s2c_slcptr(x)  ((salloc_t *)(x))

#  define __s2c_t(x)    typeof(x)
#  define __s2c_tx(v)   typeof((v)[0])
#  define __s2c_slct(x) (salloc_t) __s2c_t(x)

#  define __s2c_chunk_map(s, u) \
    (salloc_chunk_t) { (s), (u) }
#  define __s2c_slc_map_vec(v) __s2c_slc((__s2c_t(v))__sva_map(v))
#  define __s2c_slc_map_buff(v, s, e, c, l) \
    __s2c_slc((__s2c_t(v))__sva_map_buff(v, s, e, c, l))

#  define __s2c_slc_vec(s)       __s2c_vec4((s).vec)
#  define __s2c_slc_vecptr(s)    __s2c_vec4ptr(&((s).vec))
#  define __s2c_slcptr_vec(s)    __s2c_vec4((s)->vec)
#  define __s2c_slcptr_vecptr(s) __s2c_vec4ptr(&((s)->vec))

#  define __s2c_vec4_slc(v)       __s2c_slc(v)
#  define __s2c_vec4_slcptr(v)    __s2c_slcptr(&(v))
#  define __s2c_vec4ptr_slc(v)    __s2c_slc(*(v))
#  define __s2c_vec4ptr_slcptr(v) __s2c_slcptr(v)

#endif

/**
 * For work with memory chunks as with bi-directional implicit free-list
 */
#ifndef __is_salloc_chunks_defined__
#  define __is_salloc_chunks_defined__ 1

/** __sc prefix stands as shortcut for __salloc_chunk **/

#  define __sc_align_default (alignof(void *) * 2)
#  define __sc_align_bits    (__s2c_uiptr(__sc_align_default - 1ul))
#  define __sc_align(x) \
    (((x) % __sc_align_default) ? ((x) + ((~(x)&__sc_align_bits) + 1UL)) : (x))
#  define __sc_align_size(x)    (__s2c_size(__sc_align(x)))
#  define __sc_align_forward(x) (__s2c_uiptr(__sc_align(x)))

#  define __sc_v_inuse  1 // v stands for value
#  define __sc_vn_inuse 0 // n stands for not

#  define __sc_fl_size       (__s2c_uiptr(sizeof(salloc_chunk_t))) // fl stands for free-list
#  define __sc_flbd_size     (__s2c_uiptr(__sc_fl_size * 2)) // bd stands for bi-directional
#  define __sc_wflbd_size(x) ((x) + __sc_flbd_size)          // w  stands for with

#  define __sc_ptr_get_chunk(x) (__s2c_chunk(__s2c_ui8ptr(x) - __sc_fl_size))
#  define __sc_chunk_get_ptr(x) ((__s2c_ui8ptr(x)) + __sc_fl_size)

#  define __sc_clear_inuse(x) (__s2c_chunk(x)->inuse = __sc_vn_inuse)
#  define __sc_clear_size(x)  (__s2c_chunk(x)->size = __sc_vn_inuse)
#  define __sc_clear(x)       (__s2c_chunk(x) = (salloc_chunk_t){0, 0})

#  define __sc_is_inuse(x) (__s2c_chunk(x)->inuse)
#  define __sc_get_size(x) (__s2c_chunk(x)->size)

#  define __sc_fl_shift(x, s) (__s2c_ui8ptr(x) + __s2c_uiptr(s) + __sc_fl_size)

#  define __sc_set(x, s) \
    { \
      *__s2c_chunk(x)                   = __s2c_chunk_map(s, __sc_v_inuse); \
      *__s2c_chunk(__sc_fl_shift(x, s)) = __s2c_chunk_map(s, __sc_v_inuse); \
    }

#  define __sc_unset(x, s) \
    { \
      *__s2c_chunk(x)                   = __s2c_chunk_map(0, __sc_vn_inuse); \
      *__s2c_chunk(__sc_fl_shift(x, s)) = __s2c_chunk_map(0, __sc_vn_inuse); \
    }

#  define __sc_valid_start(v, offset) \
    ((__s2c_ui8ptr(__sva_get_cursor(v)) - __s2c_uiptr(offset)) >= \
     __s2c_ui8ptr(__sva_get_start(v)))
#  define __sc_valid_end(v, offset) \
    ((__s2c_ui8ptr(__sva_get_cursor(v)) + __s2c_uiptr(offset)) <= \
     __s2c_ui8ptr(__sva_get_end(v)))

#endif

#endif /* __SALLOC_MACROS_H__ */
