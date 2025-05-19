#include <avr/io.h>
#include <Arduino.h>

int main(void) {
  init();
  Serial.begin(9600);
  Serial.println("Hello, World!");

  while (1) {
      delay(1000);
      Serial.println("Running...");
  }
  return 0;
}
