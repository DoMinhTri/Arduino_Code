
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *wi_ssid  = "Robo-T";    // Wifi 
const char *wi_pass  = "dmt@123abc";

void setup() 
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wi_ssid, wi_pass);
  delay(3000);
}
//////////////////////////////////////////////////////////////////////////////////
void loop() 
{
   if(WiFi.status() == WL_CONNECTED) 
   {
      String tuoicay = LayChuoiJson();
      Serial.println(tuoicay);
   }
   delay(1000);
}
////////////////////////////////////////////////////////////////////////////////////
String LayChuoiJson()
{
        HTTPClient http;
        String SVLink    = "http://192.168.1.4/home/congtac/index.php" ;//+ SVID ;
         http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){  SVLink = http.getString();  }
        } 
        http.end();
        //////////
        return SVLink.trim();
}
////////////////////////////////////////////////////////////////////////////////////
