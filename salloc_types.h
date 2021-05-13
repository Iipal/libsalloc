#ifndef __SALLOC_TYPES_H__
#define __SALLOC_TYPES_H__

#if !defined(__SALLOC_H__)
#  error "Do not include `salloc_types.h` directly."
#endif

#include "salloc_attrs.h"

#ifndef NULL
#  define __is_salloc_null_defined__ 1

#  define NULL ((void *)0)
#endif

#if defined(__x86_64__) && !defined(__ILP32__)
typedef unsigned long int __s_uintptr_t; /* salloc analog of uintptr_t; x64  */
#else
typedef unsigned int __s_uintptr_t; /* salloc analog of uintptr_t; x32 */
#endif

typedef __SIZE_TYPE__                __s_size_t;  /* salloc analog of size_t */
typedef unsigned char                __s_uint8_t; /* salloc analog of uint8_t  */
typedef __s_uint8_t * restrict       __s_ptr_t;   /* alias for most common pointer type */
typedef __s_uint8_t * const restrict __s_cptr_t;  /* __s_ptr_t but const */

/**
 * Memory chunk mapping with bidirectional implicit free list.
 *
 * Example:
 * By calling salloc with \c size of 48 bytes it actually allocating \c size +
 * \c (sizeof(__s_chunk_t)*2) bytes for memory mapping:
 *
 * [ __s_chunk_t with ->size == 48 and by default ->busy == 1] **header**
 * [                 your 48 bytes of memory                 ] **payload**
 * [ __s_chunk_t with ->size == 48 and by default ->busy == 1] **footer**
 */
typedef struct __s_salloc_chunk {
  __s_uintptr_t size : 60; /* size of current memory chunk */
  __s_uint8_t   busy : 1;  /* indicates is current memory chunk was freed or not */
  __s_uint8_t   __alignment_dummy : 3 __sattr_munused; /* as it is */
} __sattr_packed __s_chunk_t;

typedef struct __s_salloc_t {
  __s_ptr_t start;  /* start of available space in static buffer */
  __s_ptr_t end;    /* end of available space in static buffer */
  __s_ptr_t cursor; /* cursor to the max address of current static buffer in use */
} salloc_t;

#endif /* __SALLOC_TYPES_H__ */
