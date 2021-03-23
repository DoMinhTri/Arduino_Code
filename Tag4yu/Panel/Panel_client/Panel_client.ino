
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

uint8_t Gio, Phut, SoLan = 0;
void Restart() { ESP.restart(); }
////////////////////////////////////
const char *wi_ssid  = "4yPanel_1";    // Wifi 
const char *wi_pass  = "0918180212";

void setup() 
{
  Serial.begin(9600);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.begin(wi_ssid, wi_pass);
  delay(3000);
}
//////////////////////////////////////////////////////////////////////////////////
String UartData = "";
void loop() 
{
  ////////////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ UartData+= KyTu; } delay(5);
  }
  ////////////////////////////
  if( UartData != "")
  { 
      SendGet(UartData);
      UartData = "";
  }
  /////////////
  delay(500);
  AutoReset();
}
////////////////////////////////////////////////////////////////////////////////////
void AutoReset()
{
  SoLan++;
  if(SoLan>=120)
  {
    Phut++; SoLan = 0;
    if(Phut >= 60)
    {
      Gio++;
      if(Gio >= 24){ Restart(); }
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////
void SendGet(String val)
{
    String link = "http://192.168.10.1/?4ypv=" + val ;
    Serial.println(link);
    //////////
    HTTPClient http;
    http.begin(link); //HTTP
    uint8_t httpCode = http.GET();
}
////////////////////////////////////////////////////////////////////////////////////
