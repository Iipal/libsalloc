#ifndef __SALLOC_TYPES_H__
#define __SALLOC_TYPES_H__

#if !defined(__SALLOC_H__)
#  error "Do not include `salloc_types.h` directly."
#endif

#include "salloc_attrs.h"
#include <stddef.h>
#include <stdint.h>

#define nullptr ((void *)0)
#define from
#define bool _Bool
#define true 1
#define false 0

#define auto __auto_type
#define typeof(x) __typeof__(x)
#if __is_c2x_support__
#  define alignof(x) (_Alignof(x))
#else
#  define alignof(x) (__alignof__(x))
#endif

typedef uintptr_t __attribute__((__aligned__, __ext_vector_type__(4))) salloc_vec4_t;

typedef uint8_t * restrict       sptr_t;
typedef uint8_t * restrict const scptr_t;

typedef struct s_salloc_chunk {
  uintptr_t size : 60;
  uint8_t   busy : 1;
  uint8_t   __alignment_dummy : 3 libsalloc_attr_munused;
} libsalloc_attr_packed salloc_chunk_t;

typedef struct s_salloc_mem_t {
  /* cursor to the end of available space \ start of each new chunk while != end*/
  sptr_t start;
  sptr_t end;
  sptr_t cursor;
  size_t capacity;
} libsalloc_attr_alignof(salloc_vec4_t) salloc_t;

#endif /* __SALLOC_TYPES_H__ */
