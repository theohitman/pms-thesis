#include <Arduino.h>

#define LED_ORANGE D6
#define RAIN_SENSOR D7

void setup() {
  Serial.begin(115200);
  pinMode(LED_ORANGE,OUTPUT);
  pinMode(RAIN_SENSOR,INPUT);  
}

void loop() {
  
  if (digitalRead(RAIN_SENSOR)==LOW)
  {
    Serial.println("Water Detected!!!");
    digitalWrite(LED_ORANGE,HIGH);
    // delay(1000);
  }

  else
  {
    Serial.println("No Water");
    digitalWrite(LED_ORANGE,LOW);
    // delay(1000);
  }
delay(1000);
}