#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void memcpy(void* src, void* dst , size_t size){
	uint8_t* bsrc = (uint8_t*)src;
	uint8_t* bdst = (uint8_t*)dst;
	for(size_t i = 0; i < size; i++){
		bdst[i] = bsrc[i];
	}
}
void memset(void* dst, uint32_t size, uint8_t fill){
	for(uint32_t i = 0; i < size; i++){
		((uint8_t*)dst)[i] = fill;
	}
}