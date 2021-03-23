#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
WiFiClientSecure client;


const char* fingerprint = "57 D6 DB 07 17 9F 1C F8 CF 22 8A 4C 1B B7 7A 73 4D CC A0 B4";
const char* ssid        = "Robo-T";
const char* password    = "dmt@123abc";
String sUrl             = "https://nfc2cloud.com/iot/demo.php";

#define GPIO2 2
void Led_2_On() { digitalWrite(GPIO2, LOW);  }
void Led_2_Off(){ digitalWrite(GPIO2, HIGH); }

void setup() {
  Serial.begin(9600); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500);  }
  pinMode(GPIO2,  OUTPUT);
  Serial.println("begin");
  client.setInsecure();
}
///////////////////////////////////////
String params = "";

void loop() 
{
   params = GetHTTPSData();
   Serial.println(params);
   
   if(params == "ok")
      Led_2_On();
    else
      Led_2_Off();
  delay(100);
}

String GetHTTPSData()
{
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;
    String payload = "";
    if (https.begin(*client, sUrl)) 
    { 
       uint8_t httpCode = https.GET();
       if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
           payload = https.getString();
       }
       https.end();
    }
    return payload;
}
