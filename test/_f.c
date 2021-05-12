#define SALLOC_UNSAFE
#define SALLOC_UNSAFE_MAPPING
// #define SALLOC_MACROS_AFTER_USE

#define _GNU_SOURCE

#include "../salloc.h"
#include <stdio.h>
#include <string.h>

int main(void) {
  static unsigned char  buffer[4096] = {0};
  register const size_t buffer_size  = sizeof(buffer) / sizeof(*buffer);
  salloc_t              slc          = salloc_new(buffer, buffer_size);

  const size_t       chunk_size = 16;
  uint8_t * restrict chunk      = salloc(&slc, chunk_size);
  uint8_t * restrict chunk2     = salloc(&slc, chunk_size);
  uint8_t * restrict chunk3     = salloc(&slc, chunk_size);
  uint8_t * restrict chunk4     = salloc(&slc, chunk_size);
  uint8_t * restrict chunk5     = salloc(&slc, chunk_size);

  printf("ALL CHUNKS NON-FREED\n");
  salloc_trace(&slc);

  sfree(&slc, chunk2);
  sfree(&slc, chunk4);
  printf("\nAFTER FREED 2 and 4 CHUNKS\n");
  salloc_trace(&slc);

  sfree(&slc, chunk3);
  printf("\nAFTER FREED 3 CHUNK\n");
  salloc_trace(&slc);

  sfree(&slc, chunk);
  sfree(&slc, chunk5);
  printf("\nAFTER FREED 1 and 5 CHUNK\n");
  salloc_trace(&slc);

  // uint8_t * restrict chunk1     = salloc(&slc, chunk_size);
  // uint8_t * restrict chunk2     = salloc(&slc, chunk_size);
  // uint8_t * restrict chunk3     = salloc(&slc, chunk_size);
  // uint8_t * restrict chunk4     = salloc(&slc, chunk_size);
  // for (size_t i = 0; 5 > i; ++i) {
  //   const size_t       chunk_size = 16 * (i + 1);
  //   uint8_t * restrict chunk      = salloc(&slc, chunk_size);

  //   for (uint8_t byte = chunk_size; byte-- > 0;) {
  //     byte from[chunk] = 42;
  //   }

  //   sfree(chunk);
  // }

  // salloc_trace(&slc);

  salloc_delete(&slc);
}
