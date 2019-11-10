#include <Servo.h>

const int sensorPin = A0;    // pin that the sensor is attached to
const int servoPin = 3;      // pin that the servo is attached to

int sensorValue;         // the sensor value
int servoValue;          // the servo value
int sensorMin = 1023;    // minimum sensor value
int sensorMax = 0;       // maximum sensor value

Servo myServo;  // Create servo object to control a servo


void setup() {
  myServo.attach(servoPin);  // Attach the servo on D3 to the servo object

  // Turn on LED to signal the start of the calibration period
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Calibrate during the first 10 seconds
  while (millis() < 10000) {
    sensorValue = analogRead(sensorPin);
    // Record the maximum sensor value
    sensorMax = max(sensorMax, sensorValue);
    // Record the minimum sensor value
    sensorMin = min(sensorMin, sensorValue);
  }

  // Signal the end of the calibration period
  digitalWrite(13, LOW);
}


void loop() {
  // Read the sensor
  sensorValue = analogRead(sensorPin);

  // Apply the calibration to the sensor reading
  servoValue = map(sensorValue, sensorMin, sensorMax, 0, 179);

  // In case the sensor value is outside the range seen during calibration
  servoValue = constrain(servoValue, 0, 179);

  // Drive the servo using the calibrated value
  myServo.write(servoValue);
}
