#define SALLOC_C2X_ATTRS
#include "../salloc.h"

int main(void) {
  salloc_new_fast(, 4096);

  const salloc_size_t chunk_size = 16;
  salloc_ptr_t        chunk1     = salloc(&_slc, chunk_size);

  sfree(chunk1);

  salloc_delete(&_slc);
}
