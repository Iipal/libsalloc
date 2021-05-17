#ifndef __SALLOC_H__
#define __SALLOC_H__

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgcc-compat"

#ifdef SALLOC_DEBUG
/* deprecated in release-version. used only by salloc_trace in debug-purposes */
#  warning "use of SALLOC_DEBUG is not allowed for non-debug purposes. please remove it."
#  include <stdio.h>
#endif

/**
 * ---------------------
 * ATTRIBUTES DEFINITION
 * ---------------------
 */

#define __is_salloc_attrs_defined__ 1

#if __has_extension(cxx_attributes) || __has_feature(cxx_attributes) || \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L)
#  define __is_cpp_attr__ 1
#else
#  define __is_cpp_attr__ 0
#endif

#if __has_attribute(diagnose_if)
#  define __sattr_diagnose_if(x, msg, type) __attribute__((diagnose_if(x, msg, type)))
#else
#  define __sattr_diagnose_if(x, msg, type)
#endif

#define __sattr_diagnose_align(x, align) \
  __sattr_diagnose_if(x % align || x < align, \
                      #x " less than or is not aligned by `" #align "`", \
                      "error")

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

/**
 * ----------------
 * TYPES DEFINITION
 * ----------------
 */

#ifndef NULL
#  define __is_salloc_null_defined__ 1

#  define NULL ((void *)0)
#endif

#ifdef SALLOC_NULLABILITY
#  define __s_nonnull  _Nonnull
#  define __s_nullable _Nullable
#else
#  define __s_nonnull
#  define __s_nullable
#endif

#if defined(__x86_64__) || defined(__aarch64__) || defined(__ia64__)
#  define __S_WORDSIZE 64
#else
#  define __S_WORDSIZE 32
#endif

#if __S_WORDSIZE == 64
typedef unsigned long int __s_uintptr_t; /* salloc analog of uintptr_t; x64 */
typedef long int          __s_ssize_t;   /* salloc analog of ssize_t; x64   */
#else
typedef unsigned int __s_uintptr_t; /* salloc analog of uintptr_t; x32 */
typedef int          __s_ssize_t;   /* salloc analog of ssize_t; x32   */
#endif

typedef __SIZE_TYPE__                      __s_size_t;  /* salloc analog of size_t  */
typedef unsigned char                      __s_uint8_t; /* salloc analog of uint8_t */
typedef __s_uint8_t * restrict __s_nonnull __s_ptr_t;   /* most common pointer type */
typedef __s_uint8_t * const restrict __s_nonnull __s_cptr_t; /* __s_ptr_t but const */

/**
 * Memory mapping with bidirectional implicit free list or Boundary Tags.
 *
 * Example:
 * By calling salloc with \c size of 48 bytes it actually allocating \c size +
 * \c (sizeof(__s_tag_t)*2) bytes for memory mapping:
 *
 * [ __s_tag_t with ->size == 48 and by default ->busy == 1] **header**
 * [                 your 48 bytes of memory                 ] **payload**
 * [ __s_tag_t with ->size == 48 and by default ->busy == 1] **footer**
 */
typedef struct __s_salloc_tag {
#if __S_WORDSIZE == 64
#  define __S_TAG_ALIGN_BITS 3
#  define __S_TAG_SIZE_BITS  60
#  define __S_TAG_BUSY_BITS  1
#else
#  define __S_TAG_ALIGN_BITS 2
#  define __S_TAG_SIZE_BITS  29
#  define __S_TAG_BUSY_BITS  1
#endif

  __s_uint8_t __alignment : __S_TAG_ALIGN_BITS __sattr_munused; /* as it is */

  __s_uintptr_t size : __S_TAG_SIZE_BITS; /* size of current pointer */
  __s_uint8_t   busy : __S_TAG_BUSY_BITS; /* is current pointer was freed or not */
} __sattr_packed __s_tag_t;

/**
 * ------------------------
 * PUBLIC TYPES DEFINITIONS
 * ------------------------
 */

/**
 * Object to which mapped the static buffer via \c salloc_new, and which tracking all the
 * available memory to use in current static buffer.
 */
