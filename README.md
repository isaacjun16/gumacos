# GumacOS

Proyecto de Sistemas Operativos I para el grupo de CentOS del segundo Semestre del 2017 de la carrera de Ingenieria en Sistemas de la Informacion de la Univerdad Mariano Galvez de Guatemala.

## Contenido

<img align="right" width="400" src="https://drive.google.com/uc?export=download&id=0B2C93DEs9kbBX1RkZTdvb3JXc00" />

* [Pre-requisitos](#pre-requirements)
* [Construcción del ISO](#make)
* [Programas](#programs)













## <a name="pre-requirements"></a>Pre-requisitos

[Instalar el cross-compiler de este sitio](https://web.archive.org/web/20170620111003/http://wiki.osdev.org/GCC_Cross-Compiler) como Pre-requisito de la construcción del ISO, este sistema esta desarrollado para una arquitectura de 32-bit y fue construido en un ambiente de Xubuntu 16.04 LTS.

## <a name="make"></a>Construcción del ISO

Ejecutar los siguientes comandos en Terminal:

1. **Creación del multiboot header:**
  ```bash
  i686-elf-as boot.s -o boot.o
  ```
2. **Compilación del Kernel escrito en C:**
  ```bash
  i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
  ```
3. **Enlazador del multiboot header y el kernel escrito en C compilado:**
  ```bash
  i686-elf-gcc -T linker.ld -o gumacos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
  ```
4. **Armando ISO de arranque:**

  Copiar el archivo **gumacos.bin** al directorio que se muestra a continuación:

  ```
  isodir    
  │
  └───boot
  │   │   gumacos.bin
  │   │
  │   └───grub
  │       │   grub.cfg
  │       │
  ```

  ```bash
  grub-mkrescue -o gumacos.iso isodir
  ```
5. **Prueba del ISO generado con QEMU:**
  ```bash
  qemu-system-i386 -cdrom gumacos.iso
  ```

6. **Prueba del iso en hardware verdadero (BOOTABLE USB):**

Cambiar /dev/sdx por el nombre correspodiente a el USB a donde se desea instalar la imagen ISO, tomar en cuenta que toda la data del dispositivo que se especifique se eliminara, por lo que TENER MUCHO CUIDADO AL ESPECIFICAR EL [NOMBRE DEL DISPOSITIVO](https://www.debian.org/releases/etch/sparc/apcs04.html.es).

  ```bash
  sudo dd if=gumacos.iso of=/dev/sdx && sync
  ```

## <a name="programs"></a>Programas

| Programa | Autor |
|:-------|:------:|
| Calculadora **(En desarrollo)** | Integrante 1 |
| Reproductor de sonido **(En desarrollo)** | Integrante 2 |
