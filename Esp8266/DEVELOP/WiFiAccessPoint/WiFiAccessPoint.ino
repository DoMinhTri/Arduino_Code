

#include <ESP8266WiFi.h>

const char *ssid     = "Smart Plugin";
const char *password = "12345678";

WiFiServer server(80);

void setup() 
{
  Serial.begin(9600);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  delay(5000);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
}

void loop() 
{ 
  WiFiClient client = server.available();
  if (!client){ return; }
  while(!client.available()){  delay(10); }
  String request = client.readStringUntil('\r');
  client.flush();
  if (request.indexOf("/") != -1)
  { 
    Serial.println(request);
    request = "";
  }
  
}
