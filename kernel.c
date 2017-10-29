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

#include "string.h"
#include "gcentos.h"
#include "io.h"
#include "keyboard.h"
#include "memory.h"
#include "shell.h"
#include "calc.h"

/* MEMORY METHODS */
uint8_t* ptr = (uint8_t*) 3145728;
void* kalloc(size_t size){
	void* ret = (void*) ptr;
	ptr += size;
	ptr+= 2;
	return ret;
}

/*TERMINAL METHODS*/

/* Hardware text mode color constants. */
#define TERMINAL_COMMAND_PORT 0x3D4
#define TERMINAL_DATA_PORT 0x3D5

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t terminal_strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
int activeShell = 0;
int loading_os = 1;

void change_terminal_color(void) {
  terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_set_cursor_position(uint16_t pos){
	outb(TERMINAL_COMMAND_PORT, 14);
	outb(TERMINAL_DATA_PORT, (pos >> 8) & 0x00FF);
	outb(TERMINAL_COMMAND_PORT, 15);
	outb(TERMINAL_DATA_PORT, pos & 0x00FF);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
  terminal_set_cursor_position(terminal_buffer[index]);
}

void terminal_clear(){
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
  const size_t index = terminal_column * VGA_WIDTH + terminal_row;
  terminal_buffer[index] = vga_entry(' ', terminal_color);
  terminal_set_cursor_position(terminal_buffer[index]);
}

void terminal_putchar(char c) {
	if(c == '\n' && activeShell == 0 && loading_os == 0) {
		terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
		terminal_column = 0;
		terminal_row = 0;
		terminal_clear();
	} else if(c == '\n') {
		terminal_row++;
		terminal_column = 0;
    if(terminal_row == VGA_HEIGHT){
      terminal_row = 0;
      terminal_clear();
    }
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
        //terminal_clear();
		}
	}
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_write_fancy(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		char c = data[i];
		if (c == '~') {
			c = '_';
			terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_LIGHT_BROWN);
		} else if (c == '*') {
			c = ' ';
			terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_LIGHT_BROWN);
		} else if ((c == '|') || (c == '/') || (c == '\\')) {
			terminal_color = vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_LIGHT_BLUE);
		} else if (c == '$') {
			terminal_color = vga_entry_color(VGA_COLOR_BLUE, VGA_COLOR_BLUE);
		} else {
			terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
		}
		terminal_putchar(c);
	}
}

void terminal_writestring(const char* data) {
	terminal_write(data, terminal_strlen(data));
}

void terminal_writestring_fancy(const char* data) {
	terminal_write_fancy(data, terminal_strlen(data));
}

/**
	inserta un backspace a la terminal
*/
void terminal_backspace(){
	if(terminal_column > terminal_strlen(terminal_in)) {
		terminal_column--;
		terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
	}
}

/*SHELL METHODS*/

char shell_buffer[200];
uint16_t index_buffer = 0;

command_t commands[10];
uint8_t index_command = 0;

/**
	maneja los comando ingresados al shell
*/
void shell_handle_command() {
	if(index_buffer > 0) {

		char* command_buff = (char*)kalloc(sizeof(char) * (index_buffer + 1));
	  command_buff[index_buffer] = 0;
	  memcpy(shell_buffer, command_buff, index_buffer);
	  int len = strlen(command_buff);

	  int space_index = string_index_of_char(command_buff, ' ');
	  char* command;
	  char* parameters;

	  if(space_index >= 0){
	    command = substring(command_buff, 0, space_index);
	    parameters = substring(command_buff, space_index + 1, len);
	  }else{
	    command = command_buff;
	    parameters = 0;
	  }

		int command_found = 0;
		if(strcmp(command, commands[0].command)) {
			command_found++;
			if(strlen(parameters) > 0) {
				terminal_writestring("el comando 'ayuda' no acepta argumentos\n");
			} else {
				for(int i = 0; i < index_command; i++) {
					terminal_writestring(commands[i].command);
					terminal_writestring(" - ");
					terminal_writestring(commands[i].description);
					terminal_putchar('\n');
				}
			}
		}

		if(strcmp(command, commands[1].command)) {
			terminal_writestring(calc(command, parameters));
			command_found++;
		}

		if(strcmp(command, commands[2].command)) {
			command_found++;
		}

		if(strcmp(command, commands[3].command)) {
			command_found++;
		}

		if(command_found == 0) {
			terminal_writestring("Comando no reconocido, escriba 'ayuda' para ver comandos validos.\n");
		}
	}
}

