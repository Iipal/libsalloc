#ifndef __SALLOC_TYPES_H__
#define __SALLOC_TYPES_H__

#if !defined(__SALLOC_H__)
#  error "Do not include `salloc_types.h` directly."
#endif

#include "salloc_attrs.h"
#include <stddef.h>
#include <stdint.h>

#ifndef bool
#  define bool _Bool
#  define true 1
#  define false 0
#endif

typedef uint8_t * restrict       sptr_t;
typedef uint8_t * restrict const scptr_t;

typedef struct s_salloc_chunk {
  uintptr_t size : 60;
  uint8_t   busy : 1;
  uint8_t   __alignment_dummy : 3 __sattr_munused;
} __sattr_packed salloc_chunk_t;

typedef struct s_salloc_mem_t {
  /* cursor to the end of available space \ start of each new chunk while != end*/
  sptr_t start;
  sptr_t end;
  sptr_t cursor;
} salloc_t;

#endif /* __SALLOC_TYPES_H__ */
