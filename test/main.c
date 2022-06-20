// #define SALLOC_MACROS_AFTER_USE
#define SALLOC_DEBUG

#include "../salloc.h"
#include <stdio.h>

int main(void) {
  salloc_new_fast(, 4096);

  const size_t chunk_size = 16;
  void *       chunk1     = salloc(&_slc, chunk_size);
  void *       chunk2     = salloc(&_slc, chunk_size);
  void *       chunk3     = salloc(&_slc, chunk_size);
  void *       chunk4     = salloc(&_slc, chunk_size);
  void *       chunk5     = salloc(&_slc, chunk_size);

  // printf("ALL CHUNKS NON-FREED\n");
  salloc_trace(&_slc);

  // printf("\nAFTER FREED 2 and 4 CHUNKS\n");
  sfree(&_slc, chunk2);
  sfree(&_slc, chunk4);
  salloc_trace(&_slc);

  // printf("\nAFTER FREED 3 CHUNK\n");
  sfree(&_slc, chunk3);
  salloc_trace(&_slc);

  for (size_t i = 0; 6 > i; ++i) {
    const size_t new_chunk_size = chunk_size * (i + 1);
    // printf("\nCREATING NEW CHUNK WITH %zu bytes\n", new_chunk_size);
    void * new_chunk = salloc(&_slc, new_chunk_size);
    salloc_trace(&_slc);

    // printf("\nFREEING NEW CHUNK\n");
    sfree(&_slc, new_chunk);
    salloc_trace(&_slc);
  }

  // printf("\nAFTER FREED 5\n");
  sfree(&_slc, chunk5);
  salloc_trace(&_slc);

  // printf("\nAFTER FREED 1\n");
  sfree(&_slc, chunk1);
  salloc_trace(&_slc);

  salloc_delete(&_slc);
}
