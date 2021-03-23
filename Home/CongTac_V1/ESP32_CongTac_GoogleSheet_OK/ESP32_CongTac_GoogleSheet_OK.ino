#include <WiFi.h>
#include <IPAddress.h>
#include <WebServer.h>
WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
#include <WiFiClientSecure.h>
WiFiClientSecure client;
//////////////////////////////////////////////////////////////////////////////////
#define Relay1  35
#define Relay2  34
#define Relay3  33
#define Relay4  32
uint8_t MangTTDen[4] = {0,0,0,0};
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
//////////////////////////////////////////////////////////////////////////////////
int  Rows      = 0;
boolean CTStas = true;
boolean UpStas = false;
String myMac   = WiFi.macAddress();
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include <EEPROM.h>
//////////////////////////////////////////////////////////////////////////////////
void LuuEEPROM(uint8_t nDiaChi, uint8_t nGiaTri){ EEPROM.write(nDiaChi, nGiaTri); EEPROM.commit(); delay(20); }
//////////////////////////////////////////////////////////////////////////////////
uint8_t LoadEEPROM_(uint8_t DiaChi){ return EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
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
      if((tmpChar > 31) && (tmpChar < 127)){ ChuoiDoc+= String(tmpChar); } else { break; }
    }
    return ChuoiDoc;
}
////////////////////////////////////////////////////////////////////////////////// Name - 30
void    Wifi_LuuName(String _WName){ EEPROM_WS(21, _WName); }
String  Wifi_LoadName(){ return EEPROM_RS(21, 30); }
////////////////////////////////////////////////////////////////////////////////// Pass - 20
void Wifi_LuuPass(String _WPass){ EEPROM_WS(51, _WPass); }
String  Wifi_LoadPass(){ return EEPROM_RS(51, 20); }
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
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void PageIndex() 
{
  ////////////////////////////////////////////////////////////////////////
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
  //////////////////////
  if (server.hasArg("wname"))
  {    
    String wname = server.arg("wname").c_str(); 
    if(wname != ""){ Wifi_LuuName(wname); } 
    Serial.println(wname);
    delay(1000);
    Restart();
  }
  //////////////////////
  String OnlineIP = "";
  if(WiFi.status() == WL_CONNECTED) { OnlineIP = LayChuoiIP(WiFi.localIP()); }
  //////////////////////
  server.send(200, "text/html", OnlineIP );
}
//////////////////////////////////////////////////////////////////////////////////
void PageSetup() 
{
  if (server.hasArg("rbtr_wname"))
  {    
    String wname = server.arg("rbtr_wname").c_str(); 
    if(wname != ""){ Wifi_LuuName(wname); } 
    Serial.println(wname);
  }
  ///////////////////////
  if (server.hasArg("rbtr_wpass"))
  {    
    String wpass = server.arg("rbtr_wpass").c_str(); 
    if(wpass != ""){ Wifi_LuuPass(wpass); } 
    Serial.println(wpass);
  }
  ///////////////////////
  ///////////////////////
  if (server.hasArg("rbtr_rs"))
  {    
    String rs = server.arg("rbtr_rs").c_str(); 
    Serial.println(rs);
    if(rs == "ok"){ delay(2000); Restart(); }
  }
  ///////////////////////
  server.send(200, "text/html", "" );
}
//////////////////////////////////////
boolean WifiNameSearch (String sName)
{
   uint8_t n = WiFi.scanNetworks();
   for (uint8_t i = 0; i < n; ++i){ if(sName == WiFi.SSID(i)){ return true; } delay(100); }
   return false;
}
//////////////////////////////////////////////////////////////////////////////////
String GoogleSheet_Data( String sURL)
{
   if (!client.connect("script.google.com", 443)) {   }   
   client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
   ////////////////////////////
   while (client.connected()) 
   {
     String lines = client.readStringUntil('\n');
     int8_t ind   = lines.indexOf("https://");
     if(ind > 0){ sURL = lines.substring(ind); break;  }
   }
   client.stop();
   ////////////////////////////
   return HTTPS_Data("script.google.com", sURL);;
}
//////////////////////////////////////////////////////////////////////////////////
String HTTPS_Data(char* Host, String sURL)
{
   String GT2  = "", GT3 = "";
   ////////////////////////////
   if (!client.connect(Host, 443)) {   }   
   client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: " + Host + "\r\nConnection: close\r\n\r\n");
   ////////////////////////////
   while (client.connected()){ sURL = client.readStringUntil('\n');  break; }
   while (client.available()){ GT3  = GT2; GT2  = sURL; sURL = client.readStringUntil('\n'); }
   ////////////////////////////
   client.stop();  sURL.trim();
   if(sURL == ""){ if( GT2.toInt() == 0 ){ sURL = GT3; }}
   ////////////////////////////
   return sURL;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  /////////////
  
  myMac.replace(":", "");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(String2Char("RBTRC20_" + myMac) , "dmt@zzz@123");     // RBTRC20_  UTS_SW4_
  /////////////
  String WifiName  = Wifi_LoadName();
  String WifiPass  = Wifi_LoadPass();
  if(WifiNameSearch(WifiName))
  {
     uint8_t nConn    = 0;
     boolean DaKetNoi = true;
     WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
     //////////////////
     while (WiFi.status() != WL_CONNECTED)
     {
        nConn++; delay(500); 
        if(nConn == 12){ DaKetNoi = false; break; }
     }
     ////////////////
     if (DaKetNoi){ WiFi.mode(WIFI_STA); Serial.println(""); Serial.println(WiFi.localIP()); }
  }
  /////////////
  pinMode(Relay1, OUTPUT); 
  pinMode(Relay2, OUTPUT); 
  pinMode(Relay3, OUTPUT); 
  pinMode(Relay4, OUTPUT); 
  /////////////
  server.on("/"     , PageIndex);
  server.on("/setup", PageSetup);
  server.begin();
}
//////////////////////////////////////////////////////////////////////////////////
void loop()
{
  server.handleClient();
  String TTCT   = TTCongTac();
  String Params = "/macros/s/AKfycbwyBgei2vkaCQmMG_o1VzU0GnbrhQZpSYibsuvWpSYm-GFUfB5W/exec?id=" + myMac  + "&stas=" + TTCT + "&rows=" + Rows + "&up=" + UpStas + "&ip=" + LayChuoiIP(WiFi.localIP()) ;
  Serial.println(Params);
  ///////////////
  if(WiFi.status() == WL_CONNECTED) 
  { 
      
      String aData  = GoogleSheet_Data(Params);
      ///////////////
      if(UpStas){ UpStas = false; }
      uint8_t nData = aData.length();
      ///////////////
      if( Params.substring(0,4) != TTCT )
      {
            CTStas       = true;
            MangTTDen[0] = aData.substring(0,1).toInt();
            MangTTDen[1] = aData.substring(1,2).toInt();
            MangTTDen[2] = aData.substring(2,3).toInt();
            MangTTDen[3] = aData.substring(3,4).toInt();
            if(aData.substring(4,5) == "1"){ Restart(); } 
            Rows = aData.substring(5,nData).toInt();
            Serial.print(aData.length());  Serial.print(" * "); Serial.print(aData); Serial.println("");
      }
      ///////////////
      Serial.println(aData);
  }
  ///////////////
  delay(500);
}
//////////////////////////////////////////////////////////////////////////////////