typedef struct s_salloc_t {
  __s_ptr_t start;  /* start  of available space in static buffer */
  __s_ptr_t end;    /* end    of available space in static buffer */
  __s_ptr_t cursor; /* current max of use memory in static buffer */
} salloc_t;

/**
 * -------------------------
 * PUBLIC MACROS DEFINITIONS
 * -------------------------
 */

/**
 * \brief Minium allocation size in static buffer because it's also default alignment.
 */
#define SALLOC_MIN_ALLOC_SIZE (sizeof(void *) * 2)

/**
 * \brief Minimum required memory in static buffer for at least 1 pointer with at least
 * \c SALLOC_MIN_ALLOC_SIZE bytes size.
 */
#define SALLOC_MIN_BUFFER_SIZE ((sizeof(__s_tag_t) * 2) + SALLOC_MIN_ALLOC_SIZE)

/**
 * Fast shorthand for creating a buffer and \c salloc_t object for s-allocators.
 */
#define salloc_new_fast(name, capacity) \
  static __s_uint8_t name##_buff[(capacity)]; \
  const __s_size_t   name##_buff_capacity = (capacity); \
  salloc_t           name##_slc           = salloc_new(name##_buff, name##_buff_capacity);

/**
 * ------------------
 * MACROS DEFINITIONS
 * ------------------
 */

/**
 * Most convenient type conversions
 */
#ifndef __is_salloc_casts_defined__
#  define __is_salloc_casts_defined__ 1

/** __s2c prefix stands as shortcut for __salloc_to_cast **/

#  define __s2c_uiptr(x) ((__s_uintptr_t)(x))
#  define __s2c_ptr(x)   ((__s_ptr_t)(x))
#  define __s2c_tag(x)   ((__s_tag_t *)(void *)(x))

#endif

/**
 * For work with memory boundary tags or as with bi-directional implicit free-list
 */
#ifndef __is_salloc_tags_defined__
#  define __is_salloc_tags_defined__ 1

/** __st prefix stands as shortcut for __salloc_tag **/

#  define __st_init(size, busy) \
    { 0 /* __alignment */, (size), (busy) }

#  define __st_align_default __s2c_uiptr(SALLOC_MIN_ALLOC_SIZE)
#  define __st_align_bits    (__st_align_default - 1)
#  define __st_align_size(x) \
    (((x) % __st_align_default) ? ((x) + ((~(x)&__st_align_bits) + 1)) : (x))

#  define __st_size    __s2c_uiptr(sizeof(__s_tag_t))
#  define __st_bd_size (__st_size * 2) // bd stands for bi-directional

#  define __st_ptr_get_tag(x) __s2c_tag(__s2c_ptr(x) - __st_size)
#  define __st_tag_get_ptr(x) (__s2c_ptr(x) + __st_size)

#  define __st_busy     1
#  define __st_not_busy 0

#  define __st_get_busy(x) (__s2c_tag(x)->busy)
#  define __st_get_size(x) (__s2c_tag(x)->size)
#  define __st_is_free(x)  !__st_get_busy(x)

#endif

/**
 * --------------------
 * FUNCTIONS PROTOTYPES
 * --------------------
 */

/**
 * \brief Creating a new static buffer to use by \c salloc -allocators with at most
 * \c capacity bytes.
 *
 * \param buff a pointer to static buffer.
 * \param capacity a \c buff length \ size \ capacity.
 *
 * \return new \c salloc_t object to work with \c salloc -allocators.
 */
__sattr_veccall_const static inline salloc_t
    salloc_new(register const void * const restrict __s_nonnull buff,
               register const __s_size_t                        capacity)
        __sattr_diagnose_align(capacity, SALLOC_MIN_BUFFER_SIZE);

/**
 * \brief Deleting created by \c salloc_new object.
 *
 * \note This is not actually deleting the object. All pointers are still valid and all
 * the data under them will be kept as well after calling the \c salloc_delete . It's only
 * removing the markup of memory chunks used by \c salloc -allocators. After this
 * call your static \c buff what you set with \c salloc_new will now have additionally
 * free 16 bytes at both sides (before and after the pointer) of each allocated memory
 * pointers with \c salloc .
 *
 * \warning If you call \c salloc with the same object you passed to
 * \c salloc_delete - new memory can possibly override data under the previously used
 * pointers by data with \c salloc memory markup.
 *
 * \param __s a pointer to \c salloc_t object.
 */
__sattr_flatten_veccall_overload static inline void
    salloc_delete(register salloc_t * const restrict __s_nonnull __s);

#ifdef SALLOC_DEBUG
/**
 * \brief Just prints \ trace all the pointers stored in \c salloc_t object.
 *
 * \deprecated This function using libc \c printf , and incudes \c stdio.h .
 *
 * \param __s a pointer to \c salloc_t object.
 */
__sattr_veccall_overload static inline void
    salloc_trace(register salloc_t * const restrict __s_nonnull __s);
#endif

/**
 * \brief Returns size\capacity of static buffer.
 *
 * \param __s a pointer to \c salloc_t object.
 *
 * \return capacity of static buffer mapped in \c __s .
 */
__sattr_flatten_veccall_overload static inline __s_size_t
    salloc_capacity(register salloc_t * const restrict __s_nonnull __s);

/**
 * \brief Returns size of used memory in static buffer.
 *
 * \param __s a pointer to \c salloc_t object.
 *
 * \return 0 - no used memory. Otherwise - size of used memory.
 */
__sattr_flatten_veccall_overload static inline __s_size_t
    salloc_used(register salloc_t * const restrict __s_nonnull __s);

/**
 * \brief Returns size of unused memory in static buffer.
 *
 * \param __s a pointer to \c salloc_t object.
 *
 * \return 0 - no unused memory. Positive - size of available\unused memory. Negative -
 * somehow in static buffer are more allocated memory than buffer can fit.
 */
__sattr_flatten_veccall_overload static inline __s_ssize_t
    salloc_unused(register salloc_t * const restrict __s_nonnull __s);
/**
 * \brief Checks is in \c __s is enough space to allocate new pointer with at least
 * \c __size bytes.
 *
 * \param __s a pointer to \c salloc_t object.
 * \param __size required size of s-allocation.
 *
 * \return 0 - if new s-allocation will take all the unused memory. Negative - the size of
 * how much the pointer with at least \c __size will exceed the unused memory. Positive -
 * available space after s-allocation of the new pointer with at least \c __size bytes.
 */
__sattr_flatten_veccall_overload static inline __s_ssize_t
    salloc_unused(register salloc_t * const restrict __s_nonnull __s,
                  register const __s_size_t                      __size)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);
/**
 * \brief Checks is in \c __s is enough space to allocate of \c __nmemb new pointers with
 * at least \c __size bytes each.
 *
 * \param __s a pointer to \c salloc_t object.
 * \param __size required size of s-allocation for each pointer.
 * \param __nmemb N-pointers to check for s-allocation.
 *
 * \return 0 - if new \c __nmemb s-allocations with at least \c __size each will take all
 * the unused memory. Negative - the size of how much new \c __nmemb pointers with at
 * least \c __size each will exceed the unused memory. Positive - available space after
 * s-allocation of \c __nmemb new pointers with at least \c __size bytes each.
 */
__sattr_flatten_veccall_overload static inline __s_ssize_t
    salloc_unused(register salloc_t * const restrict __s_nonnull __s,
                  register const __s_size_t                      __size,
                  register const __s_size_t                      __nmemb)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \brief Allocates new static pointer in \c __s with at least \c __size bytes, and
 * returns it.
 *
 * \param __s a pointer to \c salloc_t object.
 * \param __size size in bytes.
 *
 * \return A valid pointer with at least \c __size bytes.
 *         Or \c NULL if:
 *           - static buffer in \c __s has no available memory;
 *           - \c __size is equals to 0;
 *           - \c __s is NULL;
 */
__sattr_veccall_overload static inline void * __s_nullable salloc(
    register salloc_t * const restrict __s_nonnull __s, register const __s_size_t __size)
    __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \brief Allocates new static pointer in \c __s for an array of \c __nmemb elements
 *        of \c __size bytes each, and returns it.
 *
 * \param __s a pointer to \c salloc_t object.
 * \param __size size of each element in bytes.
 * \param __nmemb N-elements.
 *
 * \return A valid pointer with at least ( \c __nmemb * \c __size ) bytes.
 *         Or \c NULL if:
 *           - static buffer in \c __s has no available memory;
 *           - ( \c __nmemb * \c __size ) is equals to 0;
 *           - \c __s is NULL;
 */
