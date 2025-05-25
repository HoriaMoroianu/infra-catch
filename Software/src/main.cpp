#include "main.h"
#include "random.h"
#include "timers.h"

#include <Arduino.h>
#include <IRRemote.hpp>
#include <LiquidCrystal_I2C.h>

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

  chooseNextLED(); // Choose and turn on a random LED

  while (1) {
    if (led_timeout) {
      // DEBUG:
      Serial.println("LED EXPIRED!");

      led_timeout = false; // Reset the flag
      chooseNextLED(); // Choose and turn on the next LED
      continue;
    }

    if (!IrReceiver.decode()) {
      continue; // Continue to wait for IR signal
    }

    //// IR signal received ////

    // Continue if the decoded data is not available
    if (!IrReceiver.decodedIRData.decodedRawData) {
      IrReceiver.resume();
      continue;
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
  return 0;
}

void initLEDs(void) {
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

void chooseNextLED(void) {
  PORTB &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3)); // Turn off all LEDs

  // TODO: delay before choosing the next LED

  switch (nextRand() & 0b11) {
    case 0: PORTB |= (1 << LED0); break;
    case 1: PORTB |= (1 << LED1); break;
    case 2: PORTB |= (1 << LED2); break;
    case 3: PORTB |= (1 << LED3); break;
    default: PORTB |= (1 << LED0);
  }
  startLedTimer();  // Restart the LED timer
}
