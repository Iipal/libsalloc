#ifndef __SALLOC_BASE_TYPES_H__
#define __SALLOC_BASE_TYPES_H__

#define auto __auto_type
#define nullptr ((void *)0)
#define from

#define typeof(x) __typeof__(x)

#define libsalloc_define_vec(n) \
  typedef uintptr_t __attribute__((__aligned__, __ext_vector_type__(n))) v##n##pu_t; \
  typedef v##n##pu_t salloc_vec##n##_t;

libsalloc_define_vec(4);
libsalloc_define_vec(2);

#endif /* __SALLOC_BASE_TYPES_H__ */
