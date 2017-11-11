CC = i686-elf-gcc
AS = i686-elf-as
LNFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib boot.o kernel.o memory.o string.o time.o -lgcc
CCFLAGS = -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra

build: acompile ccompile lcompile

runbuild: acompile ccompile lcompile makeiso run

acompile:
		$(AS) boot.s -o boot.o

ccompile:
		$(CC) $(CCFLAGS) kernel.c -o kernel.o
		$(CC) $(CCFLAGS) memory.c -o memory.o
		$(CC) $(CCFLAGS) string.c -o string.o
		$(CC) $(CCFLAGS) time.c -o time.o

lcompile:
		$(CC) $(LNFLAGS) -o gumacos.bin

makeiso:
		cp gumacos.bin isodir/boot/
		grub-mkrescue -o gumacos.iso isodir

run:
		qemu-system-i386 -cdrom gumacos.iso
