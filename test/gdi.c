#define SALLOC_GDI_BUFFER
#include "../salloc.h"

int main(void) {
  for (__s_size_t i = 0; 10 > i; ++i) {
    __s_ptr_t chunk = salloc(SALLOC_MIN_ALLOC_SIZE * (i + 1));

    sfree_gdi(chunk);
  }
}
