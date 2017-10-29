#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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

		int hasParameters = 0;
	  if(space_index >= 0){
	    command = substring(command_buff, 0, space_index);
	    parameters = substring(command_buff, space_index + 1, len);
			hasParameters++;
	  }else{
	    command = command_buff;
	    parameters = 0;
	  }

		int command_found = 0;
		if(strcmp(command, commands[0].command)) {
			command_found++;
			if(hasParameters == 1) {
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
void shell_init() {
	memset(commands, sizeof(commands), 0);
	register_command("ayuda", "imprime este menu");
	register_command("calc", "realiza operaciones basicas de aritmetica");
	register_command("sonido", "reproduce un sonido");
	register_command("apagar", "apaga la computadora");
}

/**
	maneja el caracter ingresado desde el teclado
*/
static inline void shell_handle_key(int32_t keycode, char ch) {
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
		shell_init();
		shell_print_gumac();
	}
}
