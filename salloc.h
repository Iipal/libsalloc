/**
 * \file salloc.h
 *
 * The Static memory ALLOCator : salloc
 *
 * Header-only library with malloc-like behavior for mapping\allocation memory in a static
 * buffer, so all work with memory mostly can be optimized and calculated at compile-time.
 */

#ifndef __SALLOC_H__
#define __SALLOC_H__

/*
 * -------------
 * CONFIGURATION
 * -------------
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#ifdef __clang__
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wgcc-compat"
#else
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wattributes"
#endif

#ifndef __has_attribute
#  define __has_attribute(__attr) 0
#endif
#ifndef __has_feature
#  define __has_feature(__attr) 0
#endif
#ifndef __has_extension
#  define __has_extension(__attr) __has_feature(__attr)
#endif

#ifndef SALLOC_DEFAULT_ALIGNMENT
/**
 * \b Specify the default size alignment of each pointer allocation.
 *
 * \note Default align for x64 systems is 16 bytes (double size of any pointer), so
 * allocation of 12 bytes will take 16 bytes instead. 20 -> 32, 8 -> 16 and so on.
 *
 * \attention If you specify value less than 8 bytes it will be overwrited to align at
 * least of it size.
 */
#  define SALLOC_DEFAULT_ALIGNMENT (sizeof(void *) * 2)
#endif

#ifdef SALLOC_DEBUG
/**
 * \b Enables #salloc_trace function with #SALLOC_AFTERUSE_INTERNAL_MACROS , and
 * #SALLOC_AFTERUSE_INTERNAL_ATTRS and includes \c stdio.h .
 */
#  define __sis_debug_defined__ 1

#  include <stdio.h>

#  ifndef SALLOC_AFTERUSE_INTERNAL_MACROS
#    define SALLOC_AFTERUSE_INTERNAL_MACROS
#  endif
#  ifndef SALLOC_AFTERUSE_INTERNAL_ATTR
#    define SALLOC_AFTERUSE_INTERNAL_ATTR
#  endif
#endif

#ifdef SALLOC_AFTERUSE_INTERNAL_MACROS
/**
 * \b Use internal-purpose-only macroses outside of salloc.h
 */
#  define __sis_afteruse_macros_defined__ 1
#endif

#ifdef SALLOC_AFTERUSE_INTERNAL_ATTR
/**
 * \b Use internal-purpose-only attributes outside of salloc.h
 */
#  define __sis_afteruse_attrs_defined__ 1
#endif

#ifdef SALLOC_NULLABILITY
/**
 * \b Enables clang nullability checks
 * (https://clang.llvm.org/docs/analyzer/developer-docs/nullability.html) extension. They
 * are can still be accessible if \c SALLOC_AFTERUSE_INTERNAL_MACROS is defined.
 */
#  define __sis_nullability_defined__ 1
#endif

#ifdef SALLOC_GDI_BUFFER
#  if __has_attribute(overloadable)
/**
 * GDI - Global Data Interace
 * In this case it's global interface to access global static buffer without manually
 * creating the \c salloc_t object. Size of this buffer can be specified with
 * \c SALLOC_GDI_BUFFER_SIZE .
 *
 * clang compiler with \c overloadable attribute is required for this feature.
 */
#    define __sis_gdi_buffer_defined__ 1
#  else
#    define __sis_gdi_buffer_defined__ 0
#    warning "salloc GDI feature can't be used without clang attribute 'overloadable'."
#  endif

#  ifndef SALLOC_GDI_BUFFER_SIZE
/**
 * Size of global static buffer
 */
#    define SALLOC_GDI_BUFFER_SIZE 4096
#  endif
#endif

/*
 * ---------------------
 * ATTRIBUTES DEFINITION
 * ---------------------
 */

/**
 * To use for cleaning the attribute macroses or not if #SALLOC_AFTERUSE_INTERNAL_ATTR
 * specified
 */
#define __sis_attrs_defined__ 1

#if __has_attribute(diagnose_if)
#  define __sattr_diagnose_if(x, msg, type) __attribute__((diagnose_if(x, msg, type)))
#else
#  define __sattr_diagnose_if(x, msg, type)
#endif

#define __sattr_diagnose_align(x, align) \
  __sattr_diagnose_if(x % align || x < align, \
                      #x " less than or is not aligned by `" #align "`", \
                      "error")

#if __has_attribute(vectorcall)
#  define __sattr_veccall __attribute__((vectorcall))
#else
#  define __sattr_veccall
#endif

#if __has_attribute(const)
#  define __sattr_const __attribute__((const))
#else
#  define __sattr_const
#endif

#if __has_attribute(overloadable)
#  define __sattr_overload __attribute__((overloadable))
#  define SALLOC_OVERLOADS 1
#else
#  define __sattr_overload
#  define SALLOC_OVERLOADS 0
#  warning "Your compiler doesn't support overloadable attribute."
#endif

#if __has_attribute(flatten)
#  define __sattr_flatten __attribute__((flatten))
#else
#  define __sattr_flatten
#endif

#if __has_attribute(maybe_unused)
#  define __sattr_munused __attribute__((maybe_unused))
#elif __has_attribute(unused)
#  define __sattr_munused __attribute__((unused))
#else
#  define __sattr_munused
#endif

#if __has_attribute(artificial)
#  define __sattr_artificial __attribute__((artificial))
#else
#  define __sattr_artificial
#endif

#ifdef __always_inline
#  define __sattr_inline static __sattr_artificial __always_inline
#else
#  define __sattr_inline static __sattr_artificial inline
#endif

