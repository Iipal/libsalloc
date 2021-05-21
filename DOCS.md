# SALLOC DOCUMENTATIONS

## Why it's here and not in code?
Because in code with time it's makes everything harder to navigate. Still it's doesn't mean that code has no comments at all, at least types are always described as well as functionality.

## Navigation

- [Introduction](#introduction)
- [Configuration](#configuration)
- [Attributes](#attributes)
- [Types](#types)
- [Public Types](#public-types)
- [Public Macroses](#public-macroses)
- [Macroses](#macroses)
- [S-Allocators](#s-allocators)
- [Additional Functional](#additional-functional)
- [GDI](#gdi)
- [End of Docs](#end-of-docs)

## Introduction

Almost every internal-use-only [macroses](#macroses)\\[attributes](#attributes)\\[types](#types), and even some functions are specified with prefix `__s`, and almost everything are un-defined(with `#undef` of course) at the end of `salloc.h`, but still [types](#types) and function can't be un-defined, because of this - it's not recommended to use anything from `salloc.h` with prefix `__s` outside of it.


Code logic based on [this](https://cs.wellesley.edu/~cs240/s19/slides/malloc.pdf) document, which explains everything about most common malloc-like allocator implementations.

## Configuration

You can specify a few settings before include `salloc.h`:

 - `SALLOC_MACROS_AFTER_USE`: for use [internal macroses](#macroses) outside of `salloc.h` (This setting will not un-define them).
 - `SALLOC_ATTRS_AFTER_USE`: for use [attributes](#attributes) outside of `salloc.h`.
 - `SALLOC_NULLABILITY`: enables clang [nullability checks](https://clang.llvm.org/docs/analyzer/developer-docs/nullability.html) extension. They are can still be accessible if `SALLOC_MACROS_AFTER_USE` is defined.
 - `SALLOC_DEBUG`: enables `salloc_trace` function and includes `stdio.h`.
 - `SALLOC_GDI_BUFFER`: more about this setting [here](#gdi).

## Attributes

This implementation are highly depended on `clang` compiler because of it's attributes, such as `overloadable`.

Available attributes macros (most of them are links for attributes description):
 - [__sattr_diagnose_if](https://clang.llvm.org/docs/AttributeReference.html#diagnose-if)
 - `__sattr_diagnose_align(x, align)`: uses `__sattr_diagnose_if` to check is given `x`-value bigger or aligned by `align`.
 - [__sattr_veccall](https://clang.llvm.org/docs/AttributeReference.html#vectorcall)
 - `__sattr_const`: It's not currently described in clang docs, but here is discussion about [pure/const attributes in different compilers](https://stackoverflow.com/questions/2798188/pure-const-function-attributes-in-different-compilers)
 - [__sattr_overload](https://clang.llvm.org/docs/AttributeReference.html#overloadable)
 - [__sattr_flatten](https://clang.llvm.org/docs/AttributeReference.html#flatten)
 - [__sattr_munused](https://clang.llvm.org/docs/AttributeReference.html#maybe-unused-unused)
 - `__sattr_packed`: It's not currently described in clang docs, but here is [explanation](https://www.keil.com/support/man/docs/armclang_ref/armclang_ref_chr1393328521340.htm) about it works on keil.com.
 - [__sattr_deprecated](https://clang.llvm.org/docs/AttributeReference.html#deprecated): Currently unused.
 - `__sattr_veccall_const`: shortcut for `__sattr_veccall __sattr_const`
 - `__sattr_veccall_const_overload`: shortcut for `__sattr_veccall __sattr_const __sattr_overload`
 - `__sattr_veccall_overload`: shortcut for `__sattr_veccall __sattr_overload`
 - `__sattr_flatten_veccall_overload`: shortcut for `__sattr_flatten __sattr_veccall __sattr_overload`
 - `__sattr_flatten_veccall`: shortcut for `__sattr_flatten __sattr_veccall`

## Types
 - `__s_size_t`: analog of `size_t` from `stddef.h`;
 - `__s_ssize_t`: analog of `ssize_t` from `stddef.h`;
 - `__s_uintptr_t`: analog of `uintptr_t` from `stdint.h`;
 - `__s_uint8_t`: analog of `uint8_t` from `stdint.h`;
 - `__s_ptr_t`: most common pointer for internal-use: `__s_uint8_t * restrict`;
 - `__s_cptr_t`: most common pointer for internal-use, but const: `__s_uint8_t * const restrict`;
 - `__s_salloc_tag`: Commentary from code:
```c
/**
 * Memory mapping with bidirectional implicit free list or Boundary Tags.
 *
 * By calling salloc with \c size of 48 bytes it actually allocating \c size +
 * \c (sizeof(__s_tag_t)*2) bytes for memory mapping:
 *
 * [ __s_tag_t with ->size == 48 and by default ->busy == 1] **header**
 * [                your 48 bytes of memory                ] **payload**
 * [ __s_tag_t with ->size == 48 and by default ->busy == 1] **footer**
 */
```

## Public Types
 - `salloc_t`: General struct for storing available static buffer memory. Commentary from code:
```c
/**
 * Object to which mapped the static buffer via \c salloc_new, and which tracking all the
 * available memory to use in current static buffer.
 */
typedef struct s_salloc_t {
  __s_ptr_t start;  /* start   of available space in static buffer */
  __s_ptr_t end;    /* end     of available space in static buffer */
  __s_ptr_t cursor; /* current max of memory used in static buffer */
} salloc_t;
```

## Public Macroses
 - `SALLOC_MIN_ALLOC_SIZE`: Minium allocation size in static buffer because it's also default alignment.
 - `SALLOC_MIN_BUFFER_SIZE`: Minimum required memory in static buffer for at least 1 pointer with at least `SALLOC_MIN_ALLOC_SIZE` bytes size.
 - `salloc_new_fast(name, capacity)`: Fast shorthand for creating a buffer with size of `capacity` prefixed with `name` and `salloc_t` object for s-allocators. It's creates:
   - `static __s_uint8_t name##_buff[(capacity)];`
   - `const __s_size_t name##_buff_capacity = (capacity);`
   - `salloc_t name##_slc = salloc_new(name##_buff, name##_buff_capacity);`

## Macroses

 - `__s2c`: Most convenient type conversions. `__s2c` prefix stands as shortcut for `__salloc_to_cast`. They are pretty straightforward, so I can just put their definition here as self-explanation about each of them:
   - `__s2c_uiptr(x) ((__s_uintptr_t)(x))`
   - `__s2c_ptr(x)   ((__s_ptr_t)(x))`
   - `__s2c_tag(x)   ((__s_tag_t *)(void *)(x))`

 - `__st`: For work with memory Boundary Tags(`__s_tag_t`). `__st` prefix stands as shortcut for `__salloc_tag`;
   - `__st_init(size, busy)`: Initializer for `__s_tag_t`.
   - `__st_align_default`: Shortcut for `SALLOC_MIN_ALLOC_SIZE` which is default alignment;
   - `__st_align_size(x)`: Forward-Align `x` by `__st_align_default`; Example: By default, on x86_64, 8 will aligned to 16 bytes. 24->32 and so on. That means that default `SALLOC_MIN_BUFFER_SIZE` value in most cases is equal to 32.
   - `__st_size`: Size of one boundary tag.
   - `__st_bd_size`: Size of two boundary tags. bd stands for bi-directional.
   - `__st_ptr_get_tag(x)`: Used to get the Boundary Tag data(`__s_tag_t*`) from the `void* x` .
   - `__st_tag_get_ptr(x)`: Used to return a valid pointer to user. Just like [s-allocators](#s-allocators) do.
   - `__st_busy`: constant 1. Used to indicate is memory chunk is busy (not freed) or not.
   - `__st_not_busy`: constant 0.
   - `__st_get_busy(x)`: gets from `x` busy-indicator (perhaps that `x` is pointer type).
   - `__st_get_size(x)`: gets from `x` size of memory chunk (perhaps that `x` is pointer type).
   - `__st_is_free(x) !__st_get_busy(x)`: Returns true is memory chunk was freed.

## S-Allocators

***

### `void *salloc(salloc_t *__s, __s_size_t __size)`
Allocates new static pointer in `__s` with at least `__size` bytes, and returns it.

#### Params
 - `__s`: a pointer to `salloc_t` object.
 - `__size`: size in bytes.

#### Returns
 - A valid pointer with at least `__size` bytes in static buffer mapped to `__s` object.
 - `NULL`, if:
   - `__s` is NULL;
   - `__size` is equals to 0;
   - static buffer in `__s` has no available memory;

#### Overloads
 - [`void *salloc(salloc_t *__s, __s_size_t __size, __s_size_t __nmemb)`](#void-sallocsalloc_t-__s-__s_size_t-__size-__s_size_t-__nmemb): Allocates new static pointer in `__s` for an array of `__nmemb` elements of `__size` bytes each, and returns it.

***

### `void *salloc(salloc_t *__s, __s_size_t __size, __s_size_t __nmemb)`
Allocates new static pointer in `__s` for an array of `__nmemb` elements of `__size` bytes each, and returns it.

#### Params
 - `__s`: a pointer to `salloc_t` object.
 - `__size`: size of each element in bytes.
 - `__nmemb`: N-elements.

#### Returns
 - A valid pointer with at least `__nmemb * __size` bytes in static buffer mapped to `__s` object.
 - `NULL`, if:
   - `__s` is NULL;
   - `__nmemb * __size` is equals to 0;
   - static buffer in `__s` has no available memory;

#### Overloads
 - [`void *salloc(salloc_t *__s, __s_size_t __size)`](#void-sallocsalloc_t-__s-__s_size_t-__size): Allocates new static pointer in `__s` with at least `__size` bytes, and returns it.

***

### `sfree(salloc_t *__s, void *__ptr)`
Free a `__ptr`. This function marking up space that was allocated for `__ptr` as free, and will try to optimize a static buffer in `__s`.

> A `__ptr` will be a valid pointer after this call of `sfree`, but data under it can possibly be overwritten with the next calls of s-allocators.

#### Params
- `__s`: a pointer to `salloc_t` object.
- `__ptr`: a pointer to be freed.

#### Overloads
 - [`sfree(void *__ptr)`](#sfreevoid-__ptr): Free without optimization.

***

### `sfree(void *__ptr)`
__Un-safe__ free of `__ptr`. This function only marking up space that was allocated for `__ptr` as free.

> This function will not optimize a static buffer that you set with `salloc_new` at all. Use it on your risk or use `sfree` with `salloc_t` as the first parameter. A `__ptr` will be a valid pointer after this call of `sfree` , but data under it can possibly be overwritten with the next calls of s-allocators.

#### Params
 - `__ptr`: a pointer to be freed.

#### Overloads
 - [`sfree(salloc_t *__s, void *__ptr)`](#sfreesalloc_t-__s-void-__ptr): Free and optimize.

***
***
***

## Additional Functional

An additional functional which makes `salloc`-allocators a bit different in use compared to `malloc`:

***

### `salloc_t salloc_new(void *buff, __s_size_t capacity)`
Creating a new static buffer to use by `s-allocators` with at most capacity bytes.

#### Params
 - `buff`: a pointer to static buffer.
 - `capacity`: a `buff` length \ size \ capacity.

#### Returns
   new `salloc_t` object to work with `s-allocators`.

#### Overloads
 - [`salloc_new(void *buff, __s_size_t capacity, salloc_t *__s)`](#salloc_newvoid-buff-__s_size_t-capacity-salloc_t-__s): Instead or creating new object, you can just give it as 3-rd argument and it will re-initialized with given `buff` and `capacity`.

***

### `salloc_new(void *buff, __s_size_t capacity, salloc_t *__s)`
Creating a new static buffer to use by `s-allocators` with at most capacity bytes at give `__s` object.

#### Params
 - `buff`: a pointer to static buffer.
 - `capacity`: a `buff` length \ size \ capacity.
 - `__s`: a pointer to `salloc_t` object

#### Overloads
 - [`salloc_t salloc_new(void *buff, __s_size_t capacity)`](#salloc_t-salloc_newvoid-buff-__s_size_t-capacity): The base, in `__s` will be stored return value of this function.

***

### `salloc_delete(salloc_t *__s)`
Deleting created by `salloc_new` object.

> This is not actually deleting the object. All pointers are still valid and all the data under them will be kept as well after calling the `salloc_delete` . It's only removing the markup of memory chunks used by `s-allocators`. After this call your static `buff` what you set with `salloc_new` will now have additionally free 16 bytes at both sides (before and after the pointer) of each allocated memory pointers with `salloc` .

#### Params
 - `__s`: a pointer to `salloc_t` object

***

<br />

`#ifdef SALLOC_DEBUG`
### `salloc_trace(salloc_t *__s)`
Just prints \ trace all the pointers stored in `salloc_t` object.

#### Params
 - `__s`: a pointer to `salloc_t` object

`#endif`

<br />

***

### `__s_size_t salloc_capacity(salloc_t *__s)`
Returns size\capacity of static buffer.

#### Params
 - `__s`: a pointer to `salloc_t` object

#### Returns
capacity of static buffer mapped in `__s`.

***

### `__s_size_t salloc_used(salloc_t *__s)`
Returns size of used memory in static buffer.

#### Params
 - `__s`: a pointer to `salloc_t` object

#### Returns
 - `0`: no used memory.
 - size of used memory.

***

### `__s_ssize_t salloc_unused(salloc_t *__s)`
Returns size of unused memory in static buffer.

#### Params
 - `__s`: a pointer to `salloc_t` object

#### Returns
 - `0`: no unused memory.
 - __Positive__: size of available\unused memory.
 - __Negative__: somehow in static buffer are more allocated memory than buffer can fit.

#### Overloads
 - [`__s_ssize_t salloc_unused(salloc_t *__s, __s_size_t __size)`](#__s_ssize_t-salloc_unusedsalloc_t-__s-__s_size_t-__size): Checks is in `__s` is enough space to allocate new pointer with at least `__size` bytes.
 - [`__s_ssize_t salloc_unused(salloc_t *__s, __s_size_t __size, __s_size_t __nmemb)`](#__s_ssize_t-salloc_unusedsalloc_t-__s-__s_size_t-__size-__s_size_t-__nmemb): Checks is in `__s` is enough space to allocate of `__nmemb` new pointers with at least `__size` bytes each.

***

### `__s_ssize_t salloc_unused(salloc_t *__s, __s_size_t __size)`
Checks is in `__s` is enough space to allocate new pointer with at least `__size` bytes.

#### Params
 - `__s`: a pointer to `salloc_t` object
 - `__size`: required size of s-allocation.

#### Returns
 - `0`: new s-allocation with at least `__size` will take all the unused memory.
 - __Negative__: the size of how much the pointer with at least `__size` will exceed the unused memory.
 - __Positive__: available space after s-allocation of the new pointer with at least `__size` bytes.

#### Overloads
 - [`__s_ssize_t salloc_unused(salloc_t *__s)`](#__s_ssize_t-salloc_unusedsalloc_t-__s): Returns size of unused memory in static buffer.
 - [`__s_ssize_t salloc_unused(salloc_t *__s, __s_size_t __size, __s_size_t __nmemb)`](#__s_ssize_t-salloc_unusedsalloc_t-__s-__s_size_t-__size-__s_size_t-__nmemb): Checks is in `__s` is enough space to allocate of `__nmemb` new pointers with at least `__size` bytes each.

***

### `__s_ssize_t salloc_unused(salloc_t *__s, __s_size_t __size, __s_size_t __nmemb)`
Checks is in `__s` is enough space to allocate of `__nmemb` new pointers with at least `__size` bytes each.

#### Params
 - `__s`: a pointer to `salloc_t` object.
 - `__size`: required size of s-allocation for each pointer.
 - `__nmemb`: N-pointers to check for s-allocation.

#### Returns
 - `0`: new `__nmemb` s-allocations with at least `__size` each will take all the unused memory.
 - __Negative__: the size of how much new `__nmemb` pointers with at
  least `__size` each will exceed the unused memory.
 - __Positive__: available space after s-allocation of `__nmemb` new pointers with at least `__size` bytes each.


#### Overloads
 - [`__s_ssize_t salloc_unused(salloc_t *__s)`](#__s_ssize_t-salloc_unusedsalloc_t-__s): Returns size of unused memory in static buffer.
 - [`__s_ssize_t salloc_unused(salloc_t *__s, __s_size_t __size)`](#__s_ssize_t-salloc_unusedsalloc_t-__s-__s_size_t-__size): Checks is in `__s` is enough space to allocate new pointer with at least `__size` bytes.

***
***
***


## GDI

__Global Data Interface__


This is just an interface above the original [s-allocators](#s-allocators), and [additional functional](#additional-functional) which uses global data interface to access the global static buffer instead of creating and passing the `salloc_t` object manually.

> You can configure size of global static buffer by configure `SALLOC_GDI_BUFFER_SIZE` (default: 4096).

> The main difference is you must to use [`sfree_gdi`](#sfree_gdivoid-__gdi_ptr) to free AND optimize the space in global static buffer.


Everything described in this section works only with `SALLOC_GDI_BUFFER` setting, still it's recommended to use it because for compiler it's harder to optimize, so performance between this functions and default s-allocators may be differ.


Also this section has less comments to the overloaded functions (which is not mentioned in __Overloads__ section of each default [s-allocators](#s-allocators)) because it will refer to the original [s-allocators](#s-allocators) from which all the behavior are __nested__.

***

### `salloc_t *__salloc_get_gdi_buffer()`
The base of GDI, in this function stored the global static buffer and global `salloc_t` object which will be used to pass it to the default [s-allocators](#s-allocators).

#### Returns
Pointer to the global `salloc_t` object.

***

### `void *salloc(__s_size_t __size)`
[`void *salloc(salloc_t *__s, __s_size_t __size)`](#void-sallocsalloc_t-__s-__s_size_t-__size)

***

### `void *salloc(__s_size_t __size, __s_size_t __nmemb)`
[`void *salloc(salloc_t *__s, __s_size_t __size, __s_size_t __nmemb)`](#void-sallocsalloc_t-__s-__s_size_t-__size-__s_size_t-__nmemb)

***

### `sfree_gdi(void *__gdi_ptr)`
[`sfree(salloc_t *__s, void *__ptr)`](#sfreesalloc_t-__s-void-__ptr)

***

### `salloc_delete()`
[`salloc_delete(salloc_t *__s)`](#salloc_deletesalloc_t-__s)

***

### `salloc_trace()`
[`salloc_trace(salloc_t *__s)`](#salloc_tracesalloc_t-__s)

***

### `salloc_capacity()`

Returns capacity of global static buffer.

#### Returns
A `SALLOC_GDI_BUFFER_SIZE` value.

***

### `__s_size_t salloc_used()`
[`__s_size_t salloc_used(salloc_t *__s)`](#__s_size_t-salloc_usedsalloc_t-__s)

***

### `__s_ssize_t salloc_unused()`
[`__s_ssize_t salloc_unused(salloc_t *__s)`](#__s_ssize_t-salloc_unusedsalloc_t-__s)

***

### `__s_ssize_t salloc_unused(__s_size_t __size)`
[`__s_ssize_t salloc_unused(salloc_t *__s, __s_size_t __size)`](#__s_ssize_t-salloc_unusedsalloc_t-__s-__s_size_t-__size)

***

### `__s_ssize_t salloc_unused(__s_size_t __size, __s_size_t __nmemb)`
[`__s_ssize_t salloc_unused(salloc_t *__s, __s_size_t __size, __s_size_t __nmemb)`](#__s_ssize_t-salloc_unusedsalloc_t-__s-__s_size_t-__size-__s_size_t-__nmemb)

***

## End of Docs.
