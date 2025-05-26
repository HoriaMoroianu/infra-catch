#include "timers.h"

uint16_t led_delay = 31250; // 2 seconds at 16MHz with 1024 prescaler

void initLedTimer(void) {
  TCCR1A = 0; // Clear control register A
  TCCR1B = 0; // Clear control register B

  TCCR1B |= (1 << WGM12); // Set CTC mode

  OCR1A = led_delay; // Set compare value for 16MHz with 1024 prescaler
  TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
}

void startLedTimer(void) {
  TCNT1 = (uint16_t)0; // Reset timer counter
  TCCR1B |= (1 << CS12) | (1 << CS10); // Start timer with prescaler of 1024
}

void stopLedTimer(void) {
  TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Stop the timer
}
