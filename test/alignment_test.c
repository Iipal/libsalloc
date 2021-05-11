#include "../salloc.h"

static inline void printer(void) {
  size_t       start        = 1;
  _Bool        is_start_set = 0;
  const size_t max          = __sc_align_size(2345);

  for (size_t i = 1; max >= i; ++i) {
    const size_t aligned = __sc_align_size(i);
    if (is_start_set && !(i % aligned)) {
      printf("[%4zu...%-4zu] -> %zu\n", start, aligned, aligned);
      is_start_set = 0;
    } else if (!is_start_set) {
      start        = i;
      is_start_set = 1;
    }
  }
}

int main(void) {
  printer();
  // const size_t max = __sc_align_size(2345);
  // size_t       d   = 0;
  // for (size_t i = 0; max >= i; ++i) {
  //   d = __sc_align_size(i);
  // }
}

/*
[     0     ] -> 0
[   1...16  ] -> 16
[  17...32  ] -> 32
[  33...48  ] -> 48
[  49...64  ] -> 64
[  65...80  ] -> 80
...
*/
