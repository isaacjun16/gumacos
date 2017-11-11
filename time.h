#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

unsigned long long rdtsc(void);
unsigned long long getTimeMs();
unsigned long long getTimeS();
void timer_wait(unsigned long long int sec);
