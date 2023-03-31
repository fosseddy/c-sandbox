#!/bin/bash

set -e

name="$1"
cwd="/home/fosseddy/programming/c-sandbox/"


if [ "$name" = "mem" ]; then
    gcc -c -o $cwd/mem/mem.o $cwd/mem/mem.c
    ar rcs $cwd/lib/libmem.a $cwd/mem/mem.o
    rm $cwd/mem/mem.o
    cp $cwd/mem/mem.h $cwd/include
fi
