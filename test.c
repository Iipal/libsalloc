#include "salloc.h"

SALLOC_ATTR_VECCALL_CONST static inline uintptr_t
    align_forward(register const uintptr_t pointer, register const size_t align) {
  register auto modulo = pointer % (typeof(pointer))align;

  if (modulo != 0) {
    return (pointer + (typeof(pointer))align - modulo);
  } else {
    return (pointer);
  }
}

SALLOC_ATTR_VECCALL_OVERLOAD static inline void *
    salloc(register salloc_vec_t * const restrict allocator,
           register const size_t                  size,
           register const size_t                  align) {
  const auto in_allocator    = *allocator;
  const auto current_pointer = in_allocator[BUFFER] + in_allocator[CURRENT_OFFSET];
  const auto relative_offset =
      align_forward(current_pointer, align) - in_allocator[BUFFER];
  const auto current_offset = (typeof(size))relative_offset + size;

  if (current_offset <= (typeof(current_offset))in_allocator[BUFFER_LENGTH]) {
    const typeof(in_allocator)
        out_allocator = {in_allocator[BUFFER],
                         in_allocator[BUFFER_LENGTH],
                         (typeof(in_allocator[PREVIOUS_OFFSET]))relative_offset,
                         (typeof(in_allocator[CURRENT_OFFSET]))current_offset};

    *allocator = out_allocator;

    return __extension__((void *)in_allocator[BUFFER] + relative_offset);
  } else {
    return nullptr;
  }
}

SALLOC_ATTR_VECCALL_OVERLOAD static inline void *
    salloc(register salloc_vec_t * restrict allocator, register const size_t size) {
  return salloc(allocator, size, SALLOC_DEFAULT_ALIGNMENT);
}

SALLOC_ATTR_FLATTEN_VECCALL_OVERLOAD static inline void *
    srealloc(register salloc_vec_t * const restrict allocator,
             register void * const restrict         old_memory,
             register const size_t                  old_size,
             register const size_t                  new_size,
             register const size_t                  align) {
  const auto in_allocator = *allocator;

  if ((old_memory == nullptr) || (old_size == 0)) {
    return __extension__ salloc(allocator, new_size, align);
  } else if ((in_allocator[BUFFER] <= (typeof(in_allocator[0]))old_memory) &&
             ((typeof(in_allocator[0]))old_memory <
              (in_allocator[BUFFER] + in_allocator[BUFFER_LENGTH]))) {
    if ((in_allocator[BUFFER] + in_allocator[PREVIOUS_OFFSET]) ==
        (typeof(in_allocator[0]))old_memory) {
      return old_memory;
    } else {
      return salloc(allocator, new_size, align);
    }
  } else {
    return nullptr;
  }
}

SALLOC_ATTR_FLATTEN_VECCALL_OVERLOAD static inline void *
    srealloc(register salloc_vec_t * const restrict allocator,
             register void * const restrict         old_memory,
             register const size_t                  old_size,
             register const size_t                  new_size) {
  return srealloc(allocator, old_memory, old_size, new_size, SALLOC_DEFAULT_ALIGNMENT);
}

SALLOC_ATTR_VECCALL_CONST static inline salloc_vec_t
    salloc_init(register const void * const restrict backing_buffer,
                register const size_t                backing_buffer_length) {
  const salloc_allocator_t allocator SALLOC_ATTR_MAYBE_UNUSED;

  return (typeof(allocator.vector)){(typeof(allocator.vector[0]))backing_buffer,
                                    (typeof(allocator.vector[0]))backing_buffer_length};
}

SALLOC_ATTR_VECCALL_CONST_OVERLOAD static inline void
    allocator_static_linear_free(register salloc_vec_t * restrict allocator,
                                 void * restrict                  pointer) {
  /* Not needed to implement */
  (void)allocator;
  (void)pointer;
}

SALLOC_ATTR_VECCALL_OVERLOAD static inline void
    allocator_static_linear_free(register salloc_vec_t * const restrict allocator) {
  const auto in_allocator                = *allocator;
  const typeof(*allocator) out_allocator = {
      in_allocator[BUFFER], in_allocator[BUFFER_LENGTH], 0, 0 /* reset all offsets */
  };

  *allocator = out_allocator;
}

#define from

int main(void) {
  static uint_least8_t buffer[4096];

  auto allocator = salloc_init(buffer, sizeof buffer / sizeof *buffer);

  const auto chunk_1_size = 256, chunk_2_size = 255, chunk_3_size = 96;

  uintptr_t * const restrict chunk_1 = salloc(&allocator, chunk_1_size);
  uintptr_t * const restrict chunk_2 = srealloc(&allocator, 0, 0, chunk_2_size, 1);
  uintptr_t * const restrict chunk_3 = salloc(&allocator, chunk_3_size);

  for (auto byte = chunk_1_size; byte-- > 0;) {
    byte from[chunk_1] = 42;
  }

  for (auto byte = chunk_1_size; byte-- > 0;) {
    byte from[chunk_2] = 42;
  }

  for (auto byte = chunk_1_size; byte-- > 0;) {
    byte from[chunk_3] = 42;
  }

  allocator_static_linear_free(&allocator);
}
