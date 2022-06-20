#define SALLOC_DEFAULT_ALIGNMENT 32
#define SALLOC_NULLABILITY
#define SALLOC_DEBUG
#include "../salloc.h"
#include <string.h>

int main(void) {
  salloc_new_fast(, SALLOC_MIN_BUFFER_SIZE);

  void * chunk = salloc(&_slc, SALLOC_MIN_ALLOC_SIZE);

  memcpy(chunk, "testing this !!", SALLOC_MIN_ALLOC_SIZE);
  printf("%s\n", chunk);

  sfree(&_slc, chunk);
  salloc_delete(&_slc);
}
