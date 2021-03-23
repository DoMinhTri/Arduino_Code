#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClientSecure client;

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
const char* ssid     = "Robo-MT";
const char* password = "dmt@123abc";
//////////////////////////////////////////////////////////////////////////////////  Relay
#define Relay1  16
#define Relay2  14
#define Relay3  12
#define Relay4  13
uint8_t MangTTDen[4] = {0,0,0,0};
//////////////////////////////////////////////////////////////////////////////////
uint16_t Rows  = 0;
boolean CTStas = true;
boolean UpStas = false;
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
  /////////////////////
  myMac.replace(":", "");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("CTTMUTS_" + myMac, "dmt@zzz@123");
  /////////////
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500);  }
  client.setInsecure();
  /////////////
  pinMode(Relay1, OUTPUT); 
  pinMode(Relay2, OUTPUT); 
  pinMode(Relay3, OUTPUT); 
  pinMode(Relay4, OUTPUT); 
  /////////////
  server.on("/", handleRoot);
  server.begin();
  /////////////
  Serial.println(WiFi.localIP());
}
/////////////////////////////////////////////////////////////////////////////
void loop() 
{
   ////////////////////////////////////
   if(WiFi.status() == WL_CONNECTED) 
   { 
        String params = "/macros/s/AKfycbwyBgei2vkaCQmMG_o1VzU0GnbrhQZpSYibsuvWpSYm-GFUfB5W/exec?id=" + myMac + "&stas=" + TTCongTac() + "&rows=" + Rows + "&up=" + UpStas + "&ip=" + LayChuoiIP(WiFi.localIP());
        String aData  = GoogleSheetData(params);
        uint8_t nData = aData.length() ;
        ///////////////////////////////
        if(nData > 5)
        {
            if(aData.substring(4,5) == "1"){ Restart(); } 
            if(Rows == 0){  Rows = aData.substring(5,nData).toInt(); }
            ///////////////////////////////
            if( aData.substring(0,4) != TTCongTac())
            { 
                CTStas       = true;
                MangTTDen[0] = aData.substring(0,1).toInt();
                MangTTDen[1] = aData.substring(1,2).toInt();
                MangTTDen[2] = aData.substring(2,3).toInt();
                MangTTDen[3] = aData.substring(3,4).toInt();
                Serial.println(aData);  
            }
        }
        ////////////////////////////////////
   }
   ////////////////////////////////////
   if(CTStas){ BatDen(); CTStas = false; }
   ////////////////////////////////////
   delay(100);
}
/////////////////////////////////////////////////////////////////
String GoogleSheetData(String sURL) 
{
  String GT2  = "", GT3 = "";
  ////////////////////////////
  if (!client.connect("script.google.com", 443)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()) 
  {
    String lines = client.readStringUntil('\n');
    int8_t ind   = lines.indexOf("https://");
    if(ind > 0){ sURL = lines.substring(ind); break;  }
  }
  ////////////////////////////
  if (!client.connect("script.google.com", 443)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()){  sURL = client.readStringUntil('\n');  break; }
  while (client.available())
  { 
    GT3  = GT2; 
    GT2  = sURL; 
    sURL = client.readStringUntil('\n');
  }
  //Serial.print(GT3 + "-" + GT2 + "-"); Serial.println(sURL);
  sURL.trim();
  if(sURL == ""){ if( GT2.toInt() == 0 ){ sURL = GT3; sURL.trim(); }}
  ////////////////////////////
  return sURL;
}
