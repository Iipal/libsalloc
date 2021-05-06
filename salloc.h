#ifndef __SALLOC_H__
#define __SALLOC_H__

#include <stddef.h> /* for size_t */
#include <stdint.h> /* for uintptr */

#pragma clang diagnostic ignored "-Wgcc-compat"

/**
 * Some helper type-operator-like-macroses defines
 */

#if !defined(auto) && (!defined(__auto_defined__) || !(__auto_defined__))
#  define __auto_defined__ 1
#  define auto __auto_type
#endif

#if !defined(nullptr) && (!defined(__nullptr_defined__) || !(__nullptr_defined__))
#  define __nullptr_defined__ 1
#  define nullptr ((void *)0)
#endif

#if !defined(typeof) && (!defined(__typeof_defined__) || !(__typeof_defined__))
#  define __typeof_defined__ 1
#  define typeof(x) __typeof__(x)
#endif
#endif

#if !defined(alignof) && (!defined(__alignof_defined__) || !(__alignof_defined__))
#define __alignof_defined__ 1
#if __STDC__ > 201710L
#  define alignof(x) (_Alignof(x))
#else
#  define alignof(x) (__alignof__(x))
#endif
#endif

#if !defined(from) && (!defined(__from_defined__) || !(__from_defined__))
#define __from_defined__ 1
#define from
#endif

/**
 * Custom constants and attributes
 */

#if !defined(SALLOC_BUFFER)
#define SALLOC_BUFFER (0)
#endif
#if !defined(SALLOC_BUFFER_LENGTH)
#define SALLOC_BUFFER_LENGTH (1)
#endif
#if !defined(SALLOC_PREVIOUS_OFFSET)
#define SALLOC_PREVIOUS_OFFSET (2)
#endif
#if !defined(SALLOC_CURRENT_OFFSET)
#define SALLOC_CURRENT_OFFSET (3)
#endif

#if !defined(SALLOC_DEFAULT_ALIGNMENT)
#define SALLOC_DEFAULT_ALIGNMENT (alignof(void *) * 2)
#endif

#if !defined(__is_c2x_support__)
#if __STDC__ > 201710L
#  define __is_c2x_support__ 1
#else
#  define __is_c2x_support__ 0
#endif
#endif

#if !defined(SALLOC_ATTR_ALIGNOF)
#if __is_c2x_support__
#  define SALLOC_ATTR_ALIGNOF(x) [[gnu::aligned(alignof(x))]]
#else
#  if __has_attribute(aligned)
#    define SALLOC_ATTR_ALIGNOF(x) __attribute__((aligned(alignof(x))))
#  else
#    define SALLOC_ATTR_ALIGNOF(x)
#  endif
#endif
#endif

#if !defined(SALLOC_ATTR_TRANSPARENT)
#if __is_c2x_support__
#  define SALLOC_ATTR_TRANSPARENT [[gnu::transparent_union]]
#else
#  if __has_attribute(transparent_union)
#    define SALLOC_ATTR_TRANSPARENT __attribute__((transparent_union))
#  else
#    define SALLOC_ATTR_TRANSPARENT
#  endif
#endif
#endif

#if !defined(SALLOC_ATTR_VECCALL)
#if __is_c2x_support__
#  define SALLOC_ATTR_VECCALL [[clang::vectorcall]]
#else
#  if __has_attribute(vectorcall)
#    define SALLOC_ATTR_VECCALL __attribute__((vectorcall))
#  else
#    define SALLOC_ATTR_VECCALL
#  endif
#endif

#if !defined(SALLOC_ATTR_CONST)
#  if __is_c2x_support__
#    define SALLOC_ATTR_CONST [[gnu::const]]
#  else
#    if __has_attribute(const)
#      define SALLOC_ATTR_CONST __attribute__((const))
#    else
#      define SALLOC_ATTR_CONST
#    endif
#  endif
#endif

#if !defined(SALLOC_ATTR_OVERLOAD)
#  if __is_c2x_support__
#    define SALLOC_ATTR_OVERLOAD [[clang::overloadable]]
#  else
#    if __has_attribute(overloadable)
#      define SALLOC_ATTR_OVERLOAD __attribute__((overloadable))
#    else
#      define SALLOC_ATTR_OVERLOAD
#      error "Your compiler doesn't support overloadable functions."
#    endif
#  endif
#endif

#if !defined(SALLOC_ATTR_FLATTEN)
#  if __is_c2x_support__
#    define SALLOC_ATTR_FLATTEN [[gnu::flatten]]
#  else
#    if __has_attribute(flatten)
#      define SALLOC_ATTR_FLATTEN __attribute__((flatten))
#    else
#      define SALLOC_ATTR_FLATTEN
#      error "Your compiler doesn't support flatten functions."
#    endif
#  endif
#endif

