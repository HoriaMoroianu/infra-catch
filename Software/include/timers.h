#ifndef TIMERS_H
#define TIMERS_H

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * This function sets up timer1 in CTC mode with a prescaler of 1024 for LED 
 * delay intervals and enables the timer compare interrupt.
 */
void initLedTimer(void);

/**
 * Starts the LED timer with the specified delay.
 */
void startLedTimer(void);

/**
 * Stops the LED timer by disabling the timer's clock source.
 */
void stopLedTimer(void);

#endif // TIMERS_H