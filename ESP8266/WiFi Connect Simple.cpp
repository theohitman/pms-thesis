/**
 Απλό Sketch για να δοκιμάσω την σύνδεση του ESP8266 σε ένα δίκτυο
 */

#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

void setup()
{
 Serial.begin(115200);
 Serial.setDebugOutput(true); // Μου φέρνει στην σειριακή τα errors (Προαιρετικό)
 delay(10);
 //connect to a WiFi network
 WiFi.mode(WIFI_STA);
 WiFiMulti.addAP("ssid", "password"); //your hotspot details go here
 Serial.println();
 Serial.println();
 Serial.print("Wait for WiFi… ");
 while (WiFiMulti.run() != WL_CONNECTED)
 {
   Serial.print(".");
   delay(500);
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 delay(500);
}

void loop()
{
}