#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t* string_ptr = (uint8_t*) 3145728;
static inline void* string_kalloc(size_t size){
	void* ret = (void*) string_ptr;
	string_ptr += size;
	string_ptr += 2;
	return ret;
}

static inline uint16_t strlen(char* c){
	char i;
	uint16_t length = 0;
	while((i = c[length]) != 0){
		length++;
	}
	return length;
}

static inline uint8_t string_starts_with(char* full, char* start) {
	if(strlen(full) < strlen(start)) {
		return 0;
	}
	for(int i = 0; i < strlen(start); i++) {
		if(full[i] != start[i]){
			return 0;
		}
	}
	return 1;
}

static inline int32_t string_index_of_char(char* str, char look){
	for(int i = 0; i < strlen(str); i++){
		if(str[i] == look){
			return i;
		}
	}
	return -1;
}

static inline uint8_t strcmp(char* str1, char* str2){
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if(len1 != len2){
		return false;
	}
	for(int i = 0; i < len1; i++){
		if(str1[i] != str2[i]){
			return false;
		}
	}
	return true;
}

static inline char* substring(char* str, uint16_t start, uint16_t end){
	char* ret = (char* )string_kalloc((sizeof(char)) * (end - start + 1));
	ret[end-start] = 0;

	for(uint16_t i = 0; i < (end - start); i++){
		ret[i] = str[i + start];
	}
	return ret;
}
