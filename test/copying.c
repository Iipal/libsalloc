#define SALLOC_DEBUG
#include "../salloc.h"

#define BUFF_SIZE 12288

int main(void) {
  static salloc_buffer_t buff[BUFF_SIZE];

  salloc_ptr_t        unmapped_buff          = buff;
  const salloc_size_t unmapped_buff_capacity = 4096;

  salloc_ptr_t        mapped_buff          = buff + unmapped_buff_capacity;
  const salloc_size_t mapped_buff_capacity = BUFF_SIZE - unmapped_buff_capacity;

  salloc_t unmapped_slc = salloc_new(unmapped_buff, unmapped_buff_capacity);
  salloc_t mapped_slc   = salloc_new(mapped_buff, mapped_buff_capacity);

  salloc_ptr_t mapped_ptr = salloc(&mapped_slc, 16);
  salloc_ptr_t big_chunk =
      salloc(&mapped_slc,
             SALLOC_MIN_ALLOC_SIZE *
                 500); // used later only for demonstration of "smart" salloc_copy
  (void)big_chunk;

  salloc_ptr_t unmapped_ptr = salloc_copy(&unmapped_slc,
                                          "it's an unmapped buffer ptr",
                                          sizeof("it's an unmapped buffer ptr"));

  printf("!! unmapped_slc:\n");
  salloc_trace(&unmapped_slc);
  printf("\nunmapped_ptr: `%s`\n\n", unmapped_ptr);

  printf("!! mapped_slc:\n");
  salloc_trace(&mapped_slc);

  printf("\n-----------------------------------------------------\n"
         "copying all the data from mapped_slc to unmapped_slc:"
         "\n-----------------------------------------------------\n");
  salloc_copy(&unmapped_slc, &mapped_slc);

  printf("\n!! unmapped_slc:\n");
  salloc_trace(&unmapped_slc);

  printf("\n!! mapped_slc:\n");
  salloc_trace(&mapped_slc);

  sfree(mapped_ptr);
  salloc_delete(&unmapped_slc);
  salloc_delete(&mapped_slc);
}
