#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
	inicializa los Interrupt
*/
void keyboard_init_pics(int pic1, int pic2)
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
char keyboard_scancode() {
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
void keyboard_charshell(int32_t keycode) {
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
