/* Surely you will remove the processor conditionals and this comment
   appropriately depending on whether or not you use C++. */
#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
int activeShell = 0;
int loading_os = 1;

uint8_t shift = 0;
uint8_t caps = 0;

typedef struct command {
  char* command;
  char* description;
} command_t;

char shell_buffer[200];
uint16_t index_buffer = 0;

command_t commands[10];
uint8_t index_command = 0;

#include "gcentos.h"
#include "string.h"
#include "io.h"
#include "key.h"
#include "memory.h"
// METHODS HEADERS GOES HERE
#include "calc.h"
// METHODS HEADERS END HERE
#include "vga.h"
#include "terminal.h"
#include "shell.h"
#include "keyboard.h"


#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(void) {
	/* Initialize terminal interface */
	terminal_initialize();

	/* Newline support is left as an exercise. */
	terminal_writestring_fancy(bootLoader);
	loading_os--;
	//terminal_writestring(cabecera);
	char c = 0;
	keyboard_init_pics(0x20, 0x28);
	do{
		if(inb(0x60)!=c) {
			c = inb(0x60);
    	if(c>0) {
				keyboard_charshell(c);
			}
		}
	}while(1);
}