#define __sattr_veccall_const            __sattr_veccall __sattr_const
#define __sattr_veccall_overload         __sattr_veccall __sattr_overload
#define __sattr_veccall_const_overload   __sattr_veccall __sattr_const __sattr_overload
#define __sattr_flatten_veccall_overload __sattr_flatten __sattr_veccall __sattr_overload
#define __sattr_flatten_veccall          __sattr_flatten __sattr_veccall

/*
 * ----------------
 * TYPES DEFINITION
 * ----------------
 */

#ifdef __sis_nullability_defined__
#  define S_Nonnull  _Nonnull
#  define S_Nullable _Nullable
#else
#  define S_Nonnull
#  define S_Nullable
#endif

#if defined(__x86_64__) || defined(__aarch64__) || defined(__ia64__)
#  define __S_WORDSIZE 64
#else
#  define __S_WORDSIZE 32
#endif

#if __S_WORDSIZE == 64
#  define __S_TAG_ALIGN_BITS \
    3 /* if we on 64bit system then default alignment of (sizeof(void*)*2) will be \
         equal to 16 bytes(alwayz: ...1111 0000). This means that 4 last bits are \
         available to use. 1 of them is for busy indicator, and 3 for alignment */
#  define __S_TAG_SIZE_BITS 60 /** value aligned by 16 always took only 60 bits */
#  define __S_TAG_BUSY_BITS 1  /** busy indicator */
#else
#  define __S_TAG_ALIGN_BITS \
    2 /* if we on 32bit system then default alignment of (sizeof(void*)*2) will be \
         equal to 8(alwayz: ...1111 1000), not 16, so this mean that only 3 last \
         bits are available to use. 1 of them is for busy indicator, so 2 for \
         alignment then */
#  define __S_TAG_SIZE_BITS 29 /** value aligned by 8 always took only 29 bits */
#  define __S_TAG_BUSY_BITS 1  /** busy indicator **/
#endif

/**
 * Memory mapping with bidirectional implicit free list or Boundary Tags.
 *
 * Example:
 * By calling #salloc with \c size of 1024 bytes it actually allocating
 * \c size + \c (sizeof(__s_tag_t)*2) bytes for memory mapping:
 *```
 * [ __s_tag_t with ->size == 1024 ] **header**
 * [   start of available to use   ] <- salloc returns this
 * [           1024 bytes          ] **payload**
 * [   of memory in static buffer  ]
 * [ __s_tag_t with ->size == 1024 ] **footer**
 * ```
 */
typedef struct {
  /** size of current pointer */
  uintptr_t size : __S_TAG_SIZE_BITS;
  /** as it is */
  __sattr_munused unsigned char __alignment : __S_TAG_ALIGN_BITS;
  /** is current pointer was freed or not */
  unsigned char busy : __S_TAG_BUSY_BITS;
} __s_tag_t;

/**
 * Object to which mapped the static buffer via #salloc_new , and which tracking all
 * the available memory to use in current static buffer.
 */
typedef struct {
  /** start of available space in static buffer */
  unsigned char * restrict S_Nonnull start;
  /** end of available space in static buffer */
  unsigned char * restrict S_Nonnull end;
  /** current end position of allocated memory in static byffer  */
  unsigned char * restrict S_Nonnull cursor;
} salloc_t;

/*
 * -------------------------
 * PUBLIC MACROS DEFINITIONS
 * -------------------------
 */

/**
 * \brief Size in bytes how much each new allocation will take memory in static
 * buffer for mapping via Boundary Tags.
 */
#define SALLOC_EACH_ALLOC_OVERHEAD (sizeof(__s_tag_t) * 2)

/**
 * \brief Minium allocation size in static buffer.
 */
#define SALLOC_MIN_ALLOC_SIZE SALLOC_DEFAULT_ALIGNMENT

/**
 * \brief Minimum required memory in static buffer for at least 1 pointer with at
 * least \c SALLOC_MIN_ALLOC_SIZE bytes size.
 */
#define SALLOC_MIN_BUFFER_SIZE (SALLOC_EACH_ALLOC_OVERHEAD + SALLOC_MIN_ALLOC_SIZE)

/**
 * Fast shorthand for creating a buffer and \c salloc_t object for s-allocators.
 *
 * Be aware that this macro implicitly creates 3 variables:
 * static unsigned char *name*_buff[capacity];
 * const size_t *name*_buff_capacity = (capacity);
 * salloc_t *name*_slc = salloc_new(*name*_buff, *name*_buff_capacity);
 */
#define salloc_new_fast(name, capacity) \
  static unsigned char __sattr_munused name##_buff[(capacity)]; \
  const size_t __sattr_munused         name##_buff_capacity = (capacity); \
  salloc_t name##_slc = salloc_new(name##_buff, name##_buff_capacity);

/*
 * ------------------
 * MACROS DEFINITIONS
 * ------------------
 */

#ifndef __sis_macroses_defined__
#  define __sis_macroses_defined__ 1

#  ifndef NULL
#    define __sis_null_defined__ 1

#    define NULL ((void *)0)
#  endif

/*
 * Most convenient type conversions
 *
 * __s2c prefix stands as shortcut for __salloc_to_cast
 */

#  define __s2c_uiptr(x) ((uintptr_t)(x))
#  define __s2c_ptr(x)   ((unsigned char * restrict S_Nonnull)(x))
#  define __s2c_tag(x)   ((__s_tag_t *)(x))

/*
 * For work with memory Boundary Tags or as with bi-directional implicit free-list
 *
 * __st prefix stands as shortcut for __salloc_tag
 */

#  define __st_init(size, busy) \
    { (size), 0 /* __alignment */, (busy) }

