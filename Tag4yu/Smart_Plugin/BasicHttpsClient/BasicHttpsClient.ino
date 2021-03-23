// Fingerprint for demo URL, expires on June 2, 2019, needs to be updated well before this date
//  86 0D BC E8 78 33 CA B7 6B 5F 05 88 23 C2 3F 61 A2 B0 AD E5                                                              // fingerprint https/nfc2cloud.com
//  0x86, 0x0D, 0xBC, 0xE8, 0x78, 0x33, 0xCA, 0xB7, 0x6B, 0x5F, 0x05, 0x88, 0x23, 0xC2, 0x3F, 0x61, 0xA2, 0xB0, 0xAD, 0xE5   // fingerprint https/nfc2cloud.com

//********** CHANGE PIN FUNCTION  TO GPIO **********
//GPIO 1 (TX) swap the pin to a GPIO.
//GPIO 3 (RX) swap the pin to a GPIO.
//**************************************************

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
ESP8266WiFiMulti WiFiMulti;

const uint8_t fingerprint[20] = { 0x86, 0x0D, 0xBC, 0xE8, 0x78, 0x33, 0xCA, 0xB7, 0x6B, 0x5F, 0x05, 0x88, 0x23, 0xC2, 0x3F, 0x61, 0xA2, 0xB0, 0xAD, 0xE5 };
String sUrl = "https://nfc2cloud.com/iot/?id=tag4yu4";
////////////////////////////////////////////////////////
#define GPIO0  1
#define GPIO2  2

void Led_1_On() {  digitalWrite(GPIO0, LOW);  } //  Serial.println("Led 1 on");  }
void Led_1_Off(){  digitalWrite(GPIO0, HIGH); } // Serial.println("Led 1 off"); }
void Led_2_On() {  digitalWrite(GPIO2, LOW);  } // Serial.println("Led 2 on");  }
void Led_2_Off(){  digitalWrite(GPIO2, HIGH); } // Serial.println("Led 2 off"); }

void setup() 
{
  //Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(4000);
  WiFiMulti.addAP("Tag4yu #iot_nfc", "0918180212");
  //Serial.println("Begin");
  /////////////
  pinMode(GPIO0,  OUTPUT);
  pinMode(GPIO2,  OUTPUT);
  digitalWrite(GPIO0, HIGH);
  digitalWrite(GPIO2, HIGH);
}
////////////////////////////////////////////////////////
void loop() {
   
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    String html = GetHTTPSData(sUrl);
    String Den1 = html.substring(0,1);
    String Den2 = html.substring(1,2);

    if(Den1 == "1") { Led_1_On();  } 
    if(Den1 == "0") { Led_1_Off(); }

    if(Den2 == "1") { Led_2_On();  } 
    if(Den2 == "0") { Led_2_Off(); }
    
    //Serial.println(html);
    //Serial.println(Den1);
    //Serial.println(Den2);
    //Serial.println("----------------------");
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
