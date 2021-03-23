

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid     = "Smart Plugin";
const char *password = "12345678";
ESP8266WebServer server(80);

//#define LED_BUILTIN 2
//////////////////////////////////////

void PageIndex() {
  ///////////////////////
  if (server.hasArg("den"))
  {    
    String den = server.arg("den").c_str(); 
    if(den !="") 
    {
        Serial.println(den);
        if(den == "1")
            digitalWrite(LED_BUILTIN, LOW); 
         else
            digitalWrite(LED_BUILTIN, LOW); 
    }
  }
  ///////////////////////
  server.send(200, "text/html", "<div align=center><h1>Hello</h1></div>");
}
//////////////////////////////////////

void setup() {
  delay(1000);
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  server.on("/", PageIndex);
  server.begin();
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
  server.handleClient();
}
