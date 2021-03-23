
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;

const uint8_t fingerprint[20] = { 0x57, 0xD6, 0xDB, 0x07, 0x17, 0x9F, 0x1C, 0xF8, 0xCF, 0x22, 0x8A, 0x4C, 0x1B, 0xB7, 0x7A, 0x73, 0x4D, 0xCC, 0xA0, 0xB4 }; // nfc2cloud.com
String sUrl  = "https://nfc2cloud.com/iot/demo.php";
String myMac = WiFi.softAPmacAddress();
////////////////////////////////////////////////////////
#define GPIO2 2
void Led_2_On() {  digitalWrite(GPIO2, LOW);   }
void Led_2_Off(){  digitalWrite(GPIO2, HIGH);  }

void setup() 
{
  Serial.begin(9600);
  delay(4000);
  WiFiMulti.addAP("Robo-T", "dmt@123abc");
  /////////////
  myMac.replace(":", "");
  /////////////
  pinMode(GPIO2,  OUTPUT);
  digitalWrite(GPIO2, HIGH);
}
////////////////////////////////////////////////////////
void loop() {
   
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    String data = GetHTTPSData(sUrl);
    if(data == "1") { Led_2_On();  } 
    if(data == "0") { Led_2_Off(); }
    Serial.println(data);
  }
  delay(500);
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
      if (httpCode > 0){ if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){ payload = https.getString(); }}
      else{ payload = https.errorToString(httpCode).c_str(); }
      https.end();
    } else {  payload = "[HTTPS] Unable to connect\n"; }
    return payload;
}
////////////////////////////////////////////////////////
