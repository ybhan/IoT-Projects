#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define LED_PIN 2
#define SENSOR_PIN A0
#define THRESHOLD 300
#define INTERVAL 1000

RF24 radio(9,10);

// 2 radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipe1 = 0xE8E8F0E0E8LL;
const uint64_t pipe2 = 0xE8E8F0E0E9LL;

// Payload
uint8_t buff[] = {0};
const uint8_t buffSize = sizeof(buff);
unsigned long startTime;


void setup(void) {
  Serial.begin(115200);
  printf_begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  // Setup and configure rf radio
  radio.begin();
  radio.openWritingPipe(pipe2);
  radio.openReadingPipe(1, pipe1);
  radio.startListening();

  radio.printDetails();
  startTime = millis() - INTERVAL - 1;
}


void loop(void) {
  // stop listening so we can send data
  radio.stopListening();
  buff[0] = analogRead(SENSOR_PIN);
  Serial.println("Try sending:");
  Serial.println(buff[0]);
  if (radio.write(buff, buffSize)) {
    Serial.println("succeeded");
  } else {
    Serial.println("failed");
  }

  // start listening
  for (int i = 0; i < 100; i++) {
    radio.startListening(); 
    if (radio.available()) {
      while (radio.available()) {
        radio.read(buff, buffSize);  
        Serial.println("Data received:");
        Serial.println(buff[0]);
      }
      if (buff[0] > THRESHOLD) {
        Serial.println("Threshold exceeded");
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }
    delay(20);
  }
}
