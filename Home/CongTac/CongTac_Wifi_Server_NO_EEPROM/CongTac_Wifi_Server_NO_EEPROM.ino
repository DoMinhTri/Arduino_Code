#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
String TTCongTac = "";
String UartData  = "";
String myMac     = "";
////////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void PageIndex() {
  ///////////////////////
  if (server.hasArg("cw4"))
  {    
    Serial.print(server.arg("cw4").c_str()); 
  }
  ///////////////////////
  server.send(200, "text/html",  TTCongTac );
}
//////////////////////////////////////////////////////////////////////////////////
void PageSetup() 
{
  //////////////////////
  server.send(200, "text/html", "192.168.1.98" );
}
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  delay(100);
  myMac = WiFi.softAPmacAddress();
  myMac.replace(":", "");

  ///////////// SERVER
  server.on("/",      PageIndex);
  server.on("/setup", PageSetup);
  server.begin();
  ///////////// ACCESS POINT
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("UTS_CW4_" + myMac, "dmt@zzz@123");
  ///////////// WIFI
  IPAddress wip     = IPAddress(192,168,1,98);
  IPAddress wgetway = IPAddress(192,168,1,1);
  IPAddress Subnet  = IPAddress(255,255,255,0);
  WiFi.config( wip, wgetway, Subnet);
  WiFi.begin("Robo-T", "dmt@123abc");
  delay(4000);
  /////////////
  Serial.println(WiFi.localIP());
}
/////////////////////////////////////////////////////////////////////////////

void loop() { 
   server.handleClient();
   //////////////////////////////////// Lay trang thai cong tac gui den ATMega
   String oData = LayDataLink();
   if(oData != ""){ TTCongTac = oData ; Serial.println(oData); }
   //////////////////////////////////// Load trang thai cong tac tu ATMega
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();
        if( 31<KyTu && KyTu < 127 ){ UartData+= KyTu; } delay(5);
   }
   /////
   if(UartData != "")
   { 
      TTCongTac = UartData; 
      UartData  = ""; 
   }
   ////////////////////////////////////
   delay(500);
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
String LayDataLink()
{
        HTTPClient http;
        String payload   = "";
        String SVLink    ="http://192.168.1.2/home/congtac/?wstas=" + TTCongTac + "&wip=192.168.1.98&wmac=" + myMac;
         http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){  payload = http.getString();  }
        } 
        http.end();
        Serial.println(SVLink);
        //////////
        return payload;
}
/////////////////////////////////////////////////////////////////////////////