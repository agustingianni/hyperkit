#!/usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset

echo "Building kernel.elf ..."

CFLAGS="-std=c17 -Wall -ffreestanding -I include"

nasm -felf32 -w+all start.s
i686-elf-gcc $CFLAGS -c kernel.c
i686-elf-gcc $CFLAGS -c string.c
i686-elf-gcc $CFLAGS -c printf.c
i686-elf-gcc $CFLAGS -c bmv.c
i686-elf-gcc $CFLAGS -c acpi.c
i686-elf-gcc $CFLAGS -c processors.c
i686-elf-gcc $CFLAGS -c cpu.c
i686-elf-gcc $CFLAGS -c interrupts.c
i686-elf-gcc $CFLAGS -c timer.c
i686-elf-gcc $CFLAGS -c pic.c

i686-elf-gcc $CFLAGS -nostdlib -O0 \
    -T linker.ld \
    start.o \
    kernel.o \
    string.o \
    printf.o \
    bmv.o \
    acpi.o \
    processors.o \
    cpu.o \
    interrupts.o \
    timer.o \
    pic.o \
    -o kernel.elf \
    -lgcc

i686-elf-objcopy --remove-section .comment kernel.elf

i686-elf-objdump -F -C -w -x -s -f -d -M intel kernel.elf > kernel.s

rm *.o