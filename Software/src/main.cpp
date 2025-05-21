#include <avr/io.h>
#include <Arduino.h>
#include <IRRemote.hpp>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,  16, 2);

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

void LCD_demo() {
  lcd.setCursor(0, 0);
  lcd.print("Lives: ");
  lcd.write(0);
  lcd.write(0);
  lcd.write(0);
  lcd.write(0);
  lcd.write(0);

  lcd.setCursor(0, 1);
  lcd.print("Score: 0");
}

int main(void) {
  // Initialize the Arduino library
  init();

  // LCD setup
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, heart);
  LCD_demo();
  
  // IR receiver setup
  IrReceiver.begin(2, ENABLE_LED_FEEDBACK);

  // Enable serial communication
  Serial.begin(9600);
  Serial.println("Ready to receive IR signals");
  
  while (1) {
    if (IrReceiver.decode()) {
      Serial.print("Received: 0x");
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      IrReceiver.resume();
    }
  }
  return 0;
}
