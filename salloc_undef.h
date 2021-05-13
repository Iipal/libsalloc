#ifndef __SALLOC_UNDEF_H__
#define __SALLOC_UNDEF_H__

#if defined(SALLOC_MACROS_AFTER_USE)
#  warning "salloc macroses still defined."
#elif defined(SALLOC_ATTRS_AFTER_USE)
#  warning "salloc attributes still defined."
#elif !defined(__SALLOC_MACROS_H__)
#  error "Do not include `salloc_undef.h` directly."
#endif

#if defined(SALLOC_MACROS_AFTER_USE)
#  undef __is_salloc_casts_defined__
#  undef __is_salloc_chunks_defined__
#endif
#if defined(SALLOC_ATTRS_AFTER_USE)
#  undef __is_salloc_attrs_defined__
#endif

#if __is_salloc_casts_defined__
#  undef __is_salloc_casts_defined__
#  undef __s2c_vptr
#  undef __s2c_ui8ptr
#  undef __s2c_uiptr
#  undef __s2c_sptr
#  undef __s2c_scptr
#  undef __s2c_chunk
#endif

#if __is_salloc_chunks_defined__
#  undef __is_salloc_chunks_defined__
#  undef __sc_align_default
#  undef __sc_align_bits
#  undef __sc_align
#  undef __sc_align_size
#  undef __sc_align_forward
#  undef __sc_fl_size
#  undef __sc_flbd_size
#  undef __sc_wflbd_size
#  undef __sc_ptr_get_chunk
#  undef __sc_chunk_get_ptr
#  undef __sc_busy
#  undef __sc_not_busy
#  undef __sc_get_busy
#  undef __sc_get_size
#  undef __sc_is_free
#endif

#if __is_salloc_attrs_defined__
#  undef __is_salloc_attrs_defined__
#  undef __is_cpp_attr__
#  undef __sattr_veccall
#  undef __sattr_const
#  undef __sattr_overload
#  undef __sattr_flatten
#  undef __sattr_munused
#  undef __sattr_packed
#  undef __sattr_deprecated
#  undef __sattr_veccall_const
#  undef __sattr_veccall_const_overload
#  undef __sattr_veccall_overload
#  undef __sattr_flatten_veccall_overload
#  undef __sattr_flatten_veccall
#endif

#endif /* __SALLOC_UNDEF_H__ */
