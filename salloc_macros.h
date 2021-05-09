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

#  define __sva_move_cursor(v, offset) \
    ((__s2c_tx(v))(__s2c_ui8ptr(__sva_get_cursor(v)) + (uintptr_t)(offset)))

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

#  define __s2c_ui8ptr(x) ((uint8_t *)(x))
#  define __s2c_t(v)      typeof(v)
#  define __s2c_tx(v)     typeof((v)[0])

#  define __s2c_vec4(x)    ((salloc_vec4_t)(x))
#  define __s2c_vec4ptr(x) ((salloc_vec4_t *)(x))
#  define __s2c_slc(x)     ((salloc_t)(x))
#  define __s2c_slct(x)    (salloc_t) __s2c_t(x)
#  define __s2c_slcptr(x)  ((salloc_t *)(x))

#  define __s2c_slc_map_vec(v) (salloc_t)(__s2c_t(v)) __sva_map(v)
#  define __s2c_slc_map_buff(v, s, e, c, l) \
    (salloc_t)(__s2c_t(v)) __sva_map_buff(v, s, e, c, l)

#  define __s2c_slc_vec(m)       (__s2c_vec4((m).vec))
#  define __s2c_slc_vecptr(m)    (__s2c_vec4ptr(&((m).vec)))
#  define __s2c_slcptr_vec(m)    (__s2c_vec4((m)->vec))
#  define __s2c_slcptr_vecptr(m) (__s2c_vec4ptr(&(m)->vec))

#  define __s2c_vec4_slc(v)       (__s2c_slc(v))
#  define __s2c_vec4_slcptr(v)    (__s2c_slcptr(&(v)))
#  define __s2c_vec4ptr_slc(v)    (__s2c_slc(*(v)))
#  define __s2c_vec4ptr_slcptr(v) (__s2c_slcptr(v))

#endif

/**
 * For work with memory chunks as with bi-directional implicit free-list
 */
#ifndef __is_salloc_chunks_defined__
#  define __is_salloc_chunks_defined__ 1

/** __sc prefix stands as shortcut for __salloc_chunk **/

#  define __sc_align_default (alignof(void *) * 2)
#  define __sc_align_bits    ((uintptr_t)(__sc_align_default - 1ul))
#  define __sc_align(x) \
    (((x) % __sc_align_default) ? ((x) + ((~(x)&__sc_align_bits) + 1UL)) : (x))
#  define __sc_align_size(x)    ((size_t)__sc_align(x))
#  define __sc_align_forward(x) ((uintptr_t)__sc_align(x))

#  define __sc_fl_size       (sizeof(uintptr_t))     // fl stands for free-list
#  define __sc_flbd_size     (sizeof(uintptr_t) * 2) // bd stands for bi-directional
#  define __sc_wflbd_size(x) ((x) + __sc_flbd_size)  // w  stands for with

#  define __ptr_get_sc(x) ((x)-__sc_fl_size)
#  define __sc_get_ptr(x) ((uint8_t *)(x) + __sc_fl_size)

#  define __sc_mask_inuse ((uintptr_t)(0x0000000000000001UL))
#  define __sc_mask_size  ((uintptr_t)(0xFFFFFFFFFFFFFFF0UL))

#  define __sc_get_value(x) (*((uintptr_t *)((uint8_t *)x)))

#  define __sc_clear_inuse(x) (__sc_get_value(x) &= ~__sc_mask_inuse)
#  define __sc_clear_size(x)  (__sc_get_value(x) &= ~__sc_mask_size)
#  define __sc_clear_value(x) (__sc_get_value(x) ^= __sc_get_value(x))

#  define __sc_is_inuse(x) !(__sc_get_value(x) & __sc_mask_size)
#  define __sc_get_size(x) (__sc_get_value(x) & __sc_mask_size)

#  define __sc_set_inuse(x, _size) \
    { \
      __sc_clear_inuse(x); \
      __sc_get_value(x) |= __sc_mask_inuse; \
      __sc_clear_inuse((x) + (_size) + __sc_fl_size); \
      __sc_get_value((x) + (_size) + __sc_fl_size) |= __sc_mask_inuse; \
    }
#  define __sc_set_size(x, _size) \
    { \
      __sc_clear_size(x); \
      __sc_get_value(x) |= _size; \
      __sc_clear_size((x) + (_size) + __sc_fl_size); \
      __sc_get_value((x) + (_size) + __sc_fl_size) |= _size; \
    }

#  define __sc_unset_inuse(x, _size) \
    { \
      __sc_clear_inuse(x); \
      __sc_clear_inuse((x) + (_size) + __sc_fl_size); \
    }
#  define __sc_unset_size(x, _size) \
    { \
      __sc_clear_size(x); \
      __sc_clear_size((x) + (_size) + __sc_fl_size); \
    }

#  define __sc_valid_start(v, offset) \
    (((uint8_t *)__sva_get_cursor(v) - (uintptr_t)(offset)) >= \
     (uint8_t *)__sva_get_start(v))
#  define __sc_valid_end(v, offset) \
    (((uint8_t *)__sva_get_cursor(v) + (uintptr_t)(offset)) <= \
     (uint8_t *)__sva_get_end(v))

#endif

#endif /* __SALLOC_MACROS_H__ */
