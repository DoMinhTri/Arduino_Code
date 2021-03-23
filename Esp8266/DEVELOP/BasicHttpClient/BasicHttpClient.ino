/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {

    Serial.begin(115200);
    WiFiMulti.addAP("Caphe sang", "khongbiet");
}

void loop() {

    if((WiFiMulti.run() == WL_CONNECTED)) 
    {
        HTTPClient http;
        http.begin("http://uptosmart.vn/homegreen/data/?utsid=6c30046100320032&utsvalue=azxcdv"); //HTTP
        int httpCode = http.GET();

        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK) 
            {
                String payload = http.getString();
                Serial.println(payload);
            }
        } 
        http.end();
    }

    delay(1000);
}

