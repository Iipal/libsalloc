#define SALLOC_GDI_BUFFER
#include "../salloc.h"

int main(void) {
  for (size_t i = 0; 10 > i; ++i) {
    void * chunk = salloc(SALLOC_MIN_ALLOC_SIZE * (i + 1));

    sfree_gdi(chunk);
  }
}
