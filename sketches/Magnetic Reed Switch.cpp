#include "Arduino.h"

#define LED_RED D3
#define DOOR D2

void setup(){

  Serial.begin(115200);
  pinMode(LED_RED, OUTPUT);
  pinMode(DOOR, INPUT);
  
}

void loop(){
  
  if (digitalRead(DOOR)==HIGH){
    digitalWrite(LED_RED, LOW);
    Serial.println("Door is Closed");
  }
  else {
    digitalWrite(LED_RED, HIGH);
    Serial.println("Door is Open");
  }
  delay(100);
}
