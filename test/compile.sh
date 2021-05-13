#!/bin/bash

BUILD_TYPE=$1

CC=clang-12

command -v $CC > /dev/null
if [ $? -eq 1 ]
then
    CC=clang
fi

CFLAGS="-Wall -Wextra -Werror -Wunused -Wno-#warnings -std=c2x"
CFLAGS_OPTIMIZE="-march=native -mtune=native -Ofast -pipe -flto -fpic"
CFLAGS_DEBUG="-g -O1"
CFLAGS_ASM="-march=native -mtune=native -Ofast -pipe -S -masm=intel"
CFLAGS_ASM_DEBUG="-g -S -masm=intel"
CFLAGS_SANITIZE="-g -fsanitize=address -static-libsan"

SRC=$2
if [ -z "$SRC" ]
then
    SRC=main.c
fi
OUT=$3
if [ -z "$OUT" ]
then
    OUT=main
fi

case $BUILD_TYPE in
    build)
        $CC $SRC $CFLAGS $CFLAGS_OPTIMIZE -o build
        ;;
    debug)
        $CC $SRC $CFLAGS $CFLAGS_DEBUG -o debug
        ;;
    asm)
        $CC $SRC $CFLAGS $CFLAGS_ASM -o $OUT.S
        ;;
    asm_debug)
        $CC $SRC $CFLAGS $CFLAGS_ASM_DEBUG -o $OUT_dbg.S
        ;;
    sanitize)
        $CC $SRC $CFLAGS $CFLAGS_SANITIZE -o sanitize
        ;;
    *)
        echo "Invalid option `$BUILD_TYPE`"
        echo " Valid are: build debug asm asm_debug sanitize"
        ;;
esac
