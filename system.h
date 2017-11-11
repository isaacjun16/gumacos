#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void apagar() {
/*  __asm__ __volatile__ ("mov ah, 53h\n"  //this is an APM command
"mov al, 07h\n"              //Set the power state...
"mov bx, 0001h\n"            //...on all devices to...
"mov cx, 03h\n"              // 01h = Standby, 02h = Suspend, 03h = Off
"int 15h\n"                 //call the BIOS function through interrupt 15h
"jc APM_error");*/
}
