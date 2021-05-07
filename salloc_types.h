#ifndef __SALLOC_TYPES_H__
#define __SALLOC_TYPES_H__

#include "salloc_attrs.h"
#include <stddef.h>
#include <stdint.h>

#define auto __auto_type
#define nullptr ((void *)0)
#define from
#define typeof(x) __typeof__(x)
#if __is_c2x_support__
#  define alignof(x) (_Alignof(x))
#else
#  define alignof(x) (__alignof__(x))
#endif

#define libsalloc_define_vec(n) \
  typedef uintptr_t __attribute__((__aligned__, __ext_vector_type__(n))) v##n##pu_t; \
  typedef v##n##pu_t salloc_vec##n##_t;

libsalloc_define_vec(4);
libsalloc_define_vec(2);

typedef struct s_salloc_mem_addr_t {
  /* cursor to the end of available space \ start of each new chunk */
  void * const restrict cursor;
  void * const restrict start;
  void * const restrict end;
  const size_t          capacity;
} libsalloc_attr_alignof(salloc_vec4_t) salloc_mem_addr_t;

typedef union u_salloc_mem {
  salloc_mem_addr_t addr;
  salloc_vec4_t     vec;
} salloc_mem_t libsalloc_attr_transparent;

typedef struct s_salloc {
  unsigned char * restrict buff;
  size_t                   buff_length;
  uintptr_t                previous_offset;
  uintptr_t                current_offset;
} libsalloc_attr_alignof(salloc_vec4_t) salloc_t;

typedef union {
  salloc_t      data;
  salloc_vec4_t vector;
} salloc_allocator_t libsalloc_attr_transparent;

#endif /* __SALLOC_TYPES_H__ */