#if !defined(SALLOC_ATTR_MAYBE_UNUSED)
#  if __is_c2x_support__
#    define SALLOC_ATTR_MAYBE_UNUSED [[maybe_unused]]
#  else
#    if __has_attribute(unused)
#      define SALLOC_ATTR_MAYBE_UNUSED __attribute__((unused))
#    else
#      define SALLOC_ATTR_MAYBE_UNUSED
#    endif
#  endif
#endif

#if !defined(SALLOC_ATTR_DEPRECATED)
#  if __is_c2x_support__
#    define SALLOC_ATTR_DEPRECATED(msg) [[gnu::deprecated(msg)]]
#  else
#    if __has_attribute(deprecated)
#      define SALLOC_ATTR_DEPRECATED(msg) __attribute__((deprecated(msg)))
#    else
#      define SALLOC_ATTR_DEPRECATED(msg)
#    endif
#  endif
#endif

#if !defined(SALLOC_ATTR_VECCALL_CONST)
#  define SALLOC_ATTR_VECCALL_CONST SALLOC_ATTR_VECCALL SALLOC_ATTR_CONST
#endif
#if !defined(SALLOC_ATTR_VECCALL_CONST_OVERLOAD)
#  define SALLOC_ATTR_VECCALL_CONST_OVERLOAD \
    SALLOC_ATTR_VECCALL SALLOC_ATTR_CONST SALLOC_ATTR_OVERLOAD
#endif
#if !defined(SALLOC_ATTR_VECCALL_OVERLOAD)
#  define SALLOC_ATTR_VECCALL_OVERLOAD SALLOC_ATTR_VECCALL SALLOC_ATTR_OVERLOAD
#endif
#if !defined(SALLOC_ATTR_FLATTEN_VECCALL_OVERLOAD)
#  define SALLOC_ATTR_FLATTEN_VECCALL_OVERLOAD \
    SALLOC_ATTR_FLATTEN SALLOC_ATTR_VECCALL SALLOC_ATTR_OVERLOAD
#endif

/**
 * Custom types definition
 */

#if !defined(__v4pu_defined__) || !(__v4pu_defined__)
#  define __v4pu_defined__ 1
typedef uintptr_t __attribute__((__aligned__, __ext_vector_type__(4))) v4pu_t;
#endif

#if !defined(__salloc_vec_defined__) || !(__salloc_vec_defined__)
#  define __salloc_vec_defined__ 1

#  if defined(__v4pu_defined__)
typedef v4pu_t salloc_vec_t;
#  else
typedef uintptr_t __attribute__((__aligned__, __ext_vector_type__(4))) salloc_vec_t;
#  endif
#endif

#if !defined(__salloc_defined__) || !(__salloc_defined__)
#  define __salloc_defined__ 1

typedef struct s_salloc {
  unsigned char * restrict buffer;
  size_t                   buffer_length;
  uintptr_t                previous_offset;
  uintptr_t                current_offset;
} SALLOC_ATTR_ALIGNOF(salloc_vec_t) salloc_t;
#endif

#if !defined(__salloc_allocator_defined__) || !(__salloc_allocator_defined__)
#  define __salloc_allocator_defined__ 1

typedef union {
#  if __salloc_defined__
  salloc_t data;
#  else
  struct {
    unsigned char * restrict buffer;
    size_t                   buffer_length;
    uintptr_t                previous_offset;
    uintptr_t                current_offset;
  } data;
#  endif

#  if __salloc_vec_defined__
  salloc_vec_t vector;
#  else
  uintptr_t vector[4];
#  endif

} salloc_allocator_t SALLOC_ATTR_TRANSPARENT;

#endif

/**
 * Function Prototypes
 */

SALLOC_ATTR_VECCALL_CONST static inline salloc_vec_t
    salloc_init(register const void * const restrict buff,
                register const size_t                buff_length);

SALLOC_ATTR_VECCALL_OVERLOAD static inline void *
    salloc(register salloc_vec_t * restrict allocator, register const size_t size);
SALLOC_ATTR_VECCALL_OVERLOAD static inline void *
    salloc(register salloc_vec_t * const restrict allocator,
           register const size_t                  size,
           register const size_t                  align);

SALLOC_ATTR_FLATTEN_VECCALL_OVERLOAD static inline void *
    srealloc(register salloc_vec_t * const restrict allocator,
             register void * const restrict         old_memory,
             register const size_t                  old_size,
             register const size_t                  new_size);
SALLOC_ATTR_FLATTEN_VECCALL_OVERLOAD static inline void *
    srealloc(register salloc_vec_t * const restrict allocator,
             register void * const restrict         old_memory,
             register const size_t                  old_size,
             register const size_t                  new_size,
             register const size_t                  align);

SALLOC_ATTR_VECCALL_CONST_OVERLOAD static inline void
    sfree(register salloc_vec_t * restrict allocator, void * restrict pointer)
        SALLOC_ATTR_DEPRECATED("Not needed to implement: sfree(salloc_vec_t *, void *)") {
  /* Not needed to implement */
  (void)allocator;
  (void)pointer;
}
SALLOC_ATTR_VECCALL_OVERLOAD static inline void
    sfree(register salloc_vec_t * const restrict allocator);

SALLOC_ATTR_VECCALL_CONST
static inline uintptr_t salloc_align_forward(register const uintptr_t pointer,
                                             register const size_t    align);

/**
 * Function Definitions
 */

SALLOC_ATTR_VECCALL_CONST static inline salloc_vec_t
    salloc_init(register const void * const restrict buff,
                register const size_t                buff_length) {
  const salloc_allocator_t allocator SALLOC_ATTR_MAYBE_UNUSED;

  return (typeof(allocator.vector)){(typeof(allocator.vector[0]))buff,
                                    (typeof(allocator.vector[0]))buff_length};
}

SALLOC_ATTR_VECCALL_OVERLOAD static inline void *
    salloc(register salloc_vec_t * restrict allocator, register const size_t size) {
  return salloc(allocator, size, SALLOC_DEFAULT_ALIGNMENT);
}

SALLOC_ATTR_VECCALL_OVERLOAD static inline void *
    salloc(register salloc_vec_t * const restrict allocator,
           register const size_t                  size,
           register const size_t                  align) {
  const salloc_vec_t in_allocator = *allocator;
  const typeof(in_allocator[0]) current_pointer =
      in_allocator[SALLOC_BUFFER] + in_allocator[SALLOC_CURRENT_OFFSET];
  const uintptr_t relative_offset =
      salloc_align_forward(current_pointer, align) - in_allocator[SALLOC_BUFFER];
  const uintptr_t current_offset = (typeof(size))relative_offset + size;

  if (current_offset <= (typeof(current_offset))in_allocator[SALLOC_BUFFER_LENGTH]) {
    const typeof(in_allocator)
        out_allocator = {in_allocator[SALLOC_BUFFER],
                         in_allocator[SALLOC_BUFFER_LENGTH],
                         (typeof(in_allocator[SALLOC_PREVIOUS_OFFSET]))relative_offset,
                         (typeof(in_allocator[SALLOC_CURRENT_OFFSET]))current_offset};

    *allocator = out_allocator;

    return __extension__((void *)in_allocator[SALLOC_BUFFER] + relative_offset);
  } else {
    return nullptr;
  }
}

SALLOC_ATTR_FLATTEN_VECCALL_OVERLOAD static inline void *
    srealloc(register salloc_vec_t * const restrict allocator,
             register void * const restrict         old_memory,
             register const size_t                  old_size,
             register const size_t                  new_size) {
  return srealloc(allocator, old_memory, old_size, new_size, SALLOC_DEFAULT_ALIGNMENT);
}

SALLOC_ATTR_FLATTEN_VECCALL_OVERLOAD static inline void *
    srealloc(register salloc_vec_t * const restrict allocator,
             register void * const restrict         old_memory,
             register const size_t                  old_size,
             register const size_t                  new_size,
             register const size_t                  align) {
  const salloc_vec_t in_allocator = *allocator;

  if ((old_memory == nullptr) || (old_size == 0)) {
    return __extension__ salloc(allocator, new_size, align);
  } else if ((in_allocator[SALLOC_BUFFER] <= (typeof(in_allocator[0]))old_memory) &&
             ((typeof(in_allocator[0]))old_memory <
              (in_allocator[SALLOC_BUFFER] + in_allocator[SALLOC_BUFFER_LENGTH]))) {
    if ((in_allocator[SALLOC_BUFFER] + in_allocator[SALLOC_PREVIOUS_OFFSET]) ==
        (typeof(in_allocator[0]))old_memory) {
      return old_memory;
    } else {
      return salloc(allocator, new_size, align);
    }
  } else {
    return nullptr;
  }
}

SALLOC_ATTR_VECCALL_OVERLOAD static inline void
    sfree(register salloc_vec_t * const restrict allocator) {
  const salloc_vec_t in_allocator        = *allocator;
  const typeof(*allocator) out_allocator = {
      in_allocator[SALLOC_BUFFER],
      in_allocator[SALLOC_BUFFER_LENGTH],
      0,
      0 /* reset all offsets */
  };

  *allocator = out_allocator;
}

SALLOC_ATTR_VECCALL_CONST static inline uintptr_t
    salloc_align_forward(register const uintptr_t pointer, register const size_t align) {
  register const uintptr_t modulo = pointer % (typeof(pointer))align;

  if (modulo != 0) {
    return (pointer + (typeof(pointer))align - modulo);
  } else {
    return (pointer);
  }
}

#endif /* __SALLOC_H__ */
