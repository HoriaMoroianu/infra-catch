#include "main.h"
#include "random.h"
#include "timers.h"
#include "lcd.h"
#include "sounds.h"

#include <Arduino.h>
#include <IRRemote.hpp>

GameState game_state = GAME_START; // Initial game state
bool valid_ir = false;      // Flag to indicate if a valid IR signal has been received
bool led_timeout = false;   // Flag to indicate if the LED timer has timed out

uint16_t high_score = 0;
uint16_t score = 0;
uint8_t lives = 6;


ISR(TIMER1_COMPA_vect) {
  led_timeout = true; // Set the flag when the timer expires
}


int main(void) {
  init();           // Initialize the Arduino library for IRRemote
  initLEDs();       // Set LED pins as output
  initLedTimer();   // Set up the LED timer1
  initLCD();        // Initialize the LCD
  initBuzz();       // Initialize the buzzer

  IrReceiver.begin(IRECV_PIN, ENABLE_LED_FEEDBACK);  // IR receiver setup
  displayStart(); // Display the start message on the LCD
  playStart();    // Play the start melody

  while (1) {
    filterIRSignal(); // Filter IR signal

    switch (game_state) {
      case GAME_START: handleStart(); break;
      case GAME_PLAYING: hadlePlay(); break;
      case GAME_OVER: handleGameOver(); break;
      default: break;
    }
  }
  return 0;
}


void filterIRSignal() {
  valid_ir = false; // Reset the valid IR flag

  if (!IrReceiver.decode()) {
    return; // No IR signal received, exit the function
  }
  if (!IrReceiver.decodedIRData.decodedRawData) {
    IrReceiver.resume();
    return; // No valid IR data, exit the function
  }
  valid_ir = true; // Set the valid IR flag to true
}


void handleStart() {
  if (!valid_ir) {
    return; // No valid IR signal, exit the function
  }

  if (IrReceiver.decodedIRData.command == BTN_START) {
    lives = 6; // Reset lives
    score = 0; // Reset score
    displayScore(lives, score); // Display the score on the LCD
    setSeed(extractTimers()); // Set the random seed based on timers
  
    _delay_ms(1500); // Delay before starting the game
    game_state = GAME_PLAYING;
    chooseNextLED();  // Choose and turn on the first LED
    led_timeout = false; // Reset the LED timeout flag
  }
  IrReceiver.resume();
}


void hadlePlay() {
  if (!lives) {
    // If no lives left, end the game
    game_state = GAME_OVER;
    PORTB &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3));
  
    displayGameOver(score);
    playFail(); // Play the fail melody
    IrReceiver.resume();
    return;
  }

  if (led_timeout) {
    lives--;
    displayScore(lives, score);
    buzz(BAD_NOTE, 100); // fail sound
    chooseNextLED(); // Choose and turn on the next LED
    return;
  }

  //// IR signal received ////
  if (!valid_ir) {
    return;
  }

  // If the start button is pressed, end the game
  if (IrReceiver.decodedIRData.command == BTN_START) {
    game_state = GAME_OVER;
    PORTB &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3));

    displayGameOver(score);
    playFail(); // Play the fail melody
    IrReceiver.resume();
    return;
  }

  // Check if the pressed button is correct and update the score
  validateButton(IrReceiver.decodedIRData.command);
  displayScore(lives, score); // Update the score display on the LCD
  chooseNextLED(); // Choose and turn on the next LED
  IrReceiver.resume();
}


void handleGameOver() {
  if (!valid_ir) {
    return; // No valid IR signal, exit the function
  }

  if (IrReceiver.decodedIRData.command == BTN_START) {
    // start the game /////
    game_state = GAME_START;
    displayStart(); // Display the start message on the LCD
    playStart();    // Play the start melody
  }
  IrReceiver.resume();
}

void initLEDs() {
  DDRB |= (1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3);
  PORTB &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3));
}

void validateButton(uint16_t button) {
  if (button == BTN0 && bit_is_set(PORTB, LED0)) {
    PORTB &= ~(1 << LED0);
    score += 10;
    buzz(GOOD_NOTE, 100);
    return;
  }
  if (button == BTN1 && bit_is_set(PORTB, LED1)) {
    PORTB &= ~(1 << LED1);
    score += 10;
    buzz(GOOD_NOTE, 100);
    return;
  }
  if (button == BTN2 && bit_is_set(PORTB, LED2)) {
    PORTB &= ~(1 << LED2);
    score += 10;
    buzz(GOOD_NOTE, 100);
    return;
  }
  if (button == BTN3 && bit_is_set(PORTB, LED3)) {
    PORTB &= ~(1 << LED3);
    score += 10;
    buzz(GOOD_NOTE, 100);
    return;
  }
  lives--; // If the button pressed does not match the LED, decrement lives
  buzz(BAD_NOTE, 100); // fail sound
}

void chooseNextLED() {
  stopLedTimer(); // Stop the LED timer to prevent further interrupts

  // Turn off all LEDs and wait a bit before choosing the next one
  PORTB &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3)); 
  _delay_ms(250);

  switch (nextRand() & 0b11) {
    case 0: PORTB |= (1 << LED0); break;
    case 1: PORTB |= (1 << LED1); break;
    case 2: PORTB |= (1 << LED2); break;
    case 3: PORTB |= (1 << LED3); break;
    default: PORTB |= (1 << LED0);
  }

  led_timeout = false; // Reset the LED timeout flag
  startLedTimer();  // Restart the LED timer
}
