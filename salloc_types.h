#ifndef __SALLOC_TYPES_H__
#define __SALLOC_TYPES_H__

#if !defined(__SALLOC_H__)
#  error "Do not include `salloc_types.h` directly."
#endif

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

#undef __salloc_define_vec
#define __salloc_define_vec(n) \
  typedef uintptr_t __attribute__((__aligned__, __ext_vector_type__(n))) v##n##pu_t; \
  typedef v##n##pu_t salloc_vec##n##_t

__salloc_define_vec(4);
__salloc_define_vec(2);

#undef __salloc_define_vec

typedef struct s_salloc_chunk {
  uintptr_t size : 60;
  uint8_t   inuse : 1;
  uint8_t   __alignment_dummy : 3 libsalloc_attr_munused;
} libsalloc_attr_packed salloc_chunk_t;

#define _(x) sizeof(salloc_chunk_t) /

typedef struct s_salloc_mem_t {
  /* cursor to the end of available space \ start of each new chunk */
  uint8_t * restrict start;
  uint8_t * restrict end;
  uint8_t * restrict cursor;
  size_t             capacity;
} libsalloc_attr_alignof(salloc_vec4_t) salloc_addr_t;

typedef union u_salloc {
  salloc_addr_t addr;
  salloc_vec4_t vec;
} salloc_t libsalloc_attr_transparent;

typedef struct _s_salloc {
  uint8_t * restrict buff;
  size_t             buff_length;
  uintptr_t          previous_offset;
  uintptr_t          current_offset;
} libsalloc_attr_alignof(salloc_vec4_t) _salloc_t;

typedef union {
  _salloc_t     data;
  salloc_vec4_t vector;
} salloc_allocator_t libsalloc_attr_transparent;

#endif /* __SALLOC_TYPES_H__ */
