#include <IPAddress.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
///////////////////////////////////////
const char *ssid     = "4ypanel_1";
const char *password = "0918180212";
String UartData, htmldata = "";
#define GPIO0 0
uint8_t Gio, Phut, SoLan = 0;
boolean TrangThai = false;

#define MAX_CLIENTS 10
WiFiClient *clients[MAX_CLIENTS] = { NULL };
///////////////////////////////////////
void RelayOn() { digitalWrite(0, HIGH); TrangThai = true;  }
void RelayOff(){ digitalWrite(0, LOW);  TrangThai = false; }
void Restart() { ESP.restart(); }
///////////////////////////////////////
void Index() 
{
   htmldata = "";
   if (server.hasArg("4ypv"))
  {    
     htmldata = server.arg("4ypv").c_str(); 
     if(htmldata != "")
     {
        if(htmldata == "off"){ RelayOff(); }
        else{ OnFunction(htmldata); }
     }
   }
   ///////
   server.send(200, "text/html", "<div align=center><h1>" + htmldata+ "</h1></div>");
}
///////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(GPIO0,OUTPUT);
  RelayOff();
  ///////////////////
  WiFi.mode(WIFI_AP);
  IPAddress Subnet    = IPAddress(255,255,255,0);
  IPAddress APlocalIP = IPAddress(192,168,10,1);
  WiFi.softAPConfig( APlocalIP, APlocalIP, Subnet);
  ///////////////////
  //  softAP(const char* ssid, const char* passphrase, int channel = 1, int ssid_hidden = 0);
  WiFi.softAP(ssid, password, 1, 1);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/", Index);
  server.begin();
}
///////////////////////////////////////
void loop() {
  server.handleClient();
  ////////////////////////////
  CheckClienConn();
  ////////////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ UartData+= KyTu; } delay(5);
  }
  ////////////////////////////
  if( UartData != "")
  { 
      if(UartData == "on")
      {     
        RelayOn();
      }
      else
      {
        OnFunction(UartData);
      }
      //////////
      UartData = "";
  }
  //////
  delay(500);
  AutoReset();
}
////////////////////////////////////////////////////////////////////////////////////
void CheckClienConn()
{

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
      if(Gio >= 18)
      {
        if(TrangThai == false){ Restart(); }
      }
    }
  }
}
/////////////////////////////////////////
void OnFunction(String hdata)
{
    uint8_t nTime = 0;
    if(hdata == "on")
    { 
        RelayOn(); 
    }
    else
    {
        String val = hdata.substring(0, 2);
        nTime = hdata.substring(2, 5).toInt();
        if(val == "on")
        {
           RelayOn();
           if(nTime >0)
           {
              if(nTime <= 255)
              {
                 delay(nTime*1000);
                 RelayOff();
              }
           }
        }
     }  
}
/////////////////////////////////////////
