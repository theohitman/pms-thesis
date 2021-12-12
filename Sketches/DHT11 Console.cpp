// DHT Temperature & Humidity Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 5          // Digital pin connected to the DHT sensor 
#define DHTTYPE DHT11     // Define the type of sensor in use (DHT11,DHT21,DHT22)

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
 
  dht.begin();

}

void loop() {                    
  delay(1000);           // Delay between measurements.
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}