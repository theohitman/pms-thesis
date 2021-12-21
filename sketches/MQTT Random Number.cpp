#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SSID HERE";
const char* password = "YOUR PASSWORD";

//Change to IP address of Pi
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_topic_pub = "example/topic";

WiFiClient wifi_client;
PubSubClient client(wifi_client);
long last_updated_at = 0;
int value = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(100);
  Serial.println();
  Serial.print("Connecting to network ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    // Attempt to connect
    if (client.connect("UniqueClientNameHere")) {
      Serial.println("Success");
    } else {
      Serial.print("Failed, rc=");
      Serial.println(client.state());
      Serial.println(" Retrying on 5s...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - last_updated_at > 5000) {
    last_updated_at = now;
    
    value = random(10);   
    
    char value_string[8];
    dtostrf(value, 1, 2, value_string);
    Serial.print("Value: ");
    Serial.println(value_string);
    client.publish(mqtt_topic_pub, value_string);
  }
}