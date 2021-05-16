#include "../salloc.h"

int main(void) {
  static __s_uint8_t buff[4096];
  salloc_t           slc = salloc_new(buff, 4096);

  for (__s_size_t i = 0; 10 > i; ++i) {
    __s_ptr_t chunk = salloc(&slc, SALLOC_MIN_ALLOC_SIZE * (i + 1));

    sfree(&slc, chunk);
  }
}
