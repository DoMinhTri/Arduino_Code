#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClientSecure client;

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
String params                 = "";
const char* fingerprint       = "A2 77 8D E8 5B D9 03 DB F9 6F 48 F5 81 C3 7A 8E 7C 7F 1B 54";
const char* ssid              = "Robo-T";
const char* password          = "dmt@123abc";
const String GOOGLE_SCRIPT_ID = "AKfycbxdUwc6esexbiVprl8xdzlJlV9ZkUVqoTdH_tWovYwH8d4dXqDx"; 
//////////////////////////////////////////////////////////////////////////////////  Relay
#define Relay1  16
#define Relay2  14
#define Relay3  12
#define Relay4  13
uint8_t MangTTDen[4] = {0,0,0,0};
//////////////////////////////////////////////////////////////////////////////////
int Rows       = 0;
boolean CTStas = true;
String myMac   = WiFi.softAPmacAddress();
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////
String LayChuoiIP(IPAddress _IP)
{
    String _ChuoiIP = (String)_IP[0] + "." + (String)_IP[1] + "." + (String)_IP[2] + "." + (String)_IP[3] ;
    return _ChuoiIP;
}
/////////////////////////////////////////////////////////////////////////////
String TTCongTac()
{
  String TTDen =  String(MangTTDen[0]) +  String(MangTTDen[1]) +  String(MangTTDen[2]) +  String(MangTTDen[3]);
  return TTDen;
}
/////////////////////////////////////////////////////////////////////////////
void BatDen()
{
    digitalWrite(Relay1, MangTTDen[0]); 
    digitalWrite(Relay2, MangTTDen[1]); 
    digitalWrite(Relay3, MangTTDen[2]); 
    digitalWrite(Relay4, MangTTDen[3]); 
}
/////////////////////////////////////////////////////////////////////////////
void handleRoot() 
{
  if (server.hasArg("cw4"))
  {    
        String cw4 = server.arg("cw4").c_str(); 
        ///////////////////
        if(cw4 != "")
        { 
            if(cw4 == "10"){ MangTTDen[0] = 0; CTStas = true;}
            if(cw4 == "11"){ MangTTDen[0] = 1; CTStas = true;}
    
            if(cw4 == "20"){ MangTTDen[1] = 0; CTStas = true;}
            if(cw4 == "21"){ MangTTDen[1] = 1; CTStas = true;}

            if(cw4 == "30"){ MangTTDen[2] = 0; CTStas = true;}
            if(cw4 == "31"){ MangTTDen[2] = 1; CTStas = true;}

            if(cw4 == "40"){ MangTTDen[3] = 0; CTStas = true;}
            if(cw4 == "41"){ MangTTDen[3] = 1; CTStas = true;}

            if(cw4 == "51"){ delay(1000); Restart(); }
        }
        ///////////////////
  }
  //////////////
  server.send(200, "text/html", LayChuoiIP(WiFi.localIP()));
}
/////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600); 
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500);  }
  client.setInsecure();
  myMac.replace(":", "");
  /////////////
  pinMode(Relay1, OUTPUT); 
  pinMode(Relay2, OUTPUT); 
  pinMode(Relay3, OUTPUT); 
  pinMode(Relay4, OUTPUT); 
  /////////////
  WiFi.softAP("CTTMUTS_" + myMac, "dmt@zzz@123");
  server.on("/", handleRoot);
  server.begin();
  /////////////
  //Serial.println(WiFi.localIP());
}
/////////////////////////////////////////////////////////////////////////////
void loop() 
{
   ////////////////////////////////////
   if(WiFi.status() == WL_CONNECTED) 
   { 
        params= "/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?id=" + myMac + "&stas=" + TTCongTac() + "&rows=" + Rows + "&ip=" + LayChuoiIP(WiFi.localIP());
        //Serial.println(params);
        GSData();
        ///////////////////
        if((params != TTCongTac()) && (params.length() == 5))
        { 
            CTStas       = true;
            MangTTDen[0] = params.substring(0,1).toInt();
            MangTTDen[1] = params.substring(1,2).toInt();
            MangTTDen[2] = params.substring(2,3).toInt();
            MangTTDen[3] = params.substring(3,4).toInt();
            if(params.substring(4,5) == "1"){ Restart(); } 
            //Serial.print(params.length());  Serial.print(" * "); Serial.print(params); Serial.println("");
        }
   }
   ////////////////////////////////////
   if(CTStas){ BatDen(); CTStas = false; }
   ////////////////////////////////////
   delay(100);
}
/////////////////////////////////////////////////////////////////
void GSData() 
{
  if (!client.connect("script.google.com", 443)) {   }   
  if (client.verify(fingerprint, "script.google.com")) {  }
  client.print(String("GET ") + params + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()) 
  {
    String lines = client.readStringUntil('\n');
    int8_t ind   = lines.indexOf("/macros/echo?user");
    if(ind > 0){ params = lines.substring(ind); break;  }
  }
  ////////////////////////////
  if (!client.connect("script.google.com", 443)) {  }   
  if (client.verify(fingerprint, "script.googleusercontent.com")) {  }
  client.print(String("GET ") + params + " HTTP/1.1\r\nHost: script.googleusercontent.com\r\nConnection: close\r\n\r\n");
  while (client.connected()){  params = client.readStringUntil('\n'); break; }
  while (client.available()){  params = client.readStringUntil('\n');  }
  params.trim();
}
