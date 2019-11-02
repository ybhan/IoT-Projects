#include <Wire.h>
#include <SPI.h>
#include "Adafruit_CAP1188.h"
#include "rgb_lcd.h"

// RESET pin is used for I2C
#define CAP1188_RESET  9

// Use I2C, with RESET pin
Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

rgb_lcd lcd;

const int lcdColumn = 16;
const int lcdRow = 2;
const int lcdColorR = 120;
const int lcdColorG = 200;
const int lcdColorB = 240;

void setup() {
  Serial.begin(9600);

  // Set up the LCD's number of columns and rows
  lcd.begin(lcdColumn, lcdRow);
  // Set up LCD's backlight color
  lcd.setRGB(lcdColorR, lcdColorG, lcdColorB);

  Serial.println("CAP1188 test!");
  lcd.print("CAP1188 test!");
  lcd.setCursor(0, 1);

  // Wire connecting AD to 3V -> I2C address 0x28
  if (!cap.begin(0x28)) {
    Serial.println("CAP1188 not found!");
    lcd.print("CAP1188 notFound");
    while (1);
  } else {
    Serial.println("CAP1188 found!");
    lcd.print("CAP1188 found!");
    delay(1000);
  }
}

void loop() {
  uint8_t touched = cap.touched();
  int numOfTouched = 0;

  // Clear the LCD screen
  lcd.clear();

  if (touched == 0) {
    // No touch detected
    lcd.print(numOfTouched); lcd.print(" touched.");
    delay(100);
    return;
  }

  // Set the LCD cursor to column 0, row 1
  lcd.setCursor(0, 1);

  for (uint8_t i=0; i<8; i++) {
    if (touched & (1 << i)) {  // bitwise operators
      Serial.print("C"); Serial.print(i+1); Serial.print("\t");
      lcd.print(i+1); lcd.print(" ");
      numOfTouched++;
    }
  }
  Serial.println();

  // Set the LCD cursor to column 0, row 0
  lcd.setCursor(0, 0);
  lcd.print(numOfTouched); lcd.print(" touched:");
  delay(100);
}
