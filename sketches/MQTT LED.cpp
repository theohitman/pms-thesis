#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED_PIN D1


const char* ssid = "........";
const char* password = "........";
const char* mqtt_server = "broker.mqtt-dashboard.com";


WiFiClient espClient;  // Initialize espClient and create three variables
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// void callback(char* topic, byte* payload, unsigned int length) 
// {
//   Serial.print("Command from MQTT broker is : ");
//   Serial.print(topic);
//   //Serial.print(payload);
//   int p =(char)payload[0]-'0';
//   // if MQTT comes a 0 turn off LED on D2
//   if(p==0) 
//   {
//      digitalWrite(LED_PIN, LOW); 
//     Serial.println(" Turn Off LED! " );
//   } 
//   // if MQTT comes a 1, turn on LED on pin D2
//   if(p==1)
//   {
//      digitalWrite(LED_PIN, HIGH); 
//     Serial.println(" Turn On LED! " );
//   }
//   Serial.println();
// } //end callback

// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived to topic [");
//   Serial.print(topic);
//   Serial.print("]: ");
//   for (unsigned int i = 0; i < length; i++) {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();

//   if ((char)payload[0] == '1') {
//     digitalWrite(LED_PIN, HIGH);   
//     Serial.println(" Turn On LED! " );
//   } 
//   if ((char)payload[0] == '0') {
//     digitalWrite(LED_PIN, LOW);
//     Serial.println(" Turn Off LED! " );   
//   }
// }

void callback(char *topic, byte *payload, unsigned int length) {
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
    Serial.println();
    Serial.println("-----------------------");
}

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
      client.subscribe("home/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} 

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_RED, LOW); 
  digitalWrite(LED_GREEN, LOW); 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}