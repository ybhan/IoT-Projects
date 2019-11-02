unsigned long time_1 = 0;
unsigned long time_2 = 0;
unsigned long time_3 = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // "millis() > time_1 + 3000" is a bad style due to roll-over.
  // Instead, codes below can handle roll-over since time_1 is unsigned.
  // Always compare DURATION rather than TIMESTAMP!
  if (millis() - time_1 > 3000) {
    time_1 = millis();
    Serial.print("1 ");
  }
  if (millis() - time_2 > 7000) {
    time_2 = millis();
    Serial.print("2 ");
  }
  if (millis() - time_3 > 15000) {
    time_3 = millis();
    Serial.print("3 ");
  }
}
