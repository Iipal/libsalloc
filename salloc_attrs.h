#ifndef __SALLOC_ATTRS_H__
#define __SALLOC_ATTRS_H__

#if defined(SALLOC_UNSAFE)
#  undef SALLOC_UNSAFE
/**
 * Removes some nullptr-checks
 */
#  define SALLOC_UNSAFE 1
#endif

#if !defined(__is_c2x_support__)
#  if __STDC__ > 201710L
#    define __is_c2x_support__ 1
#  else
#    define __is_c2x_support__ 0
#  endif
#endif

#if __is_c2x_support__
#  define alignof(x) (_Alignof(x))
#else
#  define alignof(x) (__alignof__(x))
#endif

#if __is_c2x_support__
#  define libsalloc_attr_alignof(x) [[gnu::aligned(alignof(x))]]
#else
#  if __has_attribute(aligned)
#    define libsalloc_attr_alignof(x) __attribute__((aligned(alignof(x))))
#  else
#    define libsalloc_attr_alignof(x)
#  endif
#endif

#if __is_c2x_support__
#  define libsalloc_attr_transparent [[gnu::transparent_union]]
#else
#  if __has_attribute(transparent_union)
#    define libsalloc_attr_transparent __attribute__((transparent_union))
#  else
#    define libsalloc_attr_transparent
#  endif
#endif

#if __is_c2x_support__
#  define libsalloc_attr_veccall [[clang::vectorcall]]
#else
#  if __has_attribute(vectorcall)
#    define libsalloc_attr_veccall __attribute__((vectorcall))
#  else
#    define libsalloc_attr_veccall
#  endif
#endif

#if __is_c2x_support__
#  define libsalloc_attr_const [[gnu::const]]
#else
#  if __has_attribute(const)
#    define libsalloc_attr_const __attribute__((const))
#  else
#    define libsalloc_attr_const
#  endif
#endif

#if __is_c2x_support__
#  define libsalloc_attr_overload [[clang::overloadable]]
#else
#  if __has_attribute(overloadable)
#    define libsalloc_attr_overload __attribute__((overloadable))
#  else
#    define libsalloc_attr_overload
#    error "Your compiler doesn't support overloadable attribute."
#  endif
#endif

#if __is_c2x_support__
#  define libsalloc_attr_flatten [[gnu::flatten]]
#else
#  if __has_attribute(flatten)
#    define libsalloc_attr_flatten __attribute__((flatten))
#  else
#    define libsalloc_attr_flatten
#  endif
#endif

#if __is_c2x_support__
#  define libsalloc_attr_munused [[maybe_unused]]
#else
#  if __has_attribute(unused)
#    define libsalloc_attr_munused __attribute__((unused))
#  else
#    define libsalloc_attr_munused
#  endif
#endif

#if __is_c2x_support__
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
