#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTTYPE DHT11 
#define DHT_SENSOR D5

const char* ssid = "pinet";
const char* password = "12345678";
const char* mqtt_server = "192.168.7.94";
const char* ESP_TOPIC = "hua/esp1";
const char* Temperature_Topic = "hua/esp1/temp";
const char* Humidity_Topic = "hua/esp1/hum";     

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHT_SENSOR, DHTTYPE);

long now = millis();
long Last_DHT_Measure = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  
  dht.begin();
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

}

void loop() {

  if(!client.loop())
    client.connect("ESP1");

  now = millis();
  // Publishes new temperature and humidity every 10 seconds
  if (now - Last_DHT_Measure > 10000) {
    Last_DHT_Measure = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    static char temperatureTemp[7];
    dtostrf(t, 6, 2, temperatureTemp);
    
    
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    // Publishes Temperature and Humidity values
    client.publish(Temperature_Topic, temperatureTemp);
    client.publish(Humidity_Topic, humidityTemp);
    
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.println();
  }
} 
