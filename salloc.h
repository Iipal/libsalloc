#ifndef __SALLOC_H__
#define __SALLOC_H__

#if defined(SALLOC_UNSAFE)
#  undef SALLOC_UNSAFE
/**
 * Removes some nullptr-checks. Better performance but causes UB.
 */
#  define SALLOC_UNSAFE 1
#endif

#if defined(SALLOC_UNSAFE_MAPPING)
#  undef SALLOC_UNSAFE_MAPPING
/**
 * Removes some mapping checks, like check of start\end of available memory
 */
#  define SALLOC_UNSAFE_MAPPING 1
#endif

#include "salloc_attrs.h"
#include "salloc_types.h" /* may be removed in future */
#include <stdio.h>        /* deprecated in release-version. used only by salloc_trace */

/**
 * \brief Creating a new static buffer to use by \c salloc -allocators.
 *
 * \param buff a pointer to static buffer.
 * \param buff_length a \c buff length \ capacity.
 *
 * \return new \c salloc_t object to work with \c salloc -allocators.
 */
libsalloc_attr_veccall_const static inline salloc_t
    salloc_new(register const void * const restrict buff,
               register const size_t                buff_length);

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
libsalloc_attr_flatten_veccall static inline void
    salloc_delete(register salloc_t * const restrict __s);

/**
 * \brief Just prints \ trace all the pointers stored in \c salloc_t object.
 *
 * \deprecated This function using libc \c printf , and incudes \c stdio.h .
 *
 * \param __s a pointer to \c salloc_t object.
 */
libsalloc_attr_veccall static inline void
    salloc_trace(register salloc_t * const restrict __s);

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
libsalloc_attr_veccall_overload static inline void *
    salloc(register salloc_t * const restrict __s, register const size_t __size);

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
libsalloc_attr_flatten_veccall_overload static inline void *
    salloc(register salloc_t * const restrict __s,
           register const size_t              __size,
           register const size_t              __nmemb);

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
libsalloc_attr_veccall_overload static inline void
    sfree(register salloc_t * const restrict __s, register void * restrict __ptr);

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
libsalloc_attr_flatten_veccall_overload static inline void
    sfree(register void * restrict __ptr);

/**
 *  Previous prototypes:
 */

libsalloc_attr_veccall_const static inline salloc_vec4_t
    _salloc_init(register const void * const restrict buff,
                 register const size_t                buff_length);

libsalloc_attr_veccall_overload static inline void *
    _salloc(register salloc_vec4_t * restrict allocator, register const size_t size);
libsalloc_attr_veccall_overload static inline void *
    _salloc(register salloc_vec4_t * const restrict allocator,
            register const size_t                   size,
            register const size_t                   align);

libsalloc_attr_flatten_veccall_overload static inline void *
    _srealloc(register salloc_vec4_t * const restrict allocator,
              register void * const restrict          old_memory,
              register const size_t                   old_size,
              register const size_t                   new_size);
libsalloc_attr_flatten_veccall_overload static inline void *
    _srealloc(register salloc_vec4_t * const restrict allocator,
              register void * const restrict          old_memory,
              register const size_t                   old_size,
              register const size_t                   new_size,
              register const size_t                   align);

#pragma clang diagnostic ignored "-Wgcc-compat"

libsalloc_attr_veccall_const_overload static inline void _sfree(
    register salloc_vec4_t * restrict allocator, void * restrict pointer)
    libsalloc_attr_deprecated("Not needed to implement: sfree(salloc_vec4_t *, void *)") {
  /* Not needed to implement */
  (void)allocator;
  (void)pointer;
}
libsalloc_attr_veccall_overload static inline void
    _sfree(register salloc_vec4_t * const restrict allocator);

libsalloc_attr_veccall_const static inline uintptr_t
    _salloc_align_forward(register const uintptr_t pointer, register const size_t align);

#include "salloc_internal.h"
#include "salloc_undef.h"

#endif /* __SALLOC_H__ */
