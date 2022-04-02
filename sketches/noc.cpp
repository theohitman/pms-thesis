#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define BUZZER D6
#define LED_BLUE D3
#define LED_RED D5

const char* ssid = "pinet";
const char* password = "12345678";
const char* mqtt_server = "192.168.7.94";
const char* ESP_TOPIC = "hua/noc";


WiFiClient espClient;
PubSubClient client(espClient);


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



void callback(String topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.print(message);
    if (message == "blue on") { digitalWrite(LED_BLUE, LOW); } 
    if (message == "blue off") { digitalWrite(LED_BLUE, HIGH); } 
    if (message == "red on") { digitalWrite(LED_RED, LOW); } 
    if (message == "red off") { digitalWrite(LED_RED, HIGH); }
    if (message == "buzzer on") { digitalWrite(BUZZER, HIGH); } 
    if (message == "buzzer off") { digitalWrite(BUZZER, LOW); } 
    Serial.println();
    Serial.println("-----------------------");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("hua/noc");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
} 

void setup() {
    
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(BUZZER,OUTPUT);
  pinMode(LED_BLUE,OUTPUT);
  pinMode(LED_RED,OUTPUT);
  digitalWrite(LED_BLUE, HIGH);
  digitalWrite(LED_RED, HIGH);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();        

  if(!client.loop())
    client.connect("NOC");
} 