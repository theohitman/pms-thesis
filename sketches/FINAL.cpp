// Load Libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>


#define DHTTYPE DHT11   // Define the type of sensor in use (DHT11,DHT21,DHT22)
#define DHT_SENSOR D5
#define LED_RED D3
#define LED_GREEN D4
#define LED_ORANGE D8
#define BUZZER D6
#define RAIN_SENSOR D7
#define MOTION_SENSOR D1
#define DOOR D2

// WiFi Credentials
const char* ssid = "........";  // Update with your SSID
const char* password = "........"; // Update with your SSID password 
// MQTT Broker IP address
const char* mqtt_server = "broker.mqtt-dashboard.com"; // Update with the IP of MQTT broker
// Topic for ESP to Subscribe
const char* ESP_TOPIC = "name/esp_no"; // Update with your basic topic, for example "hua/esp1"

// Create topic names to Publish
const char* Temperature_Topic = "name/esp_no/temp";     // Update basic topic only for example "hua/esp1/temp"
const char* Humidity_Topic = "name/esp_no/hum";         // Update basic topic only
const char* Water_Topic = "name/esp_no/water";          // Update basic topic only
const char* Motion_Topic = "name/esp_no/motion";        // Update basic topic only
const char* Door_Topic = "name/esp_no/door";            // Update basic topic only


// Initialize espClient and create three variables
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Initialize DHT Sensor
DHT dht(DHT_SENSOR, DHTTYPE);

// Timers auxiliar variables
long now = millis();
long Last_DHT_Measure = 0;
long Last_Water_Measure = 0;
long Last_Motion_Measure = 0;
long Last_Door_Measure = 0;


// Connect to WiFi network
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

// Receives MQTT messages to a topic that EPS is subscribed
// Prints in serial monitor the message and switches GPIO states if message received   
void callback(String topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.print(message);
    if (message == "red on") { digitalWrite(LED_RED, HIGH); } 
    if (message == "red off") { digitalWrite(LED_RED, LOW); } 
    if (message == "green on") { digitalWrite(LED_GREEN, HIGH); } 
    if (message == "green off") { digitalWrite(LED_GREEN, LOW); }
    if (message == "buzzer on") { digitalWrite(BUZZER, HIGH); } 
    if (message == "buzzer off") { digitalWrite(BUZZER, LOW); } 
    Serial.println();
    Serial.println("-----------------------");
}

// If ESP can't establish an MQTT communication with broker, keeps trying to reconnect every 5 seconds
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe
      client.subscribe(ESP_TOPIC);
      //client.subscribe("topic2"); I can subscribe to more than one topics
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} 


void setup() {

  dht.begin();
  
  Serial.begin(115200); // Start serial communication
  setup_wifi();  // Calls setup_wifi() function to connect to wifi
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(RAIN_SENSOR,INPUT);
  pinMode(MOTION_SENSOR,INPUT);
  pinMode(DOOR,INPUT);

}

void loop() {

  if (!client.connected()) { // Check if ESP is connected to the MQTT broker. If not reconnect. 
    reconnect();
  }
  client.loop();  

  if(!client.loop())
    client.connect("ESP8266Client");

  now = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now - Last_DHT_Measure > 30000) {
    Last_DHT_Measure = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Convert float to string 
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
      digitalWrite(BUZZER,HIGH);
    }
    else
    {
      // Serial.println("No Water");
      client.publish(Water_Topic, "no water");
      digitalWrite(BUZZER,LOW);
    }
  }  
  
  // Detects motion every 2 seconds
  if (now - Last_Motion_Measure > 2000) {
    Last_Motion_Measure = now;

    int val = digitalRead(MOTION_SENSOR);
    if (val == HIGH) 
    {
      Serial.println("Motion Detected!!!");
      client.publish(Motion_Topic, "motion");
      digitalWrite(LED_ORANGE, HIGH);
    }
    else 
    {
      // Serial.println("No Motion detected!");
      client.publish(Motion_Topic, "no motion");
      digitalWrite(LED_ORANGE, LOW); 
    }

  }

  // Detects door status every second
  if (now - Last_Door_Measure > 1000) {
    Last_Door_Measure = now;

    if (digitalRead(DOOR)==HIGH) 
    {
      // Serial.println("Door is Closed!");
      client.publish(Door_Topic, "door off");
      digitalWrite(LED_RED, LOW);
    }
    else 
    {
      Serial.println("Door is Open!");
      client.publish(Door_Topic, "door on");
      digitalWrite(LED_RED, HIGH); 
    }

  }
} 