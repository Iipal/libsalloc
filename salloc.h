#ifndef __SALLOC_H__
#define __SALLOC_H__

#include "salloc_attrs.h"
#include "salloc_base_types.h" /* may be removed in future */
#include <stddef.h>            /* for size_t only */

typedef union u_salloc_mem salloc_mem_t libsalloc_attr_transparent;

/**
 * New prototypes:
 */
libsalloc_attr_veccall_const static inline salloc_mem_t
    salloc_init(register const void * const restrict buff,
                register const size_t                buff_length);

libsalloc_attr_veccall_overload static inline void *
    salloc(register salloc_mem_t * const restrict mem, register const size_t size);

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
    srealloc(register salloc_vec4_t * const restrict allocator,
             register void * const restrict          old_memory,
             register const size_t                   old_size,
             register const size_t                   new_size);
libsalloc_attr_flatten_veccall_overload static inline void *
    srealloc(register salloc_vec4_t * const restrict allocator,
             register void * const restrict          old_memory,
             register const size_t                   old_size,
             register const size_t                   new_size,
             register const size_t                   align);

libsalloc_attr_veccall_const_overload static inline void sfree(
    register salloc_vec4_t * restrict allocator, void * restrict pointer)
    libsalloc_attr_deprecated("Not needed to implement: sfree(salloc_vec4_t *, void *)") {
  /* Not needed to implement */
  (void)allocator;
  (void)pointer;
}
libsalloc_attr_veccall_overload static inline void
    sfree(register salloc_vec4_t * const restrict allocator);

libsalloc_attr_veccall_const static inline uintptr_t
    salloc_align_forward(register const uintptr_t pointer, register const size_t align);

#endif /* __SALLOC_H__ */
