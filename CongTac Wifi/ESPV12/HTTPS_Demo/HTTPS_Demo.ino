
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;
//  echo | openssl s_client -connect nfc2cloud.com:443 | openssl x509 -fingerprint -noout
const uint8_t fingerprint[20] = { 0x57, 0xD6, 0xDB, 0x07, 0x17, 0x9F, 0x1C, 0xF8, 0xCF, 0x22, 0x8A, 0x4C, 0x1B, 0xB7, 0x7A, 0x73, 0x4D, 0xCC, 0xA0, 0xB4 };
String sUrl  = "https://nfc2cloud.com/sw/index.php";
String myMac = WiFi.softAPmacAddress();
////////////////////////////////////////////////////////
/////////////////////  Relay
#define Relay1  16
#define Relay2  14
#define Relay3  12
#define Relay4  13
uint8_t MangTTDen[4] = {0,0,0,0};

void setup() 
{
  Serial.begin(9600);
  delay(4000);
  WiFiMulti.addAP("Robo-T", "dmt@123abc");
  /////////////
  myMac.replace(":", "");
  String sUrl  = "https://nfc2cloud.com/sw/index.php?uts_id=" + myMac + "&uts_stas=" + TTCongTac() ;
  /////////////
  /////////////
  pinMode(Relay1, OUTPUT); 
  pinMode(Relay2, OUTPUT); 
  pinMode(Relay3, OUTPUT); 
  pinMode(Relay4, OUTPUT); 
}
////////////////////////////////////////////////////////
void loop() {
   
   BatDen();
  
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    String data = GetHTTPSData(sUrl); // 0000,2358
    MangTTDen[0] = data.substring(0,1).toInt();
    MangTTDen[1] = data.substring(1,2).toInt();
    MangTTDen[2] = data.substring(2,3).toInt();
    MangTTDen[3] = data.substring(3,4).toInt();
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
////////////////////////////////////////////////////////////////////////////////////
String TTCongTac()
{
  String TTDen =  String(MangTTDen[0]) + "," + String(MangTTDen[1]) + "," + String(MangTTDen[2]) + "," + String(MangTTDen[3]);
  return TTDen;
}
//////////////////////////////////////////////////////////////////////////////////
void BatDen()
{
    digitalWrite(Relay1, MangTTDen[0]); 
    digitalWrite(Relay2, MangTTDen[1]); 
    digitalWrite(Relay3, MangTTDen[2]); 
    digitalWrite(Relay4, MangTTDen[3]); 
    if(MangTTDen[3] == 1){ delay(5000); MangTTDen[3] = 0; }
}
////////////////////////////////////////////////////////
