
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
ESP8266WiFiMulti WiFiMulti;
////////////////////////////////////////////////////////
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
////////////////////////////////////////////////////////
const char *ssid     = "TAG4YU_SPW1_";
const char *password = "0918180212";
////////////////////////////////////////////////////////
const uint8_t fingerprint[20] = { 0xE5,0x2E,0x56,0x9D,0x01,0xB0,0x88,0xF9,0x12,0x63,0x4E,0xD8,0x32,0x51,0xA9,0xBC,0x32,0x4C,0x0C,0x0E };
String sUrl  = "https://nfc2cloud.com/iot/?id=tag4yu";
String myMac = WiFi.softAPmacAddress();
////////////////////////////////////////////////////////
#define GPIO2 2
void Led_2_On() {  digitalWrite(GPIO2, LOW);   }
void Led_2_Off(){  digitalWrite(GPIO2, HIGH);  }
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}
////////////////////////////////////////////////////////


void setup() 
{
  Serial.begin(9600);
  delay(1000);
  /////////////
  myMac.replace(":", "");
  sUrl = "https://nfc2cloud.com/spw1/?id=" + myMac;
  /////////////
  Serial.println("Begin");
  WiFiMulti.addAP("Tag4yu", "0918180212");
  /////////////
  WiFi.softAP(ssid + myMac, password);
  server.on("/", handleRoot);
  server.begin();
  /////////////
  pinMode(GPIO2,  OUTPUT);
  digitalWrite(GPIO2, HIGH);
  ////////////
  Serial.println("");
  Serial.println(myMac);
}
////////////////////////////////////////////////////////
void loop() {
   
  server.handleClient();
  /////////////////////
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    uint8_t PinOut = GetHTTPSData(sUrl).substring(0,1).toInt();
    if(PinOut == 1) { Led_2_On();  } 
    if(PinOut == 0) { Led_2_Off(); }
    Serial.println(PinOut);
  }
  delay(200);
}

////////////////////////////////////////////////////////
String GetHTTPSData(String sUrl)
{
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;
    String payload = "";
    if (https.begin(*client, sUrl)) 
    { 
      int httpCode = https.GET();
      if (httpCode > 0)
      {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {  payload = https.getString();   }
      }
      else { payload = https.errorToString(httpCode).c_str(); }
      https.end();
    }
    else {  payload = "[HTTPS] Unable to connect\n";   }
    return payload;
}
////////////////////////////////////////////////////////