__sattr_flatten_veccall_overload static inline void * __s_nullable
    salloc(register salloc_t * const restrict __s_nonnull __s,
           register const __s_size_t                      __size,
           register const __s_size_t                      __nmemb)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \brief Free a \c __ptr.
 *
 * \note This function marking up space that was allocated for \c __ptr as free,
 * and will try to optimize a static buffer in \c __s.
 *
 * \warning A \c __ptr will be a valid pointer after this call of \c sfree , but data
 * under it can possibly be overwritten with the next calls of \c salloc -allocators.
 *
 * \param __s a pointer to \c salloc_t object.
 * \param __ptr a pointer to be freed.
 */
__sattr_veccall_overload static inline void
    sfree(register salloc_t * const restrict __s_nonnull __s,
          register void * restrict __s_nonnull           __ptr);

/**
 * \brief Un-safe free of \c __ptr.
 *
 * \note This function only marking up space that was allocated for \c __ptr as free.
 *
 * \warning This function will not optimize a static buffer that you set with \c
 * salloc_new at all. Use it on your risk or use \c sfree with \c salloc_t as the first
 * parameter. A \c __ptr will be a valid pointer after this call of \c sfree , but data
 * under it can possibly be overwritten with the next calls of \c salloc -allocators.
 *
 * \param __ptr a pointer to be freed.
 */
__sattr_flatten_veccall_overload static inline void
    sfree(register void * restrict __s_nonnull __ptr);

/**
 * --------------------
 * FUNCTIONS DEFINITION
 * --------------------
 */

/**
 * ||||||||||||||||||||||
 * SALLOC_NEW DEFINITIONS
 * ||||||||||||||||||||||
 */

__sattr_veccall_const static inline salloc_t
    salloc_new(register const void * const restrict __s_nonnull buff,
               register const __s_size_t                        capacity) {
  __s_ptr_t buff_end = (__s2c_ptr(buff) + (__s_uintptr_t)capacity);
  salloc_t  out      = (salloc_t){__s2c_ptr(buff), buff_end, __s2c_ptr(buff)};

  return out;
}

/**
 * |||||||||||||||||||||||||
 * SALLOC_DELETE DEFINITIONS
 * |||||||||||||||||||||||||
 */

__sattr_flatten_veccall_overload static inline void
    salloc_delete(register salloc_t * const restrict __s_nonnull __s) {
  __s->cursor = __s->start;
}

#ifdef SALLOC_DEBUG
/**
 * ||||||||||||||||||||||||
 * SALLOC_TRACE DEFINITIONS
 * ||||||||||||||||||||||||
 */

__sattr_veccall_overload static inline void
    salloc_trace(register salloc_t * const restrict __s_nonnull __s) {
  __s_ptr_t iptr   = __s->start;
  __s_ptr_t cursor = __s->cursor;

  printf("%s:\n", __FUNCTION__);

  __s_size_t blocks_count = 0;
  while (iptr < cursor) {
    __s_tag_t * iptr_header = __s2c_tag(iptr);
    __s_tag_t * iptr_footer = __s2c_tag(iptr + iptr_header->size + __st_size);

    printf("%d %-4zu [%p ... %p] %d %zu\n",
           iptr_header->busy,
           iptr_header->size,
           iptr_header,
           iptr_footer,
           iptr_footer->busy,
           iptr_footer->size);

    iptr = __s2c_ptr(iptr_footer) + __st_size;
    ++blocks_count;
  }

  const __s_ssize_t unused   = salloc_unused(__s);
  const __s_size_t  used     = salloc_used(__s);
  const __s_size_t  capacity = salloc_capacity(__s);

  if (used) {
    printf("       |\n");
  }

  printf("  used [%p ... %p] %zu (%zu)\n",
         __s->start,
         cursor,
         used - (blocks_count * __st_bd_size),
         used);
  printf("unused [%p ... %p] %zu\n"
         "capacity: %zu;\n",
         cursor,
         __s->end,
         unused,
         capacity);
}
#endif

