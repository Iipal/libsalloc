# SALLOC

Static memory allocator.

> Original idea by Sergey Shevchuk (Nachos836);

> Implementation by Taras Maliukh (Iipal);

## General Idea

Creating the simplest header-only library with malloc-like behavior for mapping\allocation memory on static buffer, so all work with memory mostly can be optimized and calculated at compile-time.

## Internal

Code logic based on [this](https://cs.wellesley.edu/~cs240/s19/slides/malloc.pdf) document, which explains everything about most common malloc-like allocator implementations. Details:

- Implemented using **implicit bidirectional free-list** aka **Bidirectional Coalescing: Boundary Tags**.
- Each allocation takes additional 16bytes in the static buffer for **Boundary Tags** info.
- Each allocation aligned by 16bytes by default. (24 will align to 32)
- Each allocation will use **Best Fit** method to find a free block. Best Fit: Search the list, choose the best free block: fits, with the fewest bytes leftover.
- On `sfree` call implemented with **Bidirectional Coalescing with preceding free blocks**.
- Optional nullability checks (`_Nonnull` and `_Nullable`) with `SALLOC_NULLABILITY`.
- All code formatted via `clang-format` (version 12.0).
- No dependency on any library, even on standard **libc**.
  - NOTE: If you define `SALLOC_DEBUG` before `salloc.h` include then it's will include also `stdio.h` and `salloc_trace` will use `printf`.

> Originally it's written for `clang` only, because of its features.

> In code, each **Boundary Tag** is described in the `__s_chunk_t` structure, and macroses, types or functions with prefixes: `__s_`, `__sc_`, `__s2c`, `__sattr_`, `__salloc_` or `__sfree_` are created only for internal-use purposes.

## Docs :?

### `salloc`-like allocators of (**s-allocators**):

- `void *salloc(salloc_t *__s, size_t __size)`: Allocates new static pointer in `__s` with at least `__size` bytes, and returns it.
- `void *salloc(salloc_t *__s, size_t __size, size_t __nmemb)`: Allocates new static pointer in `__s` for an array of `__nmemb` elements of `__size` bytes each, and returns it.

- `sfree(salloc_t *__s, void *__ptr)`: Free a `__ptr`.

- `sfree(void *__ptr)`: Un-safe free of `__ptr`. This implementation is NOT using **Bidirectional Coalescing with preceding free blocks**.

### Additional to `s-alloactors` functional:

An additional functional which makes `salloc`-allocators a bit different in use compared to `malloc`:

- `salloc_t salloc_new(void *buff, size_t buff_length)`: Will create a new `salloc_t` object with mapped to it `buff`- static buffer with at least `buff_length` length.
- `salloc_new_fast(name, capacity)`: Fast shorthand for creating a buffer and `salloc_t` object for s-allocators. See [this macro definition](https://github.com/Iipal/libsalloc/blob/8cf7bd80d61f57422ce3fa183b5b1a62d241ff28/salloc.h#L217) for more info about it works.
- `salloc_delete(salloc_t *__s)`: Will remove `salloc`-markup from given `__s` object to re-use all of the static buffer memory in `__s`.
- `salloc_trace(salloc_t *__s)`: Will print\trace memory usage in static buffer. Works only when `SALLOC_DEBUG` defined.

- `size_t salloc_capacity(salloc_t *__s)`: Returns size\capacity of static buffer.
- `size_t salloc_used(salloc_t *__s)`: Returns size of used memory in static buffer.

- `ssize_t salloc_unused(salloc_t *__s)`: Returns size of unused memory in static buffer.
- `ssize_t salloc_unused(salloc_t *__s, size_t __size)`: Checks is in `__s` is enough space to allocate new pointer with at least `__size` bytes.
- `ssize_t salloc_unused(salloc_t *__s, size_t __size, size_t __nmemb)`: Checks is in `__s` is enough space to allocate of `__nmemb` new pointers with at least `__size` bytes each.

#### SALLOC_GDI_BUFFER

Also, you can use additional overloads of s-allocators via `SALLOC_GDI_BUFFER` pre-definition before `salloc.h` include.

- This macro provides more standard malloc-looks-like s-allocators without passing as the first argument `salloc_t` object and this functions will use `Global Data Interface` to access to global static buffer.

- The only difference - use must use `sfree_gdi(void *__ptr)` instead of `sfree(salloc_t *__s, void *__ptr)` if you want to optimize the space in **GDI static buffer** after freeing the pointers.

- You can also pre-define the `SALLOC_GDI_BUFFER_SIZE` macro for custom size of **GDI static buffer**. Default: 4096;
