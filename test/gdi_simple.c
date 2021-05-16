#define SALLOC_GDI_BUFFER
#include "../salloc.h"

int main(void) {
  const __s_size_t chunk_size = 16;
  __s_ptr_t        chunk      = salloc(chunk_size);

  sfree_gdi(chunk);
}
