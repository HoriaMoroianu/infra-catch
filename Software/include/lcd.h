#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

#define HEART 0 // Custom character index for heart icon

void initLCD();
void displayStart();
void displayScore(uint8_t lives, uint16_t score);
void displayGameOver(uint16_t score);

#endif // LCD_H