#include "random.h"

uint32_t generator = 0;

void setSeed(uint32_t seed) {
  generator = seed;
}

uint8_t nextRand(void) {
  if (!generator) {
    generator = 1;
  }

  uint32_t x = generator;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  generator = x;
  return (uint8_t)(x & 0xFF);
}
