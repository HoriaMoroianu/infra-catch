#ifndef TIMERS_H
#define TIMERS_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define MIN_LED_DELAY 3125 // 200ms at 16MHz with 1024 prescaler
#define MAX_LED_DELAY 31250 // 2 seconds at 16MHz with 1024 prescaler

/**
 * This function sets up timer1 in CTC mode with a prescaler of 1024 for LED 
 * delay intervals and enables the timer compare interrupt.
 */
void initLedTimer();

/**
 * Starts the LED timer with the specified delay.
 */
void startLedTimer();

/**
 * Stops the LED timer by disabling the timer's clock source.
 */
void stopLedTimer();

/**
 * Decreases the LED delay for the next blink.
 */
void decreaseLedDelay();

/**
 * Calculates the appropriate OCR value and prescaler for a given frequency.
 */
void timer_freq_prescale(uint32_t freq, uint8_t *a_ocr, uint8_t *a_prescaler);

/**
 * Initializes the buzzer by setting up timer0 in CTC mode.
 */
void initBuzz();

/**
 * Buzzes the buzzer at a specified frequency for a specified duration.
 * 
 * @param a_freq Frequency in Hz.
 * @param a_duration Duration in milliseconds.
 */
void buzz(uint16_t a_freq, uint16_t a_duration);

/**
 * Extrat a seed from the timers
 */
uint32_t extractTimers();

#endif // TIMERS_H