/**
 * |||||||||||||||||||||||||||
 * SALLOC_CAPACITY DEFINITIONS
 * |||||||||||||||||||||||||||
 */

__sattr_flatten_veccall_overload static inline __s_size_t
    salloc_capacity(register salloc_t * const restrict __s_nonnull __s) {
  const __s_size_t capacity = __s->end - __s->start;

  return capacity;
}

/**
 * |||||||||||||||||||||||
 * SALLOC_USED DEFINITIONS
 * |||||||||||||||||||||||
 */

__sattr_flatten_veccall_overload static inline __s_size_t
    salloc_used(register salloc_t * const restrict __s_nonnull __s) {
  const __s_size_t used = __s->cursor - __s->start;

  return used;
}

/**
 * |||||||||||||||||||||||||
 * SALLOC_UNUSED DEFINITIONS
 * |||||||||||||||||||||||||
 */

__sattr_flatten_veccall_overload static inline __s_ssize_t
    salloc_unused(register salloc_t * const restrict __s_nonnull __s) {
  const __s_ssize_t unused = __s->end - __s->cursor;

  return unused;
}
__sattr_flatten_veccall_overload static inline __s_ssize_t
    salloc_unused(register salloc_t * const restrict __s_nonnull __s,
                  register const __s_size_t                      __size) {
  const __s_size_t  aligned = __st_align_size(__size);
  const __s_ssize_t unused  = salloc_unused(__s);
  const __s_ssize_t out     = unused - aligned - __st_bd_size;

  return out;
}
__sattr_flatten_veccall_overload static inline __s_ssize_t
    salloc_unused(register salloc_t * const restrict __s_nonnull __s,
                  register const __s_size_t                      __size,
                  register const __s_size_t                      __nmemb) {
  const __s_size_t  aligned      = __st_align_size(__size);
  const __s_size_t  require_size = __nmemb * aligned + __nmemb * __st_bd_size;
  const __s_ssize_t unused       = salloc_unused(__s);
  const __s_ssize_t out          = unused - require_size;

  return out;
}

/**
 * ||||||||||||||||||
 * SALLOC DEFINITIONS
 * ||||||||||||||||||
 */

/**
 * Extending the used memory cursor if it's possible and if in \c __s is no best-fit
 * memory chunk for new s-allocation.
 */
__sattr_flatten_veccall static inline void * __s_nullable
    __salloc_extend_cursor(register salloc_t * const restrict __s_nonnull __s,
                           register const __s_uintptr_t                   size) {
  register const __s_uintptr_t alloc_size = size + __st_bd_size;

  if ((__s->cursor + alloc_size) <= __s->end) {
    register __s_ptr_t out = __s->cursor;

    register __s_tag_t *     header  = __s2c_tag(out);
    register __s_tag_t *     footer  = __s2c_tag(out + size + __st_size);
    register const __s_tag_t payload = __st_init(size, __st_busy);

    *header = payload;
    *footer = payload;

    register __s_ptr_t new_cursor = out + alloc_size;
    __s->cursor                   = new_cursor;

    return __st_tag_get_ptr(out);
  } else {
    return NULL;
  }
}
/**
 * Fragmentation memory space if best-fit memory chunk was found.
 */
__sattr_flatten_veccall static inline void
    __salloc_frag_free_space(const __s_cptr_t __s_nonnull __bestptr,
                             const __s_uintptr_t          __bestsize,
                             const __s_uintptr_t          __require_size) {
  register __s_tag_t * restrict __fragptr_header;
  register __s_tag_t * restrict __fragptr_footer;
  register __s_tag_t __fragptr_payload;

  if ((__bestsize - __st_bd_size) > __require_size) {
    const __s_uintptr_t __fragsize = __bestsize - __st_bd_size - __require_size;
    __s_uint8_t *       __fragptr  = __bestptr + __require_size + __st_bd_size;

    __fragptr_header  = __s2c_tag(__fragptr);
    __fragptr_footer  = __s2c_tag(__fragptr + __fragsize + __st_size);
    __fragptr_payload = (__s_tag_t)__st_init(__fragsize, __st_not_busy);
  } else {
    /**
     * may seem useless at first sight, because for the \c __bestptr I already set it's
     * size in \c __salloc_find_best_chunk, but there I actually set only
     * \c __require_size , but \c __bestsize can be grater than that value only for a few
     * bytes, and less then \c (__bestsize-__st_bd_size)
     */
    __fragptr_header  = __s2c_tag(__bestptr);
    __fragptr_footer  = __s2c_tag(__bestptr + __bestsize + __st_size);
    __fragptr_payload = (__s_tag_t)__st_init(__bestsize, __st_busy);
  }

  *__fragptr_header = __fragptr_payload;
  *__fragptr_footer = __fragptr_payload;
}
__sattr_flatten_veccall static inline void * __s_nullable
    __salloc_find_best_chunk(register salloc_t * const restrict __s_nonnull __s,
                             register const __s_size_t __require_size) {
  const __s_cptr_t __s_nonnull __s_cursor = __s->cursor;

  __s_ptr_t     __iptr     = __s->start;
  __s_ptr_t     __bestptr  = 0;
  __s_uintptr_t __isize    = 0;
  __s_uintptr_t __bestsize = salloc_capacity(__s);

  while (__iptr < __s_cursor) {
    const __s_uint8_t is_free = __st_is_free(__iptr);

    __isize = __st_get_size(__iptr);
    if (!!is_free && __isize >= __require_size && __isize <= __bestsize) {
      __bestsize = __isize;
      __bestptr  = __iptr;
    }
    __iptr += __isize + __st_bd_size;
  }

  if (!!__bestptr) {
    __s_tag_t *     __bestptr_header  = __s2c_tag(__bestptr);
    __s_tag_t *     __bestptr_footer  = __s2c_tag(__bestptr + __require_size + __st_size);
    const __s_tag_t __bestptr_payload = __st_init(__require_size, __st_busy);

    *__bestptr_header = __bestptr_payload;
    *__bestptr_footer = __bestptr_payload;

    if (__bestsize > __require_size) {
      __salloc_frag_free_space(__bestptr, __bestsize, __require_size);
    }

    return __st_tag_get_ptr(__bestptr);
  }

  return NULL;
}
__sattr_veccall_overload static inline void * __s_nullable
    salloc(register salloc_t * const restrict __s_nonnull __s,
           register const __s_size_t                      __size) {
  register const __s_uintptr_t aligned_size = __st_align_size(__size);
  register void * restrict out              = NULL;

  if (!aligned_size) {
    return NULL;
  }

  if (__s->start < __s->cursor) {
    out = __salloc_find_best_chunk(__s, aligned_size);
  }
  if (!out) {
    out = __salloc_extend_cursor(__s, aligned_size);
  }

  return out;
}
__sattr_flatten_veccall_overload static inline void * __s_nullable
    salloc(register salloc_t * const restrict __s_nonnull __s,
           register const __s_size_t                      __size,
           register const __s_size_t                      __nmemb) {
  const __s_size_t __arr_size = __nmemb * __size;
  return salloc(__s, __arr_size);
}

/**
 * |||||||||||||||||
 * SFREE DEFINITIONS
 * |||||||||||||||||
 */

/**
 * Base functional of memory space fragmentation on sfree call.
 */
