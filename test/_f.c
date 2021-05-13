// #define SALLOC_UNSAFE_MAPPING
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
  uint8_t * restrict chunk1     = salloc(&slc, chunk_size + 666);
  uint8_t * restrict chunk2     = salloc(&slc, chunk_size + 999);
  uint8_t * restrict chunk3     = salloc(&slc, chunk_size * 5);
  uint8_t * restrict chunk4     = salloc(&slc, chunk_size * 10);
  uint8_t * restrict chunk5     = salloc(&slc, chunk_size * 16);

  printf("ALL CHUNKS NON-FREED\n");
  salloc_trace(&slc);

  printf("\nAFTER FREED 2 and 4 CHUNKS\n");
  sfree(&slc, chunk2);
  sfree(&slc, chunk4);
  salloc_trace(&slc);

  printf("\nAFTER FREED 3 CHUNK\n");
  sfree(&slc, chunk3);
  salloc_trace(&slc);

  printf("\nAFTER FREED 5\n");
  sfree(&slc, chunk5);
  salloc_trace(&slc);

  printf("\nAFTER FREED 1\n");
  sfree(&slc, chunk1);
  salloc_trace(&slc);

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
