#define SALLOC_DEBUG
#include "../salloc.h"

#define BUFF_SIZE 12288

/*

static buff       [           12288 bytes          ]
smoll             [ 4096b ]
big                       [       8192 bytes       ]


allocating in big 2 pointers with 16 and 8000 bytes
big [          8192 bytes          ]
->
big [[16][       8000      ]  144  ]

then calling salloc_copy(unmapped)
smoll             [    4096    ]
->
smoll             [[16]  4064  ]

*/

int main(void) {
  static unsigned char buff[BUFF_SIZE];

  unsigned char * restrict smoll = buff;
  const size_t smoll_capacity    = 4096;

  unsigned char * restrict big = buff + smoll_capacity;
  const size_t big_capacity    = BUFF_SIZE - smoll_capacity;

  salloc_t smoll_slc = salloc_new(smoll, smoll_capacity);
  salloc_t big_slc   = salloc_new(big, big_capacity);

  void * mapped_ptr = salloc(&big_slc, 16);
  void * big_chunk  = salloc(&big_slc, SALLOC_MIN_ALLOC_SIZE * 500);

  printf("\n-----------------------------------------------------\n"
         "copying all the data to smoll_slc from big_slc:"
         "\n-----------------------------------------------------\n");
  salloc_copy(&smoll_slc, &big_slc);

  printf("\n!! smoll_slc:\n");
  salloc_trace(&smoll_slc);

  printf("\n!! big_slc:\n");
  salloc_trace(&big_slc);

  sfree(mapped_ptr);
  sfree(big_chunk);
  salloc_delete(&smoll_slc);
  salloc_delete(&big_slc);
}