#  define __st_align_default __s2c_uiptr(SALLOC_DEFAULT_ALIGNMENT)
#  define __st_align_size(x) \
    (((x) % __st_align_default) ? ((x) + ((~(x) & (__st_align_default - 1)) + 1)) : (x))

#  define __st_size    __s2c_uiptr(sizeof(__s_tag_t))
#  define __st_bd_size SALLOC_EACH_ALLOC_OVERHEAD // bd stands for bi-directional

#  define __st_move    (ptr, size)((ptr) + (size) + (__st_size))
#  define __st_bd_move (ptr, size)((ptr) + (size) + (__st_bd_size))

#  define __st_ptr_get_tag(x) __s2c_tag(__s2c_ptr(x) - __st_size)
#  define __st_tag_get_ptr(x) (__s2c_ptr(x) + __st_size)

#  define __st_busy     1
#  define __st_not_busy 0

#  define __st_get_busy(x) (__s2c_tag(x)->busy)
#  define __st_get_size(x) (__s2c_tag(x)->size)
#  define __st_is_free(x)  !__st_get_busy(x)

#endif

/*
 * --------------------
 * FUNCTIONS PROTOTYPES
 * --------------------
 */

/**
 * \b Creates new S-Alloc buffer.
 *
 * \param buff a pointer to buffer
 * \param capacity a total capacity of given buffer
 *
 * \return new S-Allocated buffer.
 */
__sattr_veccall_const_overload __sattr_inline salloc_t
    salloc_new(register unsigned char * const restrict S_Nonnull buff,
               register const size_t                             capacity)
        __sattr_diagnose_align(capacity, SALLOC_MIN_BUFFER_SIZE);

/**
 * \b Replaces S-Allocated buffer in given \p __s with provided \p buff and \p capacity .
 *
 * \note The previous data\pointers inside the \p __s will not be changed\\free\\purged at
 * all, this method only replaces pointer to the \p buff and it's \p capacity inside the
 * given \p __s .
 *
 * \param buff a pointer to new buffer
 * \param capacity a total capacity of given buffer
 * \param __s S-Allocated buffer
 *
 * \return new S-Allocated buffer.
 */
__sattr_veccall_const __sattr_inline void
    salloc_replace(register unsigned char * const restrict S_Nonnull buff,
                   register const size_t                             capacity,
                   register salloc_t * const restrict S_Nonnull      __s)
        __sattr_diagnose_align(capacity, SALLOC_MIN_BUFFER_SIZE);

/**
 * \b Copies all the pointers from \p __src to \p __dst .
 *
 * \note The previous data\pointers inside the \p __dst will not be changed\\free\\purged
 * at all, this method only copies all the data from \p __src to the start \p __dst until
 * it's fullfilment or until all the data from \p __src are copied, no buffer capacities
 * are changes.
 *
 *
 * \param __dst a destination S-Allocated buffer
 * \param __src a source S-Allocated buffer
 *
 * \return a destination S-Alloc pointer.
 */
__sattr_flatten_veccall_overload __sattr_inline salloc_t * S_Nonnull
    salloc_copy(register salloc_t * const restrict S_Nonnull __dst,
                register salloc_t * const restrict S_Nonnull __src);

/**
 * \b Marks given S-Allocated buffer as freed.
 *
 * \note The previous data\pointers inside the \p __s will not be changed\\free\\purged
 * at all, this method only marks the buffer inside as totally free to new S-Allocations.
 *
 * \param __s a S-Allocated buffer
 */
__sattr_flatten_veccall __sattr_inline void
    salloc_delete(register salloc_t * const restrict S_Nonnull __s);

/**
 * \b Get the \c capacity of given S-Allocated buffer.
 *
 * \param __s a S-Allocated buffer
 *
 * \return capacity of \c __s .
 */
__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_capacity(register const salloc_t * const restrict S_Nonnull __s);

/**
 * \b Get the size of allocated \\ used memory inside the S-Allocated buffer \p __s .
 *
 * \param __s a S-Allocated buffer
 *
 * \return memory size in-use in \c __s .
 */
__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_used(register const salloc_t * const restrict S_Nonnull __s);

/**
 * \b Get the size of un-allocated \\ unsed memory inside the S-Allocated buffer \p __s .
 *
 * \param __s a S-Allocated buffer
 *
 * \return unused memory size in \c __s .
 */
__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_unused(register const salloc_t * const restrict S_Nonnull __s);

/**
 * \b Checks is in \p __s enough space to allocate new pointer with at least \p __size
 * bytes.
 *
 * \param __s a S-Allocated buffer
 * \param __size a size to be checked for
 *
 * \return  \b 0 : new S-Allocation with at least \c __size will take all the unused
 * memory. \b Negative : size of how much the pointer with at least \c __size will
 * exceed the unused memory. \b Positive : available space after new S-Allocation with at
 * least \c __size bytes pointer.
 */
__sattr_flatten_veccall __sattr_inline ssize_t salloc_check(
    register const salloc_t * const restrict S_Nonnull __s, register const size_t __size)
    __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \b Checks is in \p __s is enough space to allocate at least \p __nmemb new pointers
 * with at least \p __size bytes each.
 *
 * \param __s a S-Allocated buffer
 * \param __size a size to be checked for
 * \param __nmemb N-pointers size to check for S-Allocations.
 *
 * \return \b 0 : new \c __nmemb S-Allocations with at least \c __size each will take all
 * the unused memory. \b Negative : the size of how much new \c __nmemb pointers with at
 * least |c __size each will exceed the unused memory. \b Positive : available space after
 * S-Allocation of \c __nmemb new pointers with at least \c __size  bytes each.
 */
