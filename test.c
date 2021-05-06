#include "salloc.h"

int main(void) {
  static uint_least8_t buffer[4096];

  salloc_vec_t allocator = salloc_init(buffer, sizeof buffer / sizeof *buffer);

  const uint_fast16_t chunk_1_size = 256, chunk_2_size = 255, chunk_3_size = 96;

  uintptr_t * const restrict chunk_1 = salloc(&allocator, chunk_1_size);
  uintptr_t * const restrict chunk_2 = srealloc(&allocator, 0, 0, chunk_2_size, 1);
  uintptr_t * const restrict chunk_3 = salloc(&allocator, chunk_3_size);

  for (uint_fast16_t byte = chunk_1_size; byte-- > 0;) {
    byte from[chunk_1] = 42;
  }

  for (uint_fast16_t byte = chunk_1_size; byte-- > 0;) {
    byte from[chunk_2] = 42;
  }

  for (uint_fast16_t byte = chunk_1_size; byte-- > 0;) {
    byte from[chunk_3] = 42;
  }

  sfree(&allocator);
  sfree(&allocator, NULL);
}
