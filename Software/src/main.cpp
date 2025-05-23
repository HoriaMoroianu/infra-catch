#include "main.h"
#include "timers.h"

#include <Arduino.h>
#include <IRRemote.hpp>
#include <LiquidCrystal_I2C.h>

ISR(TIMER1_COMPA_vect) {
  led_timeout = true; // Set the flag when the timer expires

  // DEBUG:
  PORTB ^= (1 << LED3); // Toggle LED3 (for debugging)
}

int main(void) {
  init(); // Initialize the Arduino library for IRRemote
  initLEDs(); // Set LED pins as output
  initLedTimer(); // Set up the LED timer1

  // IR receiver setup
  IrReceiver.begin(IRECV_PIN, ENABLE_LED_FEEDBACK);

  // Enable serial communication
  Serial.begin(9600);
  Serial.println("Ready to receive IR signals");
  
  while (1) {
    if (led_timeout) {
      // DEBUG:
      Serial.println("LED EXPIRED!");

      led_timeout = false; // Reset the flag
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
      // score++
    } else {
      // lives--
    }

    // turn the current led off
    // choose and turn on the next led
    // reset the timer

    IrReceiver.resume();
  }
  return 0;
}

void initLEDs(void) {
  DDRB |= (1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3);
  PORTB &= ~((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3));
}

bool validateButton(uint16_t button) {
  switch (button) {
    case BTN_START:
      Serial.println("Start button pressed");
      return true;
    case BTN0:
      Serial.println("Button 0 pressed");
      return true;
    case BTN1:
      Serial.println("Button 1 pressed");
      return true;
    case BTN2:
      Serial.println("Button 2 pressed");
      return true;
    case BTN3:
      Serial.println("Button 3 pressed");
      return true;
    default:
      Serial.println("Unknown button pressed");
      return false;
  }
}
