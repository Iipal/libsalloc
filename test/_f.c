// #define SALLOC_MACROS_AFTER_USE
#define SALLOC_DEBUG

#include "../salloc.h"
#include <stdio.h>

int main(void) {
  static unsigned char      buffer[4096] = {0};
  register const __s_size_t buffer_size  = sizeof(buffer) / sizeof(*buffer);
  salloc_t                  slc          = salloc_new(buffer, buffer_size);

  const __s_size_t chunk_size = 16;
  __s_ptr_t        chunk1     = salloc(&slc, chunk_size + 666);
  __s_ptr_t        chunk2     = salloc(&slc, chunk_size + 999);
  __s_ptr_t        chunk3     = salloc(&slc, chunk_size * 5);
  __s_ptr_t        chunk4     = salloc(&slc, chunk_size * 10);
  __s_ptr_t        chunk5     = salloc(&slc, chunk_size * 16);

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

  // for (__s_size_t i = 0; 5 > i; ++i) {
  //   const __s_size_t       chunk_size = 16 * (i + 1);
  //   __s_ptr_t chunk      = salloc(&slc, chunk_size);

  //   for (__s_uint8_t byte = chunk_size; byte-- > 0;) {
  //     byte from[chunk] = 42;
  //   }

  //   sfree(chunk);
  // }

  // salloc_trace(&slc);

  salloc_delete(&slc);
}
