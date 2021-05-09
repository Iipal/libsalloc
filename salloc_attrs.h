#ifndef __SALLOC_ATTRS_H__
#define __SALLOC_ATTRS_H__

#if !defined(__SALLOC_H__)
#  error "Do not include `salloc_attrs.h` directly."
#endif

#if __has_extension(cxx_attributes) || __has_feature(cxx_attributes)
#  define __is_cpp_attr__ 1
#else
#  define __is_cpp_attr__ 0
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L
#  define __is_c2x_support__ 1
#else
#  define __is_c2x_support__ 0
#endif

#if __GNUC__ >= 3
#  define __is_glibc_likely__   1
#  define __is_glibc_unlikely__ 1
#else
#  define __is_glibc_likely__   0
#  define __is_glibc_unlikely__ 0
#endif

#if __is_glibc_likely__
#  if __is_cpp_attr__
// for this one i'm not sure about syntax for now, because it's has no examples yet
#    define __salloc_likely(_cond) [[likely(_cond)]]
#  else
#    define __salloc_likely(_cond) __builtin_expect((_cond), 1)
#  endif
#else
#  define __salloc_likely(_cond) (_cond)
#endif

#ifdef __is_glibc_unlikely__
#  if __is_cpp_attr__
#    define __salloc_unlikely(_cond) [[unlikely(_cond)]]
#  else
#    define __salloc_unlikely(_cond) __builtin_expect((cond), 1)
#  endif
#else
#  define __salloc_unlikely(_cond) (_cond)
#endif

#if __is_cpp_attr__
#  define libsalloc_attr_alignof(x) [[gnu::aligned(alignof(x))]]
#else
#  if __has_attribute(aligned)
#    define libsalloc_attr_alignof(x) __attribute__((aligned(alignof(x))))
#  else
#    define libsalloc_attr_alignof(x)
#  endif
#endif

#if __is_cpp_attr__
#  define libsalloc_attr_transparent [[gnu::transparent_union]]
#else
#  if __has_attribute(transparent_union)
#    define libsalloc_attr_transparent __attribute__((transparent_union))
#  else
#    define libsalloc_attr_transparent
#  endif
#endif

#if __is_cpp_attr__
#  define libsalloc_attr_veccall [[clang::vectorcall]]
#else
#  if __has_attribute(vectorcall)
#    define libsalloc_attr_veccall __attribute__((vectorcall))
#  else
#    define libsalloc_attr_veccall
#  endif
#endif

#if __is_cpp_attr__
#  define libsalloc_attr_const [[gnu::const]]
#else
#  if __has_attribute(const)
#    define libsalloc_attr_const __attribute__((const))
#  else
#    define libsalloc_attr_const
#  endif
#endif

#if __is_cpp_attr__
#  define libsalloc_attr_overload [[clang::overloadable]]
#else
#  if __has_attribute(overloadable)
#    define libsalloc_attr_overload __attribute__((overloadable))
#  else
#    define libsalloc_attr_overload
#    error "Your compiler doesn't support overloadable attribute."
#  endif
#endif

#if __is_cpp_attr__
#  define libsalloc_attr_flatten [[gnu::flatten]]
#else
#  if __has_attribute(flatten)
#    define libsalloc_attr_flatten __attribute__((flatten))
#  else
#    define libsalloc_attr_flatten
#  endif
#endif

#if __is_cpp_attr__
#  define libsalloc_attr_munused [[maybe_unused]]
#else
#  if __has_attribute(unused)
#    define libsalloc_attr_munused __attribute__((unused))
#  else
#    define libsalloc_attr_munused
#  endif
#endif

#if __is_cpp_attr__
#  define libsalloc_attr_deprecated(msg) [[gnu::deprecated(msg)]]
#else
#  if __has_attribute(deprecated)
#    define libsalloc_attr_deprecated(msg) __attribute__((deprecated(msg)))
#  else
#    define libsalloc_attr_deprecated(msg)
#  endif
#endif

#define libsalloc_attr_veccall_const libsalloc_attr_veccall libsalloc_attr_const
#define libsalloc_attr_veccall_const_overload \
  libsalloc_attr_veccall libsalloc_attr_const libsalloc_attr_overload
#define libsalloc_attr_veccall_overload libsalloc_attr_veccall libsalloc_attr_overload
#define libsalloc_attr_flatten_veccall_overload \
  libsalloc_attr_flatten libsalloc_attr_veccall libsalloc_attr_overload

#endif /* __SALLOC_ATTRS_H__ */
