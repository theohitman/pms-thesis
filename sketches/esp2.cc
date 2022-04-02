#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTTYPE DHT11 
#define DHT_SENSOR D5
#define RAIN_SENSOR D3
#define LED D6

const char* ssid = "pinet";
const char* password = "12345678";
const char* mqtt_server = "192.168.7.94";
const char* ESP_TOPIC = "hua/esp2";
const char* Temperature_Topic = "hua/esp2/temp";
const char* Humidity_Topic = "hua/esp2/hum"; 
const char* Water_Topic = "hua/esp2/water";    

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHT_SENSOR, DHTTYPE);

long now = millis();
long Last_DHT_Measure = 0;
long Last_Water_Measure = 0;

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
  pinMode(RAIN_SENSOR,INPUT);
  pinMode(LED, OUTPUT);

}

void loop() {

  if(!client.loop())
    client.connect("ESP2");

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
  
  // Detects for water every 3 seconds
  if (now - Last_Water_Measure > 3000) {
    Last_Water_Measure = now;

    if (digitalRead(RAIN_SENSOR)==LOW)
    {
      Serial.println("Water Detected!!!");
      client.publish(Water_Topic, "water");
      digitalWrite(LED, HIGH);
    }
    else
    {
      client.publish(Water_Topic, "no water");
      digitalWrite(LED, LOW);
    }
  }  
} 
