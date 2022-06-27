#define SALLOC_DEBUG
#include "../salloc.h"

typedef struct {
  int    a;
  int    c;
  char * b;
} test_t;

int main(void) {
  const size_t         __buff_size = SALLOC_CALC_BUFF_SIZE(sizeof(test_t), 10);
  static unsigned char __buff[__buff_size];
  salloc_t             slc = salloc_new(__buff, __buff_size);

  test_t * restrict ptr1 = salloc(&slc, sizeof(test_t));

  ptr1->a = 10;
  ptr1->c = 20;
  ptr1->b = "string";

  printf("ptr1: %d - %s - %d\nallocated and salloc_delete called;\n\n",
         ptr1->a,
         ptr1->b,
         ptr1->c);

  salloc_delete(&slc);

  test_t * restrict ptr2 = salloc(&slc, sizeof(test_t));
  ptr2->a                = 11;
  ptr2->c                = 22;
  ptr2->b                = "strins";

  printf("ptr1: %d - %s - %d\n"
         "ptr2: %d - %s - %d\n\n"
         "ptr2 alloced after salloc_delete, on the same memory chunk, an it's ovverides "
         "ptr1",
         ptr1->a,
         ptr1->b,
         ptr1->c,
         ptr2->a,
         ptr2->b,
         ptr2->c);
}
