# SALLOC

Static memory allocator.

> Original idea by Sergey Shevchuk (Nachos836);
> Implementation by Taras Maliukh (Iipal);

## General Idea

Creating the simplest header-only library with malloc-like behavior for mapping\allocation memory on static buffer, so all work with memory mostly can be optimized and calculated at compile-time.

## Internal

Code logic based on [this](https://cs.wellesley.edu/~cs240/s19/slides/malloc.pdf) document, which explains everything about most common malloc-like allocator implementations. Details:

- Implemented using **implicit bidirectional free-list** or **Bidirectional Coalescing: Boundary Tags**. In code, each **Boundary Tag** is described in the `__s_chunk_t` structure, and macroses with prefix `__sc_` are created only for internal use purposes.
- Each allocation takes additional 16bytes in the static buffer for **Boundary Tags** info.
- Each allocation aligned by 16bytes by default. (24 will align to 32)
- Each allocation will use **Best Fit** method to find a free block. Best Fit: Search the list, choose the best free block: fits, with the fewest bytes leftover.
- On `sfree` call implemented with **Bidirectional Coalescing with preceding free blocks**.
- All code formatted via `clang-format` (version 10.0).
- No dependency on any library, even on standard **libc**.
  - NOTE: If you define `SALLOC_DEBUG` before `salloc.h` include then it's will include also `stdio.h` and `salloc_trace` will actually **print\\trace** used memory in static buffer.

> Originally it's written for `clang` only, because of its features.

## Docs?

An additional functional which makes `salloc`-allocators a bit different in use compared to `malloc`:

- `salloc_t salloc_new(void *buff, size_t buff_length)`: Will create a new `salloc_t` object with mapped to it `buff`- static buffer with at least `buff_length` length.
- `salloc_delete(salloc_t *__s)`: Will remove `salloc`-markup from given `__s` object to re-use all of the static buffer memory in `__s`.

`salloc`-allocators:

- `void *salloc(salloc_t *__s, size_t __size)`: Allocates new static pointer in `__s` with at least `__size` bytes, and returns it.
- `void *salloc(salloc_t *__s, size_t __size, size_t __nmemb)`: Allocates new static pointer in `__s` for an array of `__nmemb` elements of `__size` bytes each, and returns it.
- `sfree(salloc_t *__s, void *__ptr)`: Free a `__ptr`.
- `sfree(void *__ptr)`: Un-safe free of `__ptr`. This implementation is NOT using **Bidirectional Coalescing with preceding free blocks**.