__sattr_flatten_veccall static inline void __sfree_frag(register __s_ptr_t  __iptr,
                                                        register __s_cptr_t __baseptr) {
  const __s_uintptr_t __base_size = __st_get_size(__baseptr);
  const __s_uintptr_t __iptr_size = __st_get_size(__iptr);
  const __s_uintptr_t __fragsize  = __base_size + __st_bd_size + __iptr_size;

  __s_tag_t *     __iptr_header  = __s2c_tag(__iptr);
  __s_tag_t *     __iptr_footer  = __s2c_tag(__iptr + __fragsize + __st_size);
  const __s_tag_t __iptr_payload = __st_init(__fragsize, __st_not_busy);

  *__iptr_header = __iptr_payload;
  *__iptr_footer = __iptr_payload;
}
__sattr_flatten_veccall static inline __s_uint8_t
    __sfree_shrink_cursor(register salloc_t * const restrict __s_nonnull __s,
                          register __s_cptr_t __s_nonnull                __ptr) {
  __s_uint8_t is_moved = 0;

  if (__s->cursor <= (__ptr + __st_get_size(__ptr) + __st_bd_size)) {
    __s->cursor = __ptr;
    is_moved    = 1;
  }

  return is_moved;
}
__sattr_veccall_overload static inline void
    sfree(register salloc_t * const restrict __s_nonnull __s,
          register void * restrict __s_nonnull           __ptr) {
  sfree(__ptr);

  const __s_cptr_t __s_start  = __s->start;
  const __s_cptr_t __s_cursor = __s->cursor;

  __s_tag_t * __ptr_tag = __st_ptr_get_tag(__ptr);
  __s_ptr_t   __iptr    = __s2c_ptr(__ptr_tag);

  /**
   * Left-side memory fragmentation.
   * Before:
   * [busy] [while_free_chunks] [free_chunk1] [__ptr chunk] [right_side_mem]
   * After:
   * [busy] [   new_big_damn_ass_fragmented_memory_chunk  ] [right_side_mem]
   */
  if (__iptr > __s_start) {
    __s_ptr_t __baseptr = __iptr;
    __iptr              = __baseptr - __st_bd_size - __st_get_size(__baseptr - __st_size);

    while (__iptr >= __s_start && __st_is_free(__iptr)) {
      __sfree_frag(__iptr, __baseptr);

      __s_ptr_t __prev_ptr = __iptr - __st_size;
      if (__s_start <= __prev_ptr) {
        __s_tag_t * __prev_tag = __s2c_tag(__prev_ptr);
        __prev_ptr             = __prev_ptr - __prev_tag->size - __st_size;
      }

      __baseptr = __iptr;
      __iptr    = __prev_ptr;
    }

    __iptr = __baseptr;
  }

  if (__sfree_shrink_cursor(__s, __iptr)) {
    /**
     * If cursors was moved after left-side fragmentation than means right-side has no
     * available memory at all
     */
    return;
  }

  /**
   * Right-side memory fragmentation.
   * Before:
   * [left_side_mem] [ left_side_fragmented_chunk ] [free_chunk1] [busy]
   * After:
   * [left_side_mem] [ new_big_damn_ass_fragmented_memory_chunk ] [busy]
   */
  {
    __s_ptr_t __baseptr = __iptr;
    __iptr              = __baseptr + __st_get_size(__baseptr) + __st_bd_size;

    while (__iptr < __s_cursor && __st_is_free(__iptr)) {
      __sfree_frag(__baseptr, __iptr);
      __iptr += __st_get_size(__iptr) + __st_bd_size;
    }

    __sfree_shrink_cursor(__s, __baseptr);
  }
}

__sattr_flatten_veccall_overload static inline void
    sfree(register void * restrict __s_nonnull __ptr) {
  __s_tag_t *         header   = __st_ptr_get_tag(__ptr);
  const __s_uintptr_t ptr_size = header->size;
  __s_tag_t *         footer   = __s2c_tag(__ptr + ptr_size);
  const __s_tag_t     payload  = __st_init(ptr_size, __st_not_busy);

  *header = payload;
  *footer = payload;
}

/**
 * -----------------------------
 * SALLOC_GDI_BUFFER DEFINITIONS
 * -----------------------------
 */

#ifdef SALLOC_GDI_BUFFER
#  undef SALLOC_GDI_BUFFER

/**
 * GDI - Global Data Interace
 * In this case it's global interface to access global static buffer without manually
 * crating \c salloc_t object. Size of this buffer can be specified with
 * \c SALLOC_GDI_BUFFER_SIZE .
 */
#  define SALLOC_GDI_BUFFER 1

#  ifndef SALLOC_GDI_BUFFER_SIZE

/**
 * Size of global static buffer
 */
#    define SALLOC_GDI_BUFFER_SIZE 4096
#  endif

/**
 * Below just an interfaces\accessors to all the standard s-allocators via gdi buffer,
 * so there is no additional prototypes or any other comments.
 */

