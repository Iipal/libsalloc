#define SALLOC_GDI_BUFFER
#include "../salloc.h"

int main(void) {
  const salloc_size_t chunk_size = 16;
  salloc_ptr_t        chunk      = salloc(chunk_size);

  sfree_gdi(chunk);
}
