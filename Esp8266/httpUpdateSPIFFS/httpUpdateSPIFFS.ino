/**
   httpUpdateSPIFFS.ino

    Created on: 05.12.2015

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

ESP8266WiFiMulti WiFiMulti;

#define APSSID "Robo-T"
#define APPSK  "dmt@123abc"
boolean DaUpdate = false;
void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(APSSID, APPSK);
  Serial.println("");
}

void loop() {

if(DaUpdate == false)
{
  DaUpdate = true;
  Serial.println("Update");
  
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    WiFiClient client;
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
    t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs(client, "http://192.168.1.2/home/home.bin");
    
    if (ret == HTTP_UPDATE_OK) 
    {
      Serial.println("Update sketch...");
      ret = ESPhttpUpdate.update(client, "http://192.168.1.2/home/home.bin");

      switch (ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;

        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("HTTP_UPDATE_NO_UPDATES");
          break;

        case HTTP_UPDATE_OK:
          Serial.println("HTTP_UPDATE_OK");
          break;
      }
    }
  }
  //////////////////
}
delay(5000);
}
