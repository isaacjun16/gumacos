#include "time.h"

unsigned long long rdtsc(void) {
  unsigned long long int x;
     asm volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
}

unsigned long long getTimeMs() {
  unsigned long long int ms;
  unsigned long long ns = rdtsc();
  ms = ns / 1000000;
  return ms;
}

unsigned long long getTimeS() {
  unsigned long long int s;
  unsigned long long ms = getTimeMs();
  s = ms / 1000;
  return s;
}

void timer_wait(unsigned long long int sec) {
  unsigned long long curTimeS = getTimeS();
  curTimeS = curTimeS + sec;
  while(getTimeS() < curTimeS) {
    //wait
  }
}
