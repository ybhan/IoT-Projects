#include <Arduino_FreeRTOS.h>
#include <semphr.h>  // Add the FreeRTOS functions for Semaphores (or Flags)

// Declare a mutex Semaphore Handle which will be used to manage the Serial Port.
// It will be used to ensure only only one task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;

int LEDPIN = 13;

// Define 3 tasks
void TaskTemperatureRead( void *pvParameters );
void TaskToggleLED( void *pvParameters );
void TaskLightRead( void *pvParameters );


void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);

  if ( xSerialSemaphore == NULL ) {
    xSerialSemaphore = xSemaphoreCreateMutex();
    if ( ( xSerialSemaphore ) != NULL ) {
      xSemaphoreGive( ( xSerialSemaphore ) );
    }
  }

  xTaskCreate(
    TaskTemperatureRead,
    (const portCHAR *) "TemperatureRead",
    128,  // stack size
    NULL,
    3,  // priority
    NULL);

  xTaskCreate(
    TaskToggleLED,
    (const portCHAR *) "toggleLED",
    128,  // stack size
    NULL,
    2,  // priority
    NULL);
    
  xTaskCreate(
    TaskLightRead,
    (const portCHAR *) "LightRead",
    128,  // stack size
    NULL,
    2,  // priority
    NULL);
}


void loop() {}


/*---------------------- Tasks ---------------------*/

void toggleLight(int state) {
  if (state==false) {
    digitalWrite(LEDPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
  }
}

// Task 1
void TaskTemperatureRead( void *pvParameters __attribute__((unused)) ) {
  for (;;) {
    if (xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ) {
      Serial.println("Task 1");
      int a = analogRead(A0);
      int count = 0;
      while(count < 4) {
        vTaskDelay( 10 );
        Serial.println(++count);
        digitalWrite(LEDPIN, !digitalRead(LEDPIN));
      }
      // Now free or "give" the Serial Port for others
      xSemaphoreGive( xSerialSemaphore );
      Serial.println("off");
    }
    vTaskDelay(1);
  }
}

// Task 2
void TaskToggleLED( void *pvParameters __attribute__((unused)) ) {
  for (;;) {
    if (xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ){
      Serial.println("Task 2");
      int count = 0;
      while(count < 2) {
        vTaskDelay(60);
        Serial.println(++count);
        toggleLight(digitalRead(LEDPIN));
      }
      // Now free or "give" the Serial Port for others
      xSemaphoreGive( xSerialSemaphore );
    }
    vTaskDelay(1);
  }
}

// Task 3
void TaskLightRead( void *pvParameters __attribute__((unused)) ) {
  for (;;) {
    if (xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ) {
      Serial.println("Task 3");
      int a = analogRead(A1);
      int count = 0;
      while(count < 2) {
        vTaskDelay(60);
        Serial.println(++count);
        toggleLight(digitalRead(LEDPIN));
      }
      // Now free or "give" the Serial Port for others
      xSemaphoreGive( xSerialSemaphore );
    }
    vTaskDelay(1);
  }
}
