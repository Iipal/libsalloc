#ifndef __SALLOC_ATTRS_H__
#define __SALLOC_ATTRS_H__

#if !defined(__SALLOC_H__)
#  error "Do not include `salloc_attrs.h` directly."
#endif

#define __is_salloc_attrs_defined__ 1

#if __has_extension(cxx_attributes) || __has_feature(cxx_attributes) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L)
#  define __is_cpp_attr__ 1
#else
#  define __is_cpp_attr__ 0
#endif

#if __is_cpp_attr__
#  define __sattr_veccall [[clang::vectorcall]]
#else
#  if __has_attribute(vectorcall)
#    define __sattr_veccall __attribute__((vectorcall))
#  else
#    define __sattr_veccall
#  endif
#endif

#if __is_cpp_attr__
#  define __sattr_const [[gnu::const]]
#else
#  if __has_attribute(const)
#    define __sattr_const __attribute__((const))
#  else
#    define __sattr_const
#  endif
#endif

#if __is_cpp_attr__
#  define __sattr_overload [[clang::overloadable]]
#else
#  if __has_attribute(overloadable)
#    define __sattr_overload __attribute__((overloadable))
#  else
#    define __sattr_overload
#    error "Your compiler doesn't support overloadable attribute."
#  endif
#endif

#if __is_cpp_attr__
#  define __sattr_flatten [[gnu::flatten]]
#else
#  if __has_attribute(flatten)
#    define __sattr_flatten __attribute__((flatten))
#  else
#    define __sattr_flatten
#  endif
#endif

#if __is_cpp_attr__
#  define __sattr_munused [[maybe_unused]]
#else
#  if __has_attribute(unused)
#    define __sattr_munused __attribute__((unused))
#  else
#    define __sattr_munused
#  endif
#endif

#if __is_cpp_attr__
#  define __sattr_packed [[packed]]
#else
#  if __has_attribute(packed)
#    define __sattr_packed __attribute__((packed))
#  else
#    define __sattr_packed
#  endif
#endif

#if __is_cpp_attr__
#  define __sattr_deprecated(msg) [[gnu::deprecated(msg)]]
#else
#  if __has_attribute(deprecated)
#    define __sattr_deprecated(msg) __attribute__((deprecated(msg)))
#  else
#    define __sattr_deprecated(msg)
#  endif
#endif

#define __sattr_veccall_const            __sattr_veccall __sattr_const
#define __sattr_veccall_const_overload   __sattr_veccall __sattr_const __sattr_overload
#define __sattr_veccall_overload         __sattr_veccall __sattr_overload
#define __sattr_flatten_veccall_overload __sattr_flatten __sattr_veccall __sattr_overload
#define __sattr_flatten_veccall          __sattr_flatten __sattr_veccall

#endif /* __SALLOC_ATTRS_H__ */
