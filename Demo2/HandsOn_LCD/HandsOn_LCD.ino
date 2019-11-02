#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;  // liquid crystal display

const int colorR = 240;
const int colorG = 120;
const int colorB = 40;

void setup() {
    // Set up the LCD's number of columns and rows
    lcd.begin(16, 2);

    // Set up the LCD's backlight color
    lcd.setRGB(colorR, colorG, colorB);

    // Print a message to the LCD
    lcd.print("Hello, world!");
}

void loop() { }
