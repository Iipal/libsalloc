# salloc.h TESTS

### (!) Required clang-compiler (!)

```bash
sudo apt install clang
```

Or if your system supports it - you can install clang-12 and the script will use it

### Script test

```bash
chmod u+x compile.sh
./compile.sh
```

### Usage
Compile `main.c` to `debug` executable:
```bash
> ./compile.sh debug
```

Compile `SALLOC_DEFAULT_ALIGNMENT.c` to `build` executable:
```bash
> ./compile.sh build SALLOC_DEFAULT_ALIGNMENT.c
```

### Options:

Default flags are: `-Wall -Wextra -Werror -Wunused -Wno-#warnings -std=c2x`

- 3 argument - $OUT; works only for `asm` and `asm_debug` rules; default: main
- 2 argument - $SRC; default main.c
- 1 argument - $BUILD_TYPE; compiles $SRC with:
  - build: default + `-march=native -mtune=native -Ofast -pipe -flto -fpic` to `./build` executable
  - debug: default + `-g -O1` to `./debug` executable
  - asm: default + `-march=native -mtune=native -Ofast -pipe -S -masm=intel` `$OUT.S ` file
  - asm_debug: default + `-g -S -masm=intel` `$OUT_dbg.S ` file
  - sanitize: default + `-g -fsanitize=address -static-libsan` to `./sanitize` executable
