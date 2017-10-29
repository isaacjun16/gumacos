# GumacOS

Proyecto de Sistemas Operativos I, realizado por el grupo de CentOS, segundo semestre del 2017 de la carrera de Ingenieria en Sistemas de la Información de la Univerdad Mariano Galvez de Guatemala.

## Contenido

<img align="right" width="400" src="https://drive.google.com/uc?export=download&id=0B2C93DEs9kbBX1RkZTdvb3JXc00" />

* [Pre-requisitos](#pre-requirements)
* [Construcción de Ejecutables y del ISO (Forma manual)](#makemanual)
* [Construcción de Ejecutables y del ISO (Makefile)](#make)
* [Integrantes](#team)
* [Programas](#programs)
<br/><br/><br/><br/><br/><br/><br/>

## <a name="pre-requirements"></a>Pre-requisitos

[Instalar el cross-compiler de este sitio](https://web.archive.org/web/20170620111003/http://wiki.osdev.org/GCC_Cross-Compiler) como pre-requisito de la construcción del ISO, este sistema esta desarrollado para una arquitectura de 32-bit y fue construido en un ambiente de Xubuntu 16.04 LTS.

## <a name="makemanual"></a>Construcción de Ejecutables y del ISO (Forma manual)

Ejecutar los siguientes comandos en Terminal:

1. **Creación del multiboot header:**
  ```bash
  i686-elf-as boot.s -o boot.o
  ```
2. **Compilación del Kernel escrito en C:**
  ```bash
  i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
  ```
3. **Enlazador del multiboot header y el kernel compilado escrito en C:**
  ```bash
  i686-elf-gcc -T linker.ld -o gumacos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
  ```
4. **Armando ISO de arranque:**

  Copiar el archivo **gumacos.bin** al directorio que se muestra a continuación, con el explorador de archivos:

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
  Y ejecutar el comando en la Terminal:

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

## <a name="make"></a>Construcción de Ejecutables y del ISO (Makefile)

Ejecutar los siguientes comandos en Terminal:

1. **Compilación de código fuente:**
  ```bash
  make
  ```
2. **Armando ISO de arranque:**
  ```bash
  make makeiso
  ```
3. **Prueba del ISO generado con QEMU:**
  ```bash
  make run
  ```


## <a name="team"></a>Integrantes

| Nombre | Carné |
|:-------|:------:|
|Ana Fernanda Guerra Alvarez|0900-15-3538|
|Andy Matias Manuel Teletor|0900-13-7773|
|Sergio Cuxil|0900-16-2835|
|Isaac Manuel Teletor|0900-11-3870|


## <a name="programs"></a>Programas

| Programa | Autor |
|:-------|:------:|
| Calculadora **(En desarrollo)** | -- |
| Reproductor de sonido **(En desarrollo)** | -- |
