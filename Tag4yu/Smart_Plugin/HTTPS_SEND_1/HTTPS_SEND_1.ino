
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
ESP8266WiFiMulti WiFiMulti;

const uint8_t fingerprint[20] = { 0xE5,0x2E,0x56,0x9D,0x01,0xB0,0x88,0xF9,0x12,0x63,0x4E,0xD8,0x32,0x51,0xA9,0xBC,0x32,0x4C,0x0C,0x0E };
String sUrl  = "https://nfc2cloud.com/iot/?id=tag4yu";
String myMac = WiFi.softAPmacAddress();
////////////////////////////////////////////////////////
#define GPIO2 5
void Led_2_On() {  digitalWrite(GPIO2, LOW);  Serial.println("Led  on");  }
void Led_2_Off(){  digitalWrite(GPIO2, HIGH); Serial.println("Led  off"); }

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(9600);
  delay(4000);
  WiFiMulti.addAP("Tag4yu", "0918180212");
  Serial.println("Begin");
  /////////////
  myMac.replace(":", "");
  sUrl = "https://nfc2cloud.com/spw1/?id=" + myMac;
  /////////////
  pinMode(GPIO2,  OUTPUT);
  digitalWrite(GPIO2, HIGH);
}
////////////////////////////////////////////////////////
void loop() {
   
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    String html   = GetHTTPSData(sUrl).substring(0,1).toInt();
    String PinOut = html.substring(0,1);

    if(PinOut == "1") { Led_2_On();  } 
    if(PinOut == "0") { Led_2_Off(); }
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
      else 
      {
          payload = https.errorToString(httpCode).c_str();
      }

      https.end();
    }
    else {  payload = "[HTTPS] Unable to connect\n";   }

    return payload;
}
////////////////////////////////////////////////////////
