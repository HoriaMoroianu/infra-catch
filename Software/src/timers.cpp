#include "timers.h"

#include <Arduino.h>

uint16_t led_delay = 31250; // 2 seconds at 16MHz with 1024 prescaler
volatile uint32_t duration = 0; // Duration for the buzzer

ISR(TIMER0_COMPA_vect) {
	if (duration != 0) {
    duration--;
	} else {
    TIMSK0 &= ~(1 << OCIE0A); // Disable the timer compare interrupt
		TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Disable clock
		PORTD &= ~(1 << PD6); // Set the port state to low
    TCCR0A &= ~(1 << COM0A0); // Disable toggle on compare match
	}
}

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

void timer_freq_prescale(uint32_t freq, uint8_t *a_ocr, uint8_t *a_prescaler) {
  // Prescaler values for 1, 8, 64, 256, 1024
  uint8_t pow[] = { 0, 3, 6, 8, 10, 0 };

  uint16_t ocr = 0;
  uint8_t prescaler = 0;

  // Find the best prescaler and OCR value for the given frequency
  do {
    ocr = (uint16_t) (F_CPU / ((freq << 1) * (1 << pow[prescaler])));
    prescaler++;
  } while ((ocr > 255) && (pow[prescaler]));

  ocr--;
  if (ocr > 255) {
    ocr = 255;
  }

  *a_ocr = ocr & 0xff;
  *a_prescaler = prescaler;
}

void initBuzz() {
  TCCR0A = 0; // Clear control register A
  TCCR0B = 0; // Clear control register B
  TCCR0A |= (1 << WGM01);  // Set CTC mode

  DDRD |= (1 << PD6);   // Set OC0A pin (PD6) as output
  PORTD &= ~(1 << PD6); // Ensure OC0A pin is low initially
}

void buzz(uint16_t a_freq, uint16_t a_duration) {
  uint8_t prescaler = 0;
  uint8_t ocr = 0;
  duration = 0; // Reset duration

  // Calculate the OCR value and prescaler for the given frequency
  timer_freq_prescale(a_freq, &ocr, &prescaler);

  // Set the OCR0A value and reset the timer counter
  TCNT0 = 0;
  OCR0A = ocr;
  TCCR0A |= (1 << COM0A0); // Enable toggle on compare match
  
  // Calculate the amount of cycles
  duration = ((uint32_t)a_freq * a_duration / 500);

  TIMSK0 |= (1 << OCIE0A); // Enable timer compare interrupt
  TCCR0B = (prescaler & 0b111); // Set prescaler bits in TCCR0B
}