__sattr_flatten_veccall __sattr_inline ssize_t salloc_ncheck(
    register const salloc_t * const restrict S_Nonnull __s,
    register const size_t                              __size,
    register const size_t __nmemb) __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \b Allocates new static pointer in \p __s buffer with at least \p __size bytes, and
 * returns it.
 *
 * \param __s a S-Allocated buffer
 * \param __size a size of new pointer to allocate in bytes
 *
 * \return  A valid pointer to operate with. \b NULL if: \c __s is \b NULL; \c __size is
 * equals to \b 0 ; static buffer in \c __s has no available memory;
 */
__sattr_veccall_overload __sattr_inline void * S_Nullable
    salloc(register salloc_t * const restrict S_Nonnull __s, register const size_t __size)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \b Allocates new static pointer in \p __s for an array of \p __nmemb elements of \p
 * __size bytes each, and returns it.
 *
 *
 * \param __s a S-Allocated buffer
 * \param __size a size of one new pointer to allocate in bytes
 * \param __nmemb N-pointer to allocate with \c __size bytes each
 *
 * \return A valid pointer with size `__nmemb * __size` bytes. \b NULL if: \c __s is \b
 * NULL; `__nmemb * __size` is equals to \b 0 ; static buffer in \c __s has no available
 * memory;
 */
__sattr_flatten_veccall __sattr_inline void * S_Nullable snalloc(
    register salloc_t * const restrict S_Nonnull __s,
    register const size_t                        __size,
    register const size_t __nmemb) __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \b Frees the \p __ptr .
 *
 * \note This function marking up space that was allocated for \p __ptr as free, and will
 * try to optimize a static buffer in \p __s .
 *
 * \attention A \c __ptr will be a valid pointer after this call of #sfree , but data
 * under it can possibly be overwritten with the next calls of S-Allocators.
 *
 * \param __s a S-Allocated buffer
 * \param __ptr a pointer from \c __s to be freed
 */
__sattr_veccall_overload __sattr_inline void
    sfree(register salloc_t * const restrict S_Nonnull __s,
          register void * restrict S_Nonnull           __ptr);

/**
 * **Un-safe free** of the \p __ptr .
 *
 * \note This function only marking-up space that was allocated for \c __ptr as free.
 *
 * \attention This function will not optimize a static buffer that you set with
 * #salloc_new at all. Use it on your risk or use #sfree with #salloc_t as the first
 * parameter. A \c __ptr will be a valid pointer after this call of #sfree , but data
 * under it can possibly be overwritten with the next calls of S-Allocators.
 *
 * \param __ptr a pointer to be marked as freed.
 */
__sattr_veccall __sattr_inline void smfree(register void * restrict S_Nonnull __ptr);

//
//----------------------
// OVERLOADS PROTOTYPES
//----------------------
//

#if SALLOC_OVERLOADS

/**
 * \b Replaces S-Allocated buffer in given \p __s with provided \p buff and \p capacity .
 *
 * \note The previous data\pointers inside the \p __s will not be changed\\free\\purged at
 * all, this method only replaces pointer to the \p buff and it's \p capacity inside the
 * given \p __s .
 *
 * \param buff a pointer to new buffer
 * \param capacity a total capacity of given buffer
 * \param __s S-Allocated buffer
 *
 * \return new S-Allocated buffer.
 */
__sattr_veccall_const_overload __sattr_inline void
    salloc_new(register unsigned char * const restrict S_Nonnull buff,
               register const size_t                             capacity,
               register salloc_t * const restrict S_Nonnull      __s)
        __sattr_diagnose_align(capacity, SALLOC_MIN_BUFFER_SIZE);

/**
 * \b Checks is in \p __s enough space to allocate new pointer with at least \p __size
 * bytes.
 *
 * \param __s a S-Allocated buffer
 * \param __size a size to be checked for
 *
 * \return  \b 0 : new S-Allocation with at least \c __size will take all the unused
 * memory. \b Negative : size of how much the pointer with at least \c __size will
 * exceed the unused memory. \b Positive : available space after new S-Allocation with at
 * least \c __size bytes pointer.
 */
__sattr_flatten_veccall_overload __sattr_inline ssize_t salloc_unused(
    register const salloc_t * const restrict S_Nonnull __s, register const size_t __size)
    __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \b Checks is in \p __s is enough space to allocate at least \p __nmemb new pointers
 * with at least \p __size bytes each.
 *
 * \param __s a S-Allocated buffer
 * \param __size a size to be checked for
 * \param __nmemb N-pointers size to check for S-Allocations.
 *
 * \return \b 0 : new \c __nmemb S-Allocations with at least \c __size each will take all
 * the unused memory. \b Negative : the size of how much new \c __nmemb pointers with at
 * least |c __size each will exceed the unused memory. \b Positive : available space after
 * S-Allocation of \c __nmemb new pointers with at least \c __size  bytes each.
 */
__sattr_flatten_veccall_overload __sattr_inline ssize_t salloc_unused(
    register const salloc_t * const restrict S_Nonnull __s,
    register const size_t                              __size,
    register const size_t __nmemb) __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * \b Frees the \p __ptr .
 *
 * \note This function marking up space that was allocated for \p __ptr as free, and will
 * try to optimize a static buffer in \p __s .
 *
 * \attention A \c __ptr will be a valid pointer after this call of #sfree , but data
 * under it can possibly be overwritten with the next calls of S-Allocators.
 *
 * \param __s a S-Allocated buffer
 * \param __ptr a pointer from \c __s to be freed
 */
__sattr_flatten_veccall_overload __sattr_inline void * S_Nullable salloc(
    register salloc_t * const restrict S_Nonnull __s,
    register const size_t                        __size,
    register const size_t __nmemb) __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE);

/**
 * **Un-safe free** of the \p __ptr .
 *
 * \note This function only marking-up space that was allocated for \c __ptr as free.
 *
 * \attention This function will not optimize a static buffer that you set with
 * #salloc_new at all. Use it on your risk or use #sfree with #salloc_t as the first
 * parameter. A \c __ptr will be a valid pointer after this call of #sfree , but data
 * under it can possibly be overwritten with the next calls of S-Allocators.
 *
 * \param __ptr a pointer to be marked as freed.
 */
__sattr_flatten_veccall_overload __sattr_inline void
    sfree(register void * restrict S_Nonnull __ptr);
#endif

/* Additional functional */

#ifdef __sis_debug_defined__
/**
 * Prints \\ trace all the S-Allocated memory chunks stored in provided buffer.
 */
#  define salloc_trace(__sptr) __strace(__sptr)

__sattr_flatten_veccall __sattr_inline void
    __strace(register salloc_t * const restrict S_Nonnull __s);
#else

/**
 * Debug Macros Disabled.
 */
#  define salloc_trace(__sptr)
#endif

/*
 * --------------------
 * FUNCTIONS DEFINITION
 * --------------------
 */

__sattr_veccall_const_overload __sattr_inline salloc_t
    salloc_new(register unsigned char * const restrict S_Nonnull buff,
               register const size_t                             capacity) {
  unsigned char * restrict S_Nonnull buff_end = (buff + capacity);
  salloc_t                           out      = (salloc_t){buff, buff_end, buff};

  return out;
}
__sattr_veccall_const __sattr_inline void
    salloc_replace(register unsigned char * const restrict S_Nonnull buff,
                   register const size_t                             capacity,
                   register salloc_t * const restrict S_Nonnull      __s) {
  salloc_t __s_new = salloc_new(buff, capacity);

  *__s = __s_new;
}

__sattr_flatten_veccall_overload __sattr_inline salloc_t * S_Nonnull
    salloc_copy(register salloc_t * const restrict S_Nonnull __dst,
                register salloc_t * const restrict S_Nonnull __src) {
  unsigned char * restrict S_Nonnull __src_iptr = __src->start;
  unsigned char * restrict S_Nonnull __dst_iptr = __dst->start;

  __dst->cursor = __dst->start;

  while (__dst_iptr < __dst->end && __src_iptr < __src->cursor) {
    __s_tag_t * restrict __src_iptr_tag = __s2c_tag(__src_iptr);
    const size_t  __src_iptr_copy_size  = __src_iptr_tag->size + __st_bd_size;
    const ssize_t __dst_available_space = salloc_unused(__dst);

    if (0 < __dst_available_space &&
        __s2c_uiptr(__dst_available_space) >= __src_iptr_copy_size) {
      for (size_t i = 0; __src_iptr_copy_size > i; ++i) {
        *__dst_iptr++ = *__src_iptr++;
      }
    } else {
      break;
    }

    __dst->cursor = __dst_iptr;
  }

  return __dst;
}

__sattr_flatten_veccall __sattr_inline void
    salloc_delete(register salloc_t * const restrict S_Nonnull __s) {
  unsigned char * restrict new_cursor = __s->cursor;

  if (__s->cursor != __s->start) {
    new_cursor = __s->start;
  }

  __s->cursor = new_cursor;
}

__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_capacity(register const salloc_t * const restrict S_Nonnull __s) {
  const ssize_t capacity = __s->end - __s->start;

  return capacity;
}

__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_used(register const salloc_t * const restrict S_Nonnull __s) {
  const ssize_t used = __s->cursor - __s->start;

  return used;
}

__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_unused(register const salloc_t * const restrict S_Nonnull __s) {
  const ssize_t unused = __s->end - __s->cursor;

  return unused;
}
__sattr_flatten_veccall __sattr_inline ssize_t
    salloc_check(register const salloc_t * const restrict S_Nonnull __s,
                 register const size_t                              __size) {
  const size_t  asize  = __st_align_size(__size);
  const ssize_t unused = salloc_unused(__s);
  const ssize_t out    = unused - asize - __st_bd_size;

  return out;
}
__sattr_flatten_veccall __sattr_inline ssize_t
    salloc_ncheck(register const salloc_t * const restrict S_Nonnull __s,
                  register const size_t                              __size,
                  register const size_t                              __nmemb) {
  const size_t  asize        = __st_align_size(__size);
  const size_t  require_size = __nmemb * (asize + __st_bd_size);
  const ssize_t unused       = salloc_unused(__s);
  const ssize_t out          = unused - require_size;

  return out;
}

/**
 * Extending the used memory cursor if it's possible and if in \p __s is no best-fit
 * memory chunk for new s-allocation.
 *
 * \param __s a S-Allocated memory buffer
 * \param size a new pointer size to be mapped
 *
 * \return NULL if there no space for new S-Allocation, or valid pointer to be used.
 */
__sattr_flatten_veccall __sattr_inline void * S_Nullable __salloc_extend_cursor(
    register salloc_t * const restrict S_Nonnull __s, register const uintptr_t size) {
  register const uintptr_t alloc_size = size + __st_bd_size;

  if ((__s->cursor + alloc_size) <= __s->end) {
    register unsigned char * const restrict S_Nonnull out = __s->cursor;

    register __s_tag_t * const restrict header = __s2c_tag(out);
    register __s_tag_t * const restrict footer = __s2c_tag(out + size + __st_size);
    register const __s_tag_t payload           = __st_init(size, __st_busy);

    *header = payload;
    *footer = payload;

    register unsigned char * const restrict S_Nonnull new_cursor = out + alloc_size;
    __s->cursor                                                  = new_cursor;

    return __st_tag_get_ptr(out);
  } else {
    return NULL;
  }
}
/**
 * Fragmentation memory space if best-fit memory chunk was found.
 *
 * \param __bestptr a pointer to best-fit pointer
 * \param __bestsize a size of best pointer
 * \param __requier_size a original require size to find
 */
__sattr_flatten_veccall __sattr_inline void
    __salloc_found_chunk_update(const unsigned char * const restrict S_Nonnull __bestptr,
                                const uintptr_t                                __bestsize,
                                const uintptr_t __require_size) {

  const unsigned char * restrict __ptr;
  uintptr_t     __ptr_size;
  unsigned char is_busy;

  if ((__bestsize - __st_bd_size) > __require_size) {
    __ptr_size = __bestsize - __st_bd_size - __require_size;
    __ptr      = __bestptr + __require_size + __st_bd_size;
    is_busy    = __st_not_busy;
  } else {
    /**
     * may seem useless at first sight, because for the \p __bestptr I already set
     * it's size in #__salloc_find_best_chunk , but there I actually set only
     * \p __require_size , but \p __bestsize can be grater than that value only for a
     * few bytes, and less then \c (__bestsize-__st_bd_size)
     */
    __ptr_size = __bestsize;
    __ptr      = __bestptr;
    is_busy    = __st_busy;
  }

  register __s_tag_t * const restrict __fragptr_header = __s2c_tag(__ptr);
  register __s_tag_t * const restrict __fragptr_footer =
      __s2c_tag(__ptr + __ptr_size + __st_size);
  register const __s_tag_t __fragptr_payload =
      (const __s_tag_t)__st_init(__ptr_size, is_busy);

  *__fragptr_header = __fragptr_payload;
  *__fragptr_footer = __fragptr_payload;
}
/**
 * Trying to find freed, best-fit memory chunk in all currently used memory in static
 * buffer memory for \c __require_size bytes
 *
 * \param __s a S-Allocated buffer
 * \param __require_size a searching size for new pointer
 *
 * \return a available space pointer to already mapped memory in static buffer, \c NULL
 * if not.
 */
__sattr_flatten_veccall __sattr_inline void * S_Nullable
    __salloc_find_best_chunk(register salloc_t * const restrict S_Nonnull __s,
                             register const size_t __require_size) {
  const unsigned char * const restrict S_Nonnull __s_cursor = __s->cursor;

  unsigned char * restrict S_Nonnull __iptr     = __s->start;
  unsigned char * restrict S_Nonnull __bestptr  = 0;
  uintptr_t                          __isize    = 0;
  uintptr_t                          __bestsize = salloc_capacity(__s);

  while (__iptr < __s_cursor) {
    const unsigned char is_free = __st_is_free(__iptr);

    __isize = __st_get_size(__iptr);
    if (!!is_free && __isize >= __require_size && __isize <= __bestsize) {
      __bestsize = __isize;
      __bestptr  = __iptr;
    }
    __iptr += __isize + __st_bd_size;
  }

  if (__bestptr) {
    __s_tag_t *     __bestptr_header  = __s2c_tag(__bestptr);
    __s_tag_t *     __bestptr_footer  = __s2c_tag(__bestptr + __require_size + __st_size);
    const __s_tag_t __bestptr_payload = __st_init(__require_size, __st_busy);

    *__bestptr_header = __bestptr_payload;
    *__bestptr_footer = __bestptr_payload;

    if (__bestsize > __require_size) {
      __salloc_found_chunk_update(__bestptr, __bestsize, __require_size);
    }

    return __st_tag_get_ptr(__bestptr);
  }

  return NULL;
}
__sattr_veccall_overload __sattr_inline void * S_Nullable salloc(
    register salloc_t * const restrict S_Nonnull __s, register const size_t __size) {
  register const uintptr_t aligned_size = __st_align_size(__size);
  register void * restrict out          = NULL;

  if (__s->start != __s->cursor) {
    out = __salloc_find_best_chunk(__s, aligned_size);
  }

  if (!out) {
    out = __salloc_extend_cursor(__s, aligned_size);
  }

  return out;
}
__sattr_flatten_veccall __sattr_inline void * S_Nullable
    snalloc(register salloc_t * const restrict S_Nonnull __s,
            register const size_t                        __size,
            register const size_t                        __nmemb) {
  const size_t __arr_size = __nmemb * __size;
  void *       out        = salloc(__s, __arr_size);

  return out;
}

#if SALLOC_OVERLOADS
__sattr_flatten_veccall_overload __sattr_inline void * S_Nullable
    salloc(register salloc_t * const restrict S_Nonnull __s,
           register const size_t                        __size,
           register const size_t                        __nmemb) {
  void * out = snalloc(__s, __size, __nmemb);
  return out;
}
#endif

/**
 * \c Concates 2 pointers, usually it's freed pointer and all the free space before and
 * after freed pointer.
 *
 * \param __update_ptr freed pointer
 * \param __concat_ptr pointer to be concated with freed
 */
__sattr_flatten_veccall __sattr_inline void __sfree_frag_concat(
    register unsigned char * restrict S_Nonnull             __update_ptr,
    register const unsigned char * const restrict S_Nonnull __concat_ptr) {
  register const uintptr_t __concant_size    = __st_get_size(__concat_ptr);
  register const uintptr_t __update_ptr_size = __st_get_size(__update_ptr);
  register const uintptr_t __new_size = __concant_size + __st_bd_size + __update_ptr_size;

  __s_tag_t * const restrict __update_ptr_header = __s2c_tag(__update_ptr);
  __s_tag_t * const restrict __update_ptr_footer =
      __s2c_tag(__update_ptr + __new_size + __st_size);
  const __s_tag_t __update_ptr_payload = __st_init(__new_size, __st_not_busy);

  *__update_ptr_header = __update_ptr_payload;
  *__update_ptr_footer = __update_ptr_payload;
}

