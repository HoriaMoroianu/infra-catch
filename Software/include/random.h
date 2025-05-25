#ifndef RANDOM_H
#define RANDOM_H

#include <inttypes.h>

/**
 * Sets the seed for the random number generator.
 */
void setSeed(uint32_t seed);

/**
 * Generates a pseudo-random number between 0 and 255 
 * based on the xorshift32 algorithm.
 * @returns A 8-bit pseudo-random number.
 */
uint8_t nextRand(void);

#endif // RANDOM_H
