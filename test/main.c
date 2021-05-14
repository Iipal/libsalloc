// #define SALLOC_MACROS_AFTER_USE
#define SALLOC_DEBUG

#include "../salloc.h"
#include <stdio.h>

int main(void) {
  static unsigned char      buffer[4096] = {0};
  register const __s_size_t buffer_size  = sizeof(buffer) / sizeof(*buffer);
  salloc_t                  slc          = salloc_new(buffer, buffer_size);

  const __s_size_t chunk_size = 16;
  __s_ptr_t        chunk1     = salloc(&slc, chunk_size);
  __s_ptr_t        chunk2     = salloc(&slc, chunk_size);
  __s_ptr_t        chunk3     = salloc(&slc, chunk_size);
  __s_ptr_t        chunk4     = salloc(&slc, chunk_size);
  __s_ptr_t        chunk5     = salloc(&slc, chunk_size);

  printf("ALL CHUNKS NON-FREED\n");
  salloc_trace(&slc);

  printf("\nAFTER FREED 2 and 4 CHUNKS\n");
  sfree(&slc, chunk2);
  sfree(&slc, chunk4);
  salloc_trace(&slc);

  printf("\nAFTER FREED 3 CHUNK\n");
  sfree(&slc, chunk3);
  salloc_trace(&slc);

  for (__s_size_t i = 0; 6 > i; ++i) {
    const __s_size_t new_chunk_size = chunk_size * (i + 1);
    printf("\nCREATING NEW CHUNK WITH %zu bytes\n", new_chunk_size);
    __s_ptr_t new_chunk = salloc(&slc, new_chunk_size);
    salloc_trace(&slc);

    printf("\nFREEING NEW CHUNK\n");
    sfree(&slc, new_chunk);
    salloc_trace(&slc);
  }

  printf("\nAFTER FREED 5\n");
  sfree(&slc, chunk5);
  salloc_trace(&slc);

  printf("\nAFTER FREED 1\n");
  sfree(&slc, chunk1);
  salloc_trace(&slc);

  salloc_delete(&slc);
}