__sattr_veccall_const static inline salloc_t * __s_nonnull __salloc_get_gdi_buffer(void) {
  static __s_uint8_t __buffer[SALLOC_GDI_BUFFER_SIZE];
  static salloc_t    __slc = {__buffer, __buffer + SALLOC_GDI_BUFFER_SIZE, __buffer};

  return &__slc;
}

__sattr_flatten_veccall_overload static inline void salloc_delete(void) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();

  salloc_delete(__gdi_slc);
}

#  ifdef SALLOC_DEBUG
__sattr_flatten_veccall_overload static inline void salloc_trace(void) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  salloc_trace(__gdi_slc);
}
#  endif

__sattr_flatten_veccall_overload static inline __s_size_t salloc_capacity(void) {
  return SALLOC_GDI_BUFFER_SIZE;
}

__sattr_flatten_veccall_overload static inline __s_size_t salloc_used(void) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  __s_size_t used               = salloc_used(__gdi_slc);

  return used;
}

__sattr_flatten_veccall_overload static inline __s_ssize_t salloc_unused(void) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  __s_ssize_t unused            = salloc_unused(__gdi_slc);

  return unused;
}

__sattr_flatten_veccall_overload static inline __s_ssize_t
    salloc_unused(register const __s_size_t __size)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  __s_ssize_t unused            = salloc_unused(__gdi_slc, __size);

  return unused;
}

__sattr_flatten_veccall_overload static inline __s_ssize_t
    salloc_unused(register const __s_size_t __size, register const __s_size_t __nmemb)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  __s_ssize_t unused            = salloc_unused(__gdi_slc, __size, __nmemb);

  return unused;
}

__sattr_flatten_veccall_overload static inline void * __s_nullable
    salloc(register const __s_size_t __size)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  void * __ptr                  = salloc(__gdi_slc, __size);

  return __ptr;
}
__sattr_flatten_veccall_overload static inline void * __s_nullable
    salloc(register const __s_size_t __size, register const __s_size_t __nmemb)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  void * __ptr                  = salloc(__gdi_slc, __size, __nmemb);

  return __ptr;
}

__sattr_flatten_veccall_overload static inline void
    sfree_gdi(register void * restrict const __gdi_ptr) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();

  sfree(__gdi_slc, __gdi_ptr);
}

#endif

/**
 * ---------------
 * MACROS UNDEFINE
 * ---------------
 */

#if defined(SALLOC_MACROS_AFTER_USE)
#  warning "salloc macroses still defined."
#endif
#if defined(SALLOC_ATTRS_AFTER_USE)
#  warning "salloc attributes still defined."
#endif

#if defined(SALLOC_MACROS_AFTER_USE)
#  undef __is_salloc_casts_defined__
#  undef __is_salloc_tags_defined__
#  undef __is_salloc_null_defined__
#  undef __is_salloc_bool_defined__
#endif

#if defined(SALLOC_ATTRS_AFTER_USE)
#  undef __is_salloc_attrs_defined__
#endif

#if __is_salloc_attrs_defined__
#  undef __is_salloc_attrs_defined__
#  undef __is_cpp_attr__
#  undef __sattr_diagnose_if
#  undef __sattr_diagnose_align
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

#ifdef __is_salloc_null_defined__
#  undef __is_salloc_null_defined__
#  undef NULL
#endif

#undef __s_nonnull
#undef __s_nullable

#undef __S_WORDSIZE

#undef __S_TAG_ALIGN_BITS
#undef __S_TAG_SIZE_BITS
#undef __S_TAG_BUSY_BITS

#if __is_salloc_casts_defined__
#  undef __is_salloc_casts_defined__
#  undef __s2c_uiptr
#  undef __s2c_sptr
#  undef __s2c_tag
#endif

#if __is_salloc_tags_defined__
#  undef __is_salloc_tags_defined__
#  undef __st_init
#  undef __st_align_default
#  undef __st_align_bits
#  undef __st_align_size
#  undef __st_size
#  undef __st_bd_size
#  undef __st_ptr_get_tag
#  undef __st_tag_get_ptr
#  undef __st_busy
#  undef __st_not_busy
#  undef __st_get_busy
#  undef __st_get_size
#  undef __st_is_free
#endif

#pragma clang diagnostic pop

#endif /* __SALLOC_H__ */
