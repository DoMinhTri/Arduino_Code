#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClientSecure client;

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
String Params                 = "";
const char* ssid              = "Robo-T";
const char* password          = "dmt@123abc";
const String GOOGLE_SCRIPT_ID = "AKfycbwyBgei2vkaCQmMG_o1VzU0GnbrhQZpSYibsuvWpSYm-GFUfB5W"; 
//////////////////////////////////////////////////////////////////////////////////  Relay
#define Relay1  16
#define Relay2  14
#define Relay3  12
#define Relay4  13
uint8_t MangTTDen[4] = {0,0,0,0};
//////////////////////////////////////////////////////////////////////////////////
int  Rows       = 0;
boolean CTStas  = true;
boolean UpStas  = false;
String myMac    = WiFi.softAPmacAddress();
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include <EEPROM.h>
void EEPROM_WS(uint8_t DiaChi, String ChuoiLuu)
{
  char* ChuoiTam = String2Char( ChuoiLuu );
  uint8_t DoDai  = ChuoiLuu.length();
  for (uint8_t i = 0; i < DoDai ; i++){ EEPROM.write(DiaChi + i,ChuoiTam[i]); }
  EEPROM.write(DiaChi + DoDai,255);
  delete ChuoiTam;
  EEPROM.commit();
}
//////////////////////////////////////////////////////////////////////////////////
String EEPROM_RS(uint8_t BatDau, uint8_t DoDai)
{
    String ChuoiDoc = "";
    for(uint8_t i = BatDau; i< (BatDau + DoDai); i++) 
    { 
      char tmpChar = char(EEPROM.read(i));
      if((tmpChar > 31) && (tmpChar < 127)){ ChuoiDoc+= String(tmpChar); }
    }
    return ChuoiDoc;
}
////////////////////////////////////////////////////////////////////////////////// Name - 30
void    Wifi_LuuName(String _WName){ EEPROM_WS(21, _WName); }
String  Wifi_LoadName(){ return EEPROM_RS(21, 30); }
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
  String TTDen =  String(MangTTDen[0]) +  String(MangTTDen[1]) +  String(MangTTDen[2]) +  String(MangTTDen[3]) ;
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
            if(cw4 == "10"){ MangTTDen[0] = 0; CTStas = true; UpStas  = true; }
            if(cw4 == "11"){ MangTTDen[0] = 1; CTStas = true; UpStas  = true; }
    
            if(cw4 == "20"){ MangTTDen[1] = 0; CTStas = true; UpStas  = true; }
            if(cw4 == "21"){ MangTTDen[1] = 1; CTStas = true; UpStas  = true; }

            if(cw4 == "30"){ MangTTDen[2] = 0; CTStas = true; UpStas  = true; }
            if(cw4 == "31"){ MangTTDen[2] = 1; CTStas = true; UpStas  = true; }

            if(cw4 == "40"){ MangTTDen[3] = 0; CTStas = true; UpStas  = true; }
            if(cw4 == "41"){ MangTTDen[3] = 1; CTStas = true; UpStas  = true; }

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
  WiFi.softAP("UTS_SW4_" + myMac, "dmt@zzz@123");
  server.on("/", handleRoot);
  server.begin();
  /////////////
  //Serial.println(WiFi.localIP());
}
/////////////////////////////////////////////////////////////////////////////
void loop() 
{
   server.handleClient();
   ////////////////////////////////////
   if(WiFi.status() == WL_CONNECTED) 
   { 
        String TTCT = TTCongTac();
        Params      = "/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?id=" + myMac  + "&stas=" + TTCT + "&rows=" + Rows + "&up=" + UpStas+ "&ip=" + LayChuoiIP(WiFi.localIP()) ;
        Serial.println(Params);
        GSData();
        Serial.println(Params);
        if(UpStas){ UpStas = false; }
        ///////////////////
        uint8_t nParam = Params.length();
        if( nParam > 5)
        { 
            if( Params.substring(0,4) != TTCT )
            {
                CTStas       = true;
                MangTTDen[0] = Params.substring(0,1).toInt();
                MangTTDen[1] = Params.substring(1,2).toInt();
                MangTTDen[2] = Params.substring(2,3).toInt();
                MangTTDen[3] = Params.substring(3,4).toInt();
                if(Params.substring(4,5) == "1"){ Restart(); } 
                Rows = Params.substring(5,nParam).toInt();
                Serial.print(Params.length());  Serial.print(" * "); Serial.print(Params); Serial.println("");
            }
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
  String GT2  = "", GT3 = "";
  ////////////////////////////
  if (!client.connect("script.google.com", 443)) {   }   
  client.print(String("GET ") + Params + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()) 
  {
    String lines = client.readStringUntil('\n');
    int8_t ind   = lines.indexOf("https://");
    if(ind > 0){ Params = lines.substring(ind); break;  }
  }
  ////////////////////////////
  if (!client.connect("script.google.com", 443)) {   }   
  client.print(String("GET ") + Params + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()){  Params = client.readStringUntil('\n');  break; }
  while (client.available())
  { 
    GT3    = GT2; 
    GT2    = Params; 
    Params = client.readStringUntil('\n');
  }
  //Serial.print(GT3 + "-" + GT2 + "-"); Serial.println(sURL);
  Params.trim();
  if(Params == ""){ if( GT2.toInt() == 0 ){ Params = GT3; }}
  ////////////////////////////
  //return sURL;
}