/**
	reinicia el buffer del shell
*/
void shell_reset_buffer() {
  index_buffer = 0;
  memset(shell_buffer, sizeof(shell_buffer), 0);
}

/**
	imprime el command prompt del shell
*/
void shell_print_gumac() {
	terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	terminal_writestring(terminal_in);
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}

/**
	registra nombre del comando y la description de un comando dado
*/
void register_command(char* command, char* description) {
  command_t newcommand;
  newcommand.command = command;
  newcommand.description = description;
  commands[index_command] = newcommand;
  index_command++;
}

/**
	inicializa el shell con un listado de comandos dados
*/
void initShell() {
	memset(commands, sizeof(commands), 0);
	register_command("ayuda", "imprime este menu");
	register_command("calc", "realiza operaciones basicas de aritmetica");
	register_command("sonido", "reproduce un sonido");
	register_command("apagar", "apaga la computadora");
}

/**
	maneja el caracter ingresado desde el teclado
*/
void shell_handle_key(int32_t keycode, char ch) {
  if(keycode > 0 && ch > 2 && index_buffer + 1 < 200 && ch != '\n' && activeShell > 0) {
    terminal_putchar(ch);
		shell_buffer[index_buffer] = ch;
		index_buffer++;
  }

	if(ch == '\n' && activeShell > 0) {
		terminal_putchar('\n');
		shell_handle_command();
		shell_print_gumac();
		shell_reset_buffer();
	}

	if(keycode == BACKSPACE && activeShell > 0) {
		if(index_buffer > 0) {
			index_buffer--;
			terminal_backspace();
		}
	}

	if(keycode == ESCAPE && activeShell > 0) {
		shell_reset_buffer();
		terminal_column = 0;
		terminal_row = 0;
		terminal_clear();
		shell_print_gumac();
	}

	if(ch == '\n' && activeShell == 0) {
		terminal_putchar('\n');
		activeShell++;
		initShell();
		shell_print_gumac();
	}
}

/*KEYBOARD METHODS*/
#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

/// KEYBOARD TEST

uint8_t shift = 0;
uint8_t caps = 0;

/**
	inicializa los Interrupt
*/
void init_pics(int pic1, int pic2)
{
   /* send ICW1 */
   outb(PIC1, ICW1);
   outb(PIC2, ICW1);

   /* send ICW2 */
   outb(PIC1 + 1, pic1);
   outb(PIC2 + 1, pic2);

   /* send ICW3 */
   outb(PIC1 + 1, 4);
   outb(PIC2 + 1, 2);

   /* send ICW4 */
   outb(PIC1 + 1, ICW4);
   outb(PIC2 + 1, ICW4);

   /* disable all IRQs */
   outb(PIC1 + 1, 0xFF);
}

/**
	obtiene el codigo de la tecla precionada
*/
char getScancode() {
    char c=0;
    do {
        if(inb(0x60)!=c) {
            c=inb(0x60);
            if(c>0)
                return c;
        }
    } while(1);
}

/**
	maneja la tecla presionada en el teclado y encuentra su carater correspondiente
	para luego ser enviada al shell
*/
void getcharshell(int32_t keycode) {
	switch(keycode) {
		case SHIFT:
			shift = 1;
			break;
		case SHIFT_RELEASE:
			shift = 0;
			break;
		case CAPSLOCK:
			if(caps > 0){
			caps = 0;
			}else {
			caps = 1;
			}
			break;
		default:
			if(keycode < 0) return;
			char ch = 0;
			if(shift || caps) {
				ch = keymap_upper[keycode];
			}else {
				ch = keymap_lower[keycode];
			}
			//case handled, now pass to shell
			shell_handle_key(keycode, ch);
	}
}

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
	init_pics(0x20, 0x28);
	do{
		if(inb(0x60)!=c) {
			c = inb(0x60);
    	if(c>0) {
				getcharshell(c);
			}
		}
	}while(1);
}
