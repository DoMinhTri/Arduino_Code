
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
WiFiServer server(80);

const char* ssid     = "Duy Khanh 7";
const char* password = "passdk07";



String WifiName, WifiPass, WifiIP, WifiPort, SVID, DVNUM, AdmPass;
void setup() {
  Serial.begin(9600);
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  server.begin();
  
}

void loop() {
  LayChuoiJson();
}

String LayChuoiJson()
{
        HTTPClient http;
        String WebCode = "";
        //String SVLink   = "http://fablabs.store/cttmm/inc/onoff/?UTSID=" + SVID + "&utstype=utsdevice&utsdvnum=" + DVNUM + "&utsvalue=" + TTCongTac + "&utsdvupdate=" + DeviceUpdate;
        String SVLink = "http://fablabs.store/cttmm/inc/onoff/?UTSID=11223344AABBCCDD&utsvalue=0,1,0,1&utstype=utsdevice&utsdvnum=1&utsdvupdate=0" ;
        http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){ WebCode = http.getString(); }
        } 
        http.end();
        Serial.println(WebCode); 
        return WebCode;
        //////////
}
