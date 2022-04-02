#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MOTION_SENSOR D1
#define DOOR D2
#define BUZZER D6
#define LED D4

const char* ssid = "pinet";
const char* password = "12345678";
const char* mqtt_server = "192.168.7.94";
const char* ESP_TOPIC = "hua/esp3";
const char* Motion_Topic = "hua/esp3/motion";
const char* Door_Topic = "hua/esp3/door"; 

WiFiClient espClient;
PubSubClient client(espClient);

long now = millis();
long Last_Motion_Measure = 0;
long Last_Door_Measure = 0;

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
    
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(MOTION_SENSOR,INPUT);
  pinMode(DOOR,INPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(LED,OUTPUT);
}

void loop() {

  if(!client.loop())
    client.connect("ESP3");

  now = millis();

  // Detects motion every 2 seconds
  if (now - Last_Motion_Measure > 2000) {
    Last_Motion_Measure = now;

    int val = digitalRead(MOTION_SENSOR);
    if (val == HIGH) 
    {
      Serial.println("Motion Detected!!!");
      client.publish(Motion_Topic, "motion");
      digitalWrite(LED, HIGH);
    }
    else 
    {
      // Serial.println("No Motion detected!");
      client.publish(Motion_Topic, "no motion");
      digitalWrite(LED, LOW); 
    }

  }

  // Detects door status every second
  if (now - Last_Door_Measure > 1000) {
    Last_Door_Measure = now;

    if (digitalRead(DOOR)==HIGH) 
    {
      // Serial.println("Door is Closed!");
      client.publish(Door_Topic, "door off");
      digitalWrite(BUZZER, LOW);
    }
    else 
    {
      Serial.println("Door is Open!");
      client.publish(Door_Topic, "door on");
      digitalWrite(BUZZER, HIGH); 
    }

  }
  

} 
