
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <IPAddress.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);
/////////////////////  
void Restart(){ ESP.restart(); }
void CallRestart(){ if (server.hasArg("restart")){ String  sStas = server.arg("restart").c_str();  if( sStas == "ok"){  Restart(); }}}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Desktop() 
{
  CallRestart();
  String data = "";
  if (server.hasArg("TrangThai"))
  { 
    data = server.arg("TrangThai") ;
    Serial.println(data);
  }
  server.send(200, "text/html", data);
}
//////////////////////////////////////////////////////////////////////////////////

const char *wi_ssid  = "Robo-T";    
const char *wi_pass  = "dmt@123abc";

void setup() 
{
  /////////////////////////////
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  /////////////////////////// Wifi connect
  WiFi.begin(wi_ssid, wi_pass);
  IPAddress wip     = IPAddress(192,168,1,120);
  IPAddress wgetway = IPAddress(192,168,1,1);
  IPAddress Subnet  = IPAddress(255,255,255,0);
  WiFi.config( wip, wgetway, Subnet);
  delay(3000);
  Serial.println();
  Serial.println(WiFi.localIP());
  ////////////////////////// 
  server.on("/", Desktop);
  server.begin();
}
//////////////////////////////////////////////////////////////////////////////////

void loop() 
{
   server.handleClient();
   String sdata = LayChuoiJson();
   Serial.println(sdata);
   delay(1000);
}

////////////////////////////////////////////////////////////////////////////////////
String LayChuoiJson()
{
        HTTPClient http;
        String payload   = "";
        String SVLink    = "http://blynk-cloud.com/c_qvdrbIV_dxy__DodN3VB78LOgHvJ9B/get/D2" ;
         http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){  payload = http.getString();  }
        } 
        http.end();
        //////////
        return payload;
}
