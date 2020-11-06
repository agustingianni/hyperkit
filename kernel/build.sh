#!/usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset

echo "Building kernel.elf ..."

i686-elf-gcc -std=c11 -ffreestanding -c start.s -o start.o
i686-elf-gcc -std=c11 -ffreestanding -c kernel.c -o kernel.o
i686-elf-gcc -std=c11 -ffreestanding -nostdlib -O0 -T linker.ld start.o kernel.o -o kernel.elf -lgcc

echo "OK!"