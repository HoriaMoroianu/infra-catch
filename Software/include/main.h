#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>

// Led pin definitions
#define LED0 PB3
#define LED1 PB2
#define LED2 PB1
#define LED3 PB0

#define DECODE_NEC   // Protocol for IR receiver
#define IRECV_PIN 2  // Pin for IR receiver

// Remote control button definitions
enum IRCode {
    BTN_START = 0x9,
    BTN0 = 0xC,
    BTN1 = 0x18,
    BTN2 = 0x8,
    BTN3 = 0x1C
};

// Game states
enum GameState {
  GAME_START,
  GAME_PLAYING,
  GAME_OVER
};

/**
 * Filters the received IR signal and sets the global variable `valid_ir` to true if the signal is valid.
 */
void filterIRSignal();

/**
 * Handles the game start logic.
 */
void handleStart();

/**
 * Handles the game playing logic.
 */
void hadlePlay();

/**
 * Displays game over message on the LCD and plays the end melody.
 */
void transitionToGameOver();

/**
 * Handles the game over logic.
 */
void handleGameOver();

/**
 * Initializes the LED pins as output and sets them to low.
 */
void initLEDs();

/**
 * Validates the pressed button against the currently lit LED and updates the score accordingly.
 */
void validateButton(uint16_t button);

/**
 * Turns on random LED.
 */
void chooseNextLED();

#endif // MAIN_H