/**
 * After each fragmentation trying to shrink the static buffer memory usage cursor.
 *
 * Example:
 * ```
 * < - cursor position
 * Before:
 * [busy] [busy] [__ptr chunk]< [un-allocated memory here]
 * After:
 * [busy] [busy]< [              un-allocated memory here]
 * ```
 *
 * \param __s a S-Allocated buffer
 * \param __ptr a freed pointer from \c __s
 *
 * \return \c 1 if cursor shrinked, \c 0 if not
 *
 */
__sattr_flatten_veccall __sattr_inline unsigned char
    __sfree_shrink_cursor(register salloc_t * const restrict S_Nonnull      __s,
                          register unsigned char * const restrict S_Nonnull __ptr) {
  unsigned char is_moved = 0;

  if (__s->cursor <= (__ptr + __st_get_size(__ptr) + __st_bd_size)) {
    __s->cursor = __ptr;
    is_moved    = 1;
  }

  return is_moved;
}

__sattr_veccall_overload __sattr_inline void
    sfree(register salloc_t * const restrict S_Nonnull __s,
          register void * restrict S_Nonnull           __ptr) {
  smfree(__ptr);

  unsigned char * restrict S_Nonnull __iptr = __s2c_ptr(__st_ptr_get_tag(__ptr));

  /*
   * Left-side memory fragmentation.
   * Before:
   * [busy] [while_free_chunks] [free_chunk1] [__ptr chunk] [busy]
   * After:
   * [busy] [   new_big_damn_ass_fragmented_memory_chunk  ] [busy]
   */
  if (__iptr > __s->start) {
    unsigned char * restrict S_Nonnull __baseptr = __iptr;
    __iptr = __baseptr - __st_get_size(__baseptr - __st_size) - __st_bd_size;

    while (__iptr >= __s->start && __st_is_free(__iptr)) {
      __sfree_frag_concat(__iptr, __baseptr);

      unsigned char * restrict S_Nonnull __prev_ptr = __iptr - __st_size;
      if (__s->start <= __prev_ptr) {
        __s_tag_t * __prev_tag = __s2c_tag(__prev_ptr);
        __prev_ptr             = __prev_ptr - __prev_tag->size - __st_size;
      }

      __baseptr = __iptr;
      __iptr    = __prev_ptr;
    }

    __iptr = __baseptr;
  }

  if (__sfree_shrink_cursor(__s, __iptr)) {
    /*
     * If cursor was moved after left-side fragmentation that means right-side has no
     * available memory at all
     */
    return;
  } else

  /*
   * Right-side memory fragmentation.

   * Before:
   * [busy] [ left_side_fragmented_chunk ] [free_chunk1] [busy]
   * After:
   * [busy] [ new_big_damn_ass_fragmented_memory_chunk ] [busy]
   */
  {
    unsigned char * restrict S_Nonnull __baseptr = __iptr;
    __iptr = __baseptr + __st_get_size(__baseptr) + __st_bd_size;

    while (__iptr < __s->cursor && __st_is_free(__iptr)) {
      __sfree_frag_concat(__baseptr, __iptr);
      __iptr += __st_get_size(__iptr) + __st_bd_size;
    }

    __sfree_shrink_cursor(__s, __baseptr);
  }
}

__sattr_flatten_veccall __sattr_inline void
    smfree(register void * restrict S_Nonnull __ptr) {
  __s_tag_t * restrict header = __st_ptr_get_tag(__ptr);
  const size_t ptr_size       = header->size;
  __s_tag_t * restrict footer = __s2c_tag(__s2c_ptr(__ptr) + ptr_size);
  const __s_tag_t payload     = __st_init(ptr_size, __st_not_busy);

  *header = payload;
  *footer = payload;
}

//
//-----------------------
// OVERLOADS DEFINITIONS
//-----------------------
//

#if SALLOC_OVERLOADS

__sattr_veccall_const_overload __sattr_inline void
    salloc_new(register unsigned char * const restrict S_Nonnull buff,
               register const size_t                             capacity,
               register salloc_t * const restrict S_Nonnull      __s) {
  salloc_replace(buff, capacity, __s);
}

__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_unused(register const salloc_t * const restrict S_Nonnull __s,
                  register const size_t                              __size) {
  return salloc_check(__s, __size);
}
__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_unused(register const salloc_t * const restrict S_Nonnull __s,
                  register const size_t                              __size,
                  register const size_t                              __nmemb) {
  return salloc_ncheck(__s, __size, __nmemb);
}

__sattr_flatten_veccall_overload __sattr_inline void
    sfree(register void * restrict S_Nonnull __ptr) {
  smfree(__ptr);
}

#endif /* SALLOC_OVERLOADS */

/*
 * -----------------------------
 * SALLOC_GDI_BUFFER DEFINITIONS
 * -----------------------------
 */

#if __sis_gdi_buffer_defined__

/*
 * Below just an interfaces\accessors to all the standard s-allocators via gdi
 * buffer, so there is no additional prototypes or any other comments.
 */

__sattr_veccall_const __sattr_inline salloc_t * S_Nonnull __salloc_get_gdi_buffer(void) {
  static unsigned char __buffer[SALLOC_GDI_BUFFER_SIZE];
  static salloc_t      __slc = {__buffer, __buffer + SALLOC_GDI_BUFFER_SIZE, __buffer};

  return &__slc;
}

