#include "main.h"
#include "random.h"
#include "timers.h"

#include <Arduino.h>
#include <IRRemote.hpp>
#include <LiquidCrystal_I2C.h>

GameState game_state = GAME_START; // Initial game state
bool valid_ir = false;      // Flag to indicate if a valid IR signal has been received
bool led_timeout = false;   // Flag to indicate if the LED timer has timed out

ISR(TIMER1_COMPA_vect) {
  led_timeout = true; // Set the flag when the timer expires
}

int main(void) {
  init();           // Initialize the Arduino library for IRRemote
  initLEDs();       // Set LED pins as output
  initLedTimer();   // Set up the LED timer1
  IrReceiver.begin(IRECV_PIN, ENABLE_LED_FEEDBACK);  // IR receiver setup

  // DEBUG:
  Serial.begin(9600); // Enable serial communication

  while (1) {
    filterIRSignal(); // Filter IR signal

    switch (game_state) {
      case GAME_START: handleStart(); break;
      case GAME_PLAYING: hadlePlay(); break;
      case GAME_OVER: handleGameOver(); break;
      default: break; // TODO: watchdog reset
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
    // start the game /////
    Serial.println("Game started!");
    game_state = GAME_PLAYING;
    chooseNextLED(); // Choose and turn on the first LED
  }
  IrReceiver.resume();
}

void hadlePlay() {
  if (led_timeout) {
    // DEBUG:
    Serial.println("LED EXPIRED!");

    led_timeout = false; // Reset the flag
    chooseNextLED(); // Choose and turn on the next LED
    return;
  }

  //// IR signal received ////
  if (!valid_ir) {
    return;
  }

  // Check if the pressed button is correct
  bool match = validateButton(IrReceiver.decodedIRData.command);

  if (match) {
    Serial.println("SOCRE++");
  } else {
    Serial.println("Wrong button pressed!");
  }

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
  }
  IrReceiver.resume();
}

void initLEDs() {
  DDRB |= (1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3);
  PORTB &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3));
}

bool validateButton(uint16_t button) {
  if (button == BTN0 && bit_is_set(PORTB, LED0)) {
    PORTB &= ~(1 << LED0);
    return true;
  }
  if (button == BTN1 && bit_is_set(PORTB, LED1)) {
    PORTB &= ~(1 << LED1);
    return true;
  }
  if (button == BTN2 && bit_is_set(PORTB, LED2)) {
    PORTB &= ~(1 << LED2);
    return true;
  }
  if (button == BTN3 && bit_is_set(PORTB, LED3)) {
    PORTB &= ~(1 << LED3);
    return true;
  }
  return false;
}

void chooseNextLED() {
  PORTB &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3)); // Turn off all LEDs

  // TODO: delay before choosing the next LED
  delay(200); // Delay for 100 milliseconds

  switch (nextRand() & 0b11) {
    case 0: PORTB |= (1 << LED0); break;
    case 1: PORTB |= (1 << LED1); break;
    case 2: PORTB |= (1 << LED2); break;
    case 3: PORTB |= (1 << LED3); break;
    default: PORTB |= (1 << LED0);
  }
  startLedTimer();  // Restart the LED timer
}
