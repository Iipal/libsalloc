#define SALLOC_DEBUG
#include "../salloc.h"

int main(void) {
  salloc_new_fast(, 4096);

  for (size_t i = 0; 10 > i; ++i) {
    void * chunk = salloc(&_slc, SALLOC_MIN_ALLOC_SIZE);

    sfree(&_slc, chunk);
  }
}