__sattr_flatten_veccall_overload __sattr_inline void * S_Nullable salloc(
    register const size_t __size) __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  void * S_Nullable __ret       = salloc(__gdi_slc, __size);

  return __ret;
}
__sattr_flatten_veccall_overload __sattr_inline void * S_Nullable
    salloc(register const size_t __size, register const size_t __nmemb)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  void * S_Nullable __ret       = salloc(__gdi_slc, __size, __nmemb);

  return __ret;
}

__sattr_flatten_veccall_overload __sattr_inline void
    sfree_gdi(register void * restrict const __gdi_ptr) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  sfree(__gdi_slc, __gdi_ptr);
}

__sattr_flatten_veccall_overload __sattr_inline void * S_Nullable
    salloc_copy(register salloc_t * const restrict S_Nonnull __src) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  void * S_Nullable __ret       = salloc_copy(__gdi_slc, __src);

  return __ret;
}

__sattr_flatten_veccall_overload __sattr_inline void salloc_delete(void) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  salloc_delete(__gdi_slc);
}

#  ifdef SALLOC_DEBUG
__sattr_flatten_veccall_overload __sattr_inline void salloc_trace(void) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  salloc_trace(__gdi_slc);
}
#  endif

__sattr_flatten_veccall_overload __sattr_inline ssize_t salloc_capacity(void) {
  return SALLOC_GDI_BUFFER_SIZE;
}

__sattr_flatten_veccall_overload __sattr_inline ssize_t salloc_used(void) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  ssize_t __ret                 = salloc_used(__gdi_slc);

  return __ret;
}

__sattr_flatten_veccall_overload __sattr_inline ssize_t salloc_unused(void) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  ssize_t __ret                 = salloc_unused(__gdi_slc);

  return __ret;
}
__sattr_flatten_veccall_overload __sattr_inline ssize_t salloc_unused(
    register const size_t __size) __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  ssize_t __ret                 = salloc_unused(__gdi_slc, __size);

  return __ret;
}
__sattr_flatten_veccall_overload __sattr_inline ssize_t
    salloc_unused(register const size_t __size, register const size_t __nmemb)
        __sattr_diagnose_align(__size, SALLOC_MIN_ALLOC_SIZE) {
  salloc_t * restrict __gdi_slc = __salloc_get_gdi_buffer();
  ssize_t __ret                 = salloc_unused(__gdi_slc, __size, __nmemb);

  return __ret;
}

#endif

#ifdef __sis_debug_defined__
#  ifdef __clang__
#    pragma clang diagnostic ignored "-Wformat"
#  else
#    pragma GCC diagnostic ignored "-Wformat"
#  endif

__sattr_flatten_veccall __sattr_inline void
    __strace(register salloc_t * const restrict S_Nonnull __s) {
  unsigned char * restrict S_Nonnull iptr   = __s->start;
  unsigned char * restrict S_Nonnull cursor = __s->cursor;

  printf("\n         -------------------------"
         "\n            SALLOC BUFFER TRACE"
         "\n         -------------------------\n");

  size_t chunks_count = 0;
  while (iptr < cursor) {
    __s_tag_t * restrict iptr_header = __s2c_tag(iptr);
    __s_tag_t * restrict iptr_footer = __s2c_tag(iptr + iptr_header->size + __st_size);

    printf("%d %-6zu [%09p ... %09p] %d %zu\n",
           iptr_header->busy,
           iptr_header->size,
           iptr_header,
           iptr_footer,
           iptr_footer->busy,
           iptr_footer->size);

    iptr = __s2c_ptr(iptr_footer) + __st_size;
    ++chunks_count;
  }

  const ssize_t unused   = salloc_unused(__s);
  const size_t  used     = salloc_used(__s);
  const size_t  capacity = salloc_capacity(__s);

  if (used) {
    printf("         -------------------------\n"
           "    used [%09p ... %09p] %zu (%zu)\n",
           __s->start,
           cursor,
           used - (chunks_count * __st_bd_size),
           used);
  }

  printf("  unused [%09p ... %09p] %zu\n"
         "capacity [%-11zu %+11zu]\n",
         cursor,
         __s->end,
         unused,
         capacity,
         chunks_count);
}
#endif

/*
 * ---------------
 * MACROS UNDEFINE
 * ---------------
 */

#ifdef __sis_afteruse_macros_defined__
#  warning "salloc macroses still defined."
#  undef __sis_macroses_defined__
#endif
#ifdef __sis_afteruse_attrs_defined__
#  warning "salloc attributes still defined."
#  undef __sis_attrs_defined__
#endif

#ifdef __sis_attrs_defined__
#  undef __sis_attrs_defined__
#  undef __sattr_diagnose_if
#  undef __sattr_diagnose_align
#  undef __sattr_veccall
#  undef __sattr_const
#  undef __sattr_overload
#  undef __sattr_flatten
#  undef __sattr_munused
#  undef __sattr_artificial
#  undef __sattr_inline
#  undef __sattr_veccall_const
#  undef __sattr_veccall_const_overload
#  undef __sattr_veccall_overload
#  undef __sattr_flatten_veccall_overload
#  undef __sattr_flatten_veccall
#endif

#ifdef __sis_null_defined__
#  undef __sis_null_defined__
#  undef NULL
#endif

#if __sis_macroses_defined__
#  undef __sis_macroses_defined__

#  undef S_Nonnull
#  undef S_Nullable

#  undef __S_WORDSIZE
#  undef __S_TAG_ALIGN_BITS
#  undef __S_TAG_SIZE_BITS
#  undef __S_TAG_BUSY_BITS

#  undef __s2c_uiptr
#  undef __s2c_sptr
#  undef __s2c_tag

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

#ifdef __clang__
#  pragma clang diagnostic pop
#else
#  pragma GCC diagnostic pop
#endif

#endif /* __SALLOC_H__ */
