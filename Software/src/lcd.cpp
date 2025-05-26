#include "lcd.h"

// Set LCD 16x2 I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Heart icon for LCD
uint8_t heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

void initLCD() {
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.createChar(HEART, heart); // Create a custom character for the heart icon
  lcd.clear(); // Clear the LCD
  lcd.setCursor(0, 0); // Set cursor to the first row
}

void displayStart() {
  lcd.clear();                  // Clear the LCD
  lcd.setCursor(2, 0);          // Set cursor to the first row
  lcd.print("EQ to START");
  lcd.setCursor(0, 1);          // Set cursor to the second row
  lcd.print("1 2 4 5 for LEDs");
}

void displayScore(uint8_t lives, uint16_t score) {
  lcd.clear();                  // Clear the LCD
  lcd.setCursor(0, 0);          // Set cursor to the first row
  lcd.print("Lives: ");
  for (uint8_t i = 0; i < lives; i++) {
    lcd.write(HEART);           // Display heart icon for each life
  }

  lcd.setCursor(0, 1);          // Set cursor to the second row
  lcd.print("Score: ");
  lcd.print(score);             // Display the current score
}

void displayGameOver(uint16_t score) {
  lcd.clear();                  // Clear the LCD
  lcd.setCursor(3, 0);          // Set cursor to the first row
  lcd.print("GAME OVER");
  lcd.setCursor(0, 1);          // Set cursor to the second row
  lcd.print("Score: ");
  lcd.print(score);             // Display the final score
}
