#include <ESP8266WiFi.h>
#include "MQ135.h"
 
String apiKey = "78FE424K622AFQZN"; // Enter your Write API key from ThingSpeak
const char *ssid = "JioFi2021";     // replace with your wifi ssid and wpa2 key
const char *pass = "bnj70bnj70";
const char *server = "api.thingspeak.com";
 
WiFiClient client;

void setup()
{
  Serial.begin(19200);
  delay(1000);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);      Serial.print(".");
  }
    Serial.println("");     Serial.println("WiFi connected");
    delay(1000);
}
void loop()
{
    MQ135 gasSensor = MQ135(A0);
    float air_quality = gasSensor.getPPM();
    Serial.print("Air Quality: ");   
    air_quality=air_quality/1000; 
    Serial.print(air_quality);
    Serial.println("  PPM");   
    Serial.println();
 
    if (client.connect(server, 80)) // api.thingspeak.com
   {
    String postStr = apiKey;     
    postStr += "&field1=";
    postStr += String(air_quality);     
    postStr += "r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.println("Air Quality sample Sent to Thingspeak");
   }
    client.stop();
    Serial.println("Waiting...");
    delay(8000);      // thingspeak 8 sec delay between sample updates.
}
