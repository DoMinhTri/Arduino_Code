#include <WiFi.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>

void OnlineUpdate()
{
        t_httpUpdate_return ret = ESPhttpUpdate.update("http://uptosmart.ddns.net/sw4/esp32.bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;
        case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;
        case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                Restart();
                break;
        }
}
