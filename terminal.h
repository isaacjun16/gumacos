#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Hardware text mode color constants. */
#define TERMINAL_COMMAND_PORT 0x3D4
#define TERMINAL_DATA_PORT 0x3D5

size_t terminal_strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
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
