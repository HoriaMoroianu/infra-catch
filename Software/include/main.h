#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>

// Led pin definitions
#define LED0 PB4
#define LED1 PB3
#define LED2 PB2
#define LED3 PB0

#define DECODE_NEC

// Pin for IR receiver
#define IRECV_PIN 2 

// Remote control button definitions
enum IRCode {
    BTN_START = 0x9,
    BTN0 = 0xC,
    BTN1 = 0x5E,
    BTN2 = 0x42,
    BTN3 = 0x4A
};

bool led_timeout = false; // Flag to indicate if the LED timer has timed out

/**
 * Initializes the LED pins as output and sets them to low.
 */
void initLEDs(void);

/**
 * @returns true if the pressed button is correct, false otherwise.
 */
bool validateButton(uint16_t button);

#endif // MAIN_H
