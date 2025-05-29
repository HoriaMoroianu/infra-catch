#include "sounds.h"
#include "timers.h"

#include <avr/io.h>
#include <util/delay.h>

// Melody frequencies and durations:
uint16_t start[4][2] = {
  { NOTE_E5, 150 },
  { NOTE_F5, 150 },
  { NOTE_G5, 150 },
  { NOTE_A5, 300 }
};

uint16_t win[4][2] = {
  { NOTE_C5, 150 },
  { NOTE_E5, 150 },
  { NOTE_G5, 150 },
  { NOTE_C6, 300 }
};

uint16_t fail[4][2] = {
  { NOTE_Bb4, 300 },
  { NOTE_G4,  350 },
  { NOTE_Eb4, 400 },
  { NOTE_C4,  600 }
};

void playStart() {
  for (uint8_t i = 0; i < 4; i++) {
    buzz(start[i][0], start[i][1]);
    if (start[i][1] == 150) {
      _delay_ms(170);
    } else {
      _delay_ms(320);
    }
  }
  // Turn off the buzzer
  PORTD &= ~(1 << PD6);
  _delay_ms(200);
}

void playWin() {
  for (uint8_t i = 0; i < 4; i++) {
    buzz(win[i][0], win[i][1]);
    if (win[i][1] == 150) {
      _delay_ms(170);
    } else {
      _delay_ms(320);
    }
  }
  // Turn off the buzzer
  PORTD &= ~(1 << PD6);
  _delay_ms(200);
}

void playFail() {
  for (uint8_t i = 0; i < 4; i++) {
    buzz(fail[i][0], fail[i][1]);
    if (i == 0) {
      _delay_ms(320);
    } else if (i == 1) {
      _delay_ms(370);
    } else if (i == 2) {
      _delay_ms(420);
    } else if (i == 3) {
      _delay_ms(620);
    }
  }
  // Turn off the buzzer
  PORTD &= ~(1 << PD6);
  _delay_ms(200);
}
