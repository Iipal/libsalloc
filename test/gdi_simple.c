#define SALLOC_GDI_BUFFER
#include "../salloc.h"

int main(void) {
  const size_t chunk_size = 16;
  void *       chunk      = salloc(chunk_size);

  sfree_gdi(chunk);
}
