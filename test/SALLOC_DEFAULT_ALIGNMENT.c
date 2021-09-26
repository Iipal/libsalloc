#define SALLOC_DEFAULT_ALIGNMENT (sizeof(void *) * 1024) // default is (sizeof(void*) * 2)
#define SALLOC_DEBUG
#include "../salloc.h"

// USE macroses instead of `const salloc_size_t ...` to avoid any kind of VLA definitions
#define TEST_N_ELEMENTS 5

int main(void) {
  // sda stands for shortcut for this setting test
  salloc_new_fast(sda, SALLOC_MIN_BUFFER_SIZE * TEST_N_ELEMENTS);
  salloc_ptr_t tests[TEST_N_ELEMENTS] = {0};

  for (salloc_size_t i = 0; TEST_N_ELEMENTS > i; ++i) {
    tests[i] = salloc(&sda_slc, SALLOC_MIN_ALLOC_SIZE);
  }

  printf("SALLOC_EACH_ALLOC_OVERHEAD: %zu\n", SALLOC_EACH_ALLOC_OVERHEAD);
  printf("SALLOC_DEFAULT_ALIGNMENT: %zu\n\n", SALLOC_DEFAULT_ALIGNMENT);
  printf("SALLOC_MIN_BUFFER_SIZE: %zu\n", SALLOC_MIN_BUFFER_SIZE);
  printf("SALLOC_MIN_ALLOC_SIZE: %zu\n\n", SALLOC_MIN_ALLOC_SIZE);

  salloc_trace(&sda_slc);
}
