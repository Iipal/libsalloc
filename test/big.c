// #define SALLOC_MACROS_AFTER_USE
#define SALLOC_DEBUG

#include "../salloc.h"
#include <stdio.h>

int main(void) {
  static unsigned char  buffer[4096] = {0};
  register const size_t buffer_size  = sizeof(buffer) / sizeof(*buffer);
  salloc_t              slc          = salloc_new(buffer, buffer_size);

  const size_t chunk_size = 1024;
  void *       chunk1     = salloc(&slc, chunk_size);
  void *       chunk2     = salloc(&slc, chunk_size);
  void *       chunk3     = salloc(&slc, chunk_size);
  void *       chunk4     = salloc(&slc, chunk_size - (SALLOC_EACH_ALLOC_OVERHEAD * 4));
  // the last one can only be this size, because of boundary tags mapping + tags for the
  // last pointer itself

  printf("1: %p, 2: %p, 3: %p, 4: %p\n", chunk1, chunk2, chunk3, chunk4);

  salloc_trace(&slc);

  salloc_delete(&slc);
}
