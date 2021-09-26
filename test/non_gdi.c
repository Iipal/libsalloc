#include "../salloc.h"

int main(void) {
  static salloc_buffer_t buff[4096];
  salloc_t               slc = salloc_new(buff, 4096);

  for (salloc_size_t i = 0; 10 > i; ++i) {
    salloc_ptr_t chunk = salloc(&slc, SALLOC_MIN_ALLOC_SIZE * (i + 1));

    sfree(&slc, chunk);
  }
}
