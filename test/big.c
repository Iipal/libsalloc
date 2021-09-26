// #define SALLOC_MACROS_AFTER_USE
#define SALLOC_DEBUG

#include "../salloc.h"
#include <stdio.h>

int main(void) {
  static salloc_buffer_t       buffer[4096] = {0};
  register const salloc_size_t buffer_size  = sizeof(buffer) / sizeof(*buffer);
  salloc_t                     slc          = salloc_new(buffer, buffer_size);

  const salloc_size_t chunk_size = 1024;
  salloc_ptr_t        chunk1     = salloc(&slc, chunk_size);
  salloc_ptr_t        chunk2     = salloc(&slc, chunk_size);
  salloc_ptr_t        chunk3     = salloc(&slc, chunk_size);
  salloc_ptr_t        chunk4 = salloc(&slc, 960); // the last one can only be this size

  printf("1: %p, 2: %p, 3: %p, 4: %p\n", chunk1, chunk2, chunk3, chunk4);

  salloc_trace(&slc);

  salloc_delete(&slc);
}
