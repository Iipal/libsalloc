#ifndef __SALLOC_UNDEF_H__
#define __SALLOC_UNDEF_H__

#if defined(SALLOC_MACROS_AFTER_USE)
#  warning "salloc macroses still defined."
#elif !defined(__SALLOC_MACROS_H__)
#  error "Do not include `salloc_undef.h` directly."
#endif

#if !defined(SALLOC_MACROS_AFTER_USE)

#  if __is_salloc_vec_accessors_defined__
#    undef __is_salloc_vec_accessors_defined__
#    undef __sva_alloc_buff
#    undef __sva_alloc_buff_len
#    undef __sva_alloc_prev_offset
#    undef __sva_alloc_curr_offset
#    undef __sva_start
#    undef __sva_end
#    undef __sva_cursor
#    undef __sva_capacity
#    undef __sva_get_start
#    undef __sva_get_end
#    undef __sva_get_cursor
#    undef __sva_get_capacity
#  endif

#  if __is_salloc_casts_defined__
#    undef __is_salloc_casts_defined__
#    undef __s2c_2vec
#    undef __s2c_2vecptr
#    undef __s2c_2mem
#    undef __s2c_2memptr
#    undef __s2c_mem2vec
#    undef __s2c_mem2vecptr
#    undef __s2c_memptr2vec
#    undef __s2c_memptr2vecptr
#    undef __s2c_vec2mem
#    undef __s2c_vec2memptr
#    undef __s2c_vecptr2mem
#    undef __s2c_vecptr2memptr
#  endif

#  if __is_salloc_chunks_defined__
#    undef __is_salloc_chunks_defined__
#    undef __sc_align_default
#    undef __sc_align_bits
#    undef __sc_align
#    undef __sc_align_size
#    undef __sc_align_forward
#    undef __sc_fl_size
#    undef __sc_flbd_size
#    undef __sc_wflbd_size
#    undef __ptr_get_sc
#    undef __sc_get_ptr
#    undef __sc_mask_inuse
#    undef __sc_mask_size
#    undef __sc_get_value
#    undef __sc_clear_inuse
#    undef __sc_clear_size
#    undef __sc_clear_value
#    undef __sc_is_inuse
#    undef __sc_get_size
#    undef __sc_set_inuse
#    undef __sc_set_size
#    undef __sc_unset_inuse
#    undef __sc_unset_size
#    undef __sc_valid_start
#    undef __sc_valid_end
#  endif

#endif

#endif /* __SALLOC_UNDEF_H__ */
