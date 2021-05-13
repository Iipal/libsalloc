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
#    undef __s2c_vptr
#    undef __s2c_ui8ptr
#    undef __s2c_uiptr
#    undef __s2c_sptr
#    undef __s2c_scptr
#    undef __s2c_chunk
#    undef __s2c_t
#    undef __s2c_tx
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
#    undef __sc_ptr_get_chunk
#    undef __sc_chunk_get_ptr
#    undef __sc_busy
#    undef __sc_not_busy
#    undef __sc_get_busy
#    undef __sc_get_size
#    undef __sc_is_free
#  endif

#endif

#endif /* __SALLOC_UNDEF_H__ */
