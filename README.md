# SALLOC

Static memory allocator.

> Original idea by Sergey Shevchuk (Nachos836);

> Implementation by Taras Maliukh (Iipal);

## General Idea

Header-only library with malloc-like behavior for mapping\allocation memory on static buffer, so all work with memory mostly can be optimized and calculated at compile-time.

## Internal

More details of everything documented in code.

Code logic based on [this](https://cs.wellesley.edu/~cs240/s19/slides/malloc.pdf) document, which explains everything about most common malloc-like allocator implementations. Details:

- Implemented using **implicit bidirectional free-list** aka **Bidirectional Coalescing: Boundary Tags**.
- Each allocation takes additional 16bytes in the static buffer for **Boundary Tags** info.
  - In code, each **Boundary Tag** is described in the `__s_tag_t` structure
- Each allocation aligned by 16bytes by default. (24 will align to 32)
- Each allocation will use **Best Fit** method to find a free block. Best Fit: Search the list, choose the best free block: fits, with the fewest bytes leftover.
- On `sfree` call implemented with **Bidirectional Coalescing with preceding free blocks**.
- Optional nullability checks (`_Nonnull` and `_Nullable`) with `SALLOC_NULLABILITY`.
- All code formatted via `clang-format` (version 12.0).
- No dependency on any library, even on standard **libc**.
  - NOTE: If you define `SALLOC_DEBUG` before `salloc.h` include then it's will include also `stdio.h` and `salloc_trace` will use `printf`.

> Originally it's written for `clang` only, because of its features.

> All macroses, types or functions with prefixes: `__s_`, `__st_`, `__s2c`, `__sattr_`, `__salloc_` or `__sfree_` are created only for internal-use purposes.
