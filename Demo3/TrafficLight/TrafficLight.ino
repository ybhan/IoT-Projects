#define RED_PIN 2
#define YELLOW_PIN 3
#define GREEN_PIN 4
#define TOUCH_PIN 8   // the touch sensor (push-button)
#define BUZZER_PIN 5  // the buzzer (accessible pedestrain signals)

// Traffic lights' duration
#define RED_MILLIS 10000
#define RED_YELLOW_MILLIS 2000
#define GREEN_MILLIS 5000  // after pressing the button
#define YELLOW_MILLIS 2000

// Number the 4 possible states for switch-case in loop()
const int RED_STATE = 0;
const int RED_YELLOW_STATE = 1;
const int GREEN_STATE = 2;
const int YELLOW_STATE = 3;

int tl_state;            // traffic light state
unsigned long tl_timer;  // traffic light timer
int touch_state;         // touch button state
unsigned long buzzer_timer;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize states and timers
  touch_state = 0;       // button not touched
  tl_state = RED_STATE;  // start with Red light
  tl_timer = millis();
  buzzer_timer = millis();
}

void loop() {
  switch (tl_state) {
    case RED_STATE:
      // Turn Red light on
      digitalWrite(RED_PIN, HIGH);

      // Buzzer: 250ms on 250ms off, 880Hz tone
      if (millis() - buzzer_timer > 500) {
        tone(BUZZER_PIN, 880, 250);
        buzzer_timer = millis();
      }

      if (millis() - tl_timer > RED_MILLIS) {
        // Turn Red light and Buzzer off
        digitalWrite(RED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        // Trigger next state: RED-YELLOW
        tl_state = RED_YELLOW_STATE;
        tl_timer = millis();  // Reset timer
      }
      break;

    case RED_YELLOW_STATE:
      // Turn Red and Yellow lights on
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(YELLOW_PIN, HIGH);

      if (millis() - tl_timer > RED_YELLOW_MILLIS) {
        // Turn Red and Yellow lights off
        digitalWrite(RED_PIN, LOW);
        digitalWrite(YELLOW_PIN, LOW);
        // Trigger next state: GREEN
        tl_state = GREEN_STATE;
        tl_timer = millis();  // Reset timer
      }
      break;

    case GREEN_STATE:
      // Turn Green light on
      digitalWrite(GREEN_PIN, HIGH);

      // Buzzer: 500ms on 1500ms off, 880Hz tone
      if (millis() - buzzer_timer > 2000) {
        tone(BUZZER_PIN, 880, 500);
        buzzer_timer = millis();
      }

      // If the button is not touched, keep detecting it
      if (touch_state == 0) {
        touch_state = digitalRead(TOUCH_PIN);
        // If touch detected, start timing for buffer time
        if (touch_state == 1) {
          tl_timer = millis();
        }
      }

      // If enough buffer time has passed after pressing button
      if ((touch_state == 1)
          && (millis() - tl_timer > GREEN_MILLIS)) {
        // Turn Green light and Buzzer off
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        // Reset button state to untouched
        touch_state = 0;
        // Trigger next state: YELLOW
        tl_state = YELLOW_STATE;
        tl_timer = millis();  // Reset timer
      }
      break;

    case YELLOW_STATE:
      // Turn Yellow light on
      digitalWrite(YELLOW_PIN, HIGH);

      if (millis() - tl_timer > YELLOW_MILLIS) {
        // Turn Yellow light off
        digitalWrite(YELLOW_PIN, LOW);
        // Trigger next state: RED
        tl_state = RED_STATE;
        tl_timer = millis();  // Reset timer
      }
      break;
    }
}
