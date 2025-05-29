#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

#define HEART 0 // Custom character index for heart icon

/**
 * @brief Initializes the LCD display and custom characters.
 */
void initLCD();

/**
 * @brief Displays Start screen with button instructions.
 */
void displayStart();

/**
 * @brief Displays the current state of the game.
 * 
 * @param lives The number of lives remaining for the player.
 * @param score The current score of the player.
 */
void displayScore(uint8_t lives, uint16_t score);

/**
 * @brief Displays the score and high score on the LCD.
 */
void displayGameOver(uint16_t score, uint16_t high_score);

#endif // LCD_H