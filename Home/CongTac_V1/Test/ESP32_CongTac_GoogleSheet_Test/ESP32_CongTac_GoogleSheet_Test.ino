#include <WiFi.h>
#include <IPAddress.h>
#include <WebServer.h>
WebServer server(80);
void Restart(){ ESP.restart(); }

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
uint8_t Version = 1;

void OnlineUpdate()
{
        t_httpUpdate_return ret = ESPhttpUpdate.update("http://uptosmart.ddns.net/sw4/esp32.bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                Serial.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                Serial.println("HTTP_UPDATE_OK");
                Restart();
                break;
        }
}
///////////////////////  OTA
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "OTA_Face.h"
//////////////////////////////////////////////////////////////////////////////////
#include <WiFiClientSecure.h>
WiFiClientSecure client;
//////////////////////////////////////////////////////////////////////////////////
#define LedConn  2
#define Relay1   34
#define Relay2   35
#define Relay3   33
#define Relay4   32
uint8_t MangTTDen[4] = {0,0,0,0};
/////////////////////////////////////////////////////////////////////////////
String TTCongTac()
{
  String TTDen =  String(MangTTDen[0]) +  String(MangTTDen[1]) +  String(MangTTDen[2]) +  String(MangTTDen[3]) ;
  return TTDen;
}
/////////////////////////////////////////////////////////////////////////////
void LedOn(){  digitalWrite(LedConn, HIGH); }
void LedOff(){ digitalWrite(LedConn, LOW);  }
/////////////////////////////////////////////////////////////////////////////
void BatDen()
{
    digitalWrite(Relay1, MangTTDen[0]); 
    digitalWrite(Relay2, MangTTDen[1]); 
    digitalWrite(Relay3, MangTTDen[2]); 
    uint8_t nStas = (MangTTDen[3] + 1)%2;
    digitalWrite(Relay4, nStas); 
}
//////////////////////////////////////////////////////////////////////////////////
uint8_t PortNum  = 4;
int  Rows        = 0;     // Dong luu data tren GOogle Sheet
boolean WiSearch = false; // So snh wifi luu va wifi tim thay
boolean UpStas   = false; // Update Stast tu device len server, de cap nhat len server
String ListSSID  = "";    // List wifi tim thay
String myMac     = WiFi.macAddress();
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
String LayChuoiIP(IPAddress _IP){   String _ChuoiIP = (String)_IP[0] + "." + (String)_IP[1] + "." + (String)_IP[2] + "." + (String)_IP[3] ;   return _ChuoiIP; }
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
            if(cw4 == "10"){ MangTTDen[0] = 0; UpStas  = true; }
            if(cw4 == "11"){ MangTTDen[0] = 1; UpStas  = true; }
    
            if(cw4 == "20"){ MangTTDen[1] = 0; UpStas  = true; }
            if(cw4 == "21"){ MangTTDen[1] = 1; UpStas  = true; }

            if(cw4 == "30"){ MangTTDen[2] = 0; UpStas  = true; }
            if(cw4 == "31"){ MangTTDen[2] = 1; UpStas  = true; }

            if(cw4 == "40"){ MangTTDen[3] = 0; UpStas  = true; }
            if(cw4 == "41"){ MangTTDen[3] = 1; UpStas  = true; }
            //////////
            if(cw4 == "51"){ delay(1000); Restart(); }
            //////////
            BatDen();
        }
        ///////////////////
  }
  //////////////////////
  if (server.hasArg("wname"))
  {    
    String wname = server.arg("wname").c_str(); 
    if(wname != ""){ Wifi_LuuName(wname); } 
    delay(2000);
    Restart();
  }
  //////////////////////
  ///if(WiFi.status() == WL_CONNECTED) { OnlineIP = LayChuoiIP(WiFi.localIP()); }
  server.send(200, "text/html", TTCongTac() );
}
//////////////////////////////////////////////////////////////////////////////////
void PageSetup() 
{
  String tpData = "";
  if (server.hasArg("sw4_wname")){    
    tpData = server.arg("sw4_wname").c_str(); 
    if(tpData != ""){ Wifi_LuuName(tpData); } 
  }
  ///////////////////////
  tpData = "";
  if (server.hasArg("sw4_wpass")){    
    tpData = server.arg("sw4_wpass").c_str(); 
    if(tpData != ""){ Wifi_LuuPass(tpData); } 
  }
  ///////////////////////
  tpData = "<!DOCTYPE html><html><head><title>UTS_CW4</title></head><body bgcolor='#FAFAFA'><script>function SendData(){var name = document.getElementById('Name').value; var pass = document.getElementById('Pass').value; var url = 'http://192.168.4.1/setup?sw4_wname=' + name + '&sw4_wpass=' + pass + '&sw4_rs=ok'; var xhttp = new XMLHttpRequest(); xhttp.open('GET', url, true); xhttp.send(); document.getElementById('textsms').innerHTML='Comblete !';} function getName(_Name){ document.getElementById('Name').value = _Name; }</script><style>body{margin:20px;} .style{width:100%;height:35px;} .bgcolor{background:#00ADEF;} .radio{width:25px;height:25px;} </style><table width='100%' cellspacing='1' cellpadding='1'><tr><td><input type='text' id='Name' class='style' placeholder='Wifi Name'></td></tr><tr><td><input type='text' id='Pass' class='style' placeholder='Wifi Pass'></td></tr><tr><td><input type='button' class='style bgcolor' value='Connect' onclick='SendData()'></td></tr></table><div align='center'><font size=3 color=red><b id='textsms'></b></font></div>" + ListSSID + "</body></html>";
  server.send(200, "text/html", tpData );
  ///////////////////////
  tpData = "";
  if (server.hasArg("sw4_rs")){    
    tpData = server.arg("sw4_rs").c_str(); 
    if(tpData == "ok"){ delay(2000); Restart(); }
  }
  ///////////////////////
}
//////////////////////////////////////
void WifiNameSearch()
{
   ListSSID  = "";
   WiSearch  = false;
   String WifiName = Wifi_LoadName();
   uint8_t n = WiFi.scanNetworks();
   ///////////////
   for (uint8_t i = 0; i < n; ++i)
   { 
      String tpSSID = WiFi.SSID(i);
      if(WifiName == tpSSID){ WiSearch = true; }
      if(myMac != tpSSID){ ListSSID += "<input type='radio' class='radio' name='wname' value='"+  tpSSID +"' onclick='getName(this.value)'><font size=2>" + tpSSID + "</font><br>"; }
      delay(100); 
   }
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
   if(sURL == ""){ if( GT2.toInt() == 0 ){  GT3.trim(); sURL = GT3; }}
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
  myMac.replace(":", "");
  String WifiName  = Wifi_LoadName();
  String WifiPass  = Wifi_LoadPass();
  /////////////
  WiFi.mode(WIFI_AP);
  WiFi.softAP(String2Char(myMac) , "dmt@xxx@123");
  /////////////
  WifiNameSearch();
  if(WiSearch)
  {
     uint8_t nConn = 0;
     WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
     //////////////////
     while (WiFi.status() != WL_CONNECTED){ nConn++; delay(500); if(nConn == 12){ break; }}
     ////////////////
     if (WiFi.status() != WL_CONNECTED){ WiFi.mode(WIFI_STA); Serial.println(""); Serial.println(WiFi.localIP()); }
  }
  /////////////
  pinMode(LedConn, OUTPUT); 
  pinMode(Relay1 , OUTPUT); 
  pinMode(Relay2 , OUTPUT); 
  pinMode(Relay3 , OUTPUT); 
  pinMode(Relay4 , OUTPUT); 
  /////////////
  OTA_Face();
  server.on("/"     , PageIndex);
  server.on("/setup", PageSetup);
  server.begin();
}
//////////////////////////////////////////////////////////////////////////////////
String ChuoiDuLieu = "";
void loop()
{
  server.handleClient();
  ///////////////
  while (Serial.available() > 0 ) 
  {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
  }
  if(ChuoiDuLieu != ""){ SuLyGiaTriChuoiNhan(ChuoiDuLieu); ChuoiDuLieu = ""; }
  ///////////////
  if(WiFi.status() == WL_CONNECTED) 
  { 
      LedOn();
      String TTCT   = TTCongTac();
      String Params = "/macros/s/AKfycbwyBgei2vkaCQmMG_o1VzU0GnbrhQZpSYibsuvWpSYm-GFUfB5W/exec?id=" + myMac  + "&stas=" + TTCT + "&rows=" + Rows + "&up=" + UpStas+ "&ip=" + LayChuoiIP(WiFi.localIP()) + "&version=" + String(Version) + "&nport=" + String(PortNum) ;
      String aData  = GoogleSheet_Data(Params);
      uint8_t nData = aData.length();
      //Serial.println(Params); 
      //Serial.println(aData); 
      ///////////////
      if(nData > 5)
      {
          if( Rows == 0){  Rows = aData.substring(6,nData).toInt(); }
          if(aData.substring(4,5) == "1"){ Restart(); } 
          ///////////////
          if( aData.substring(0,4) != TTCT )
          {
              MangTTDen[0] = aData.substring(0,1).toInt();
              MangTTDen[1] = aData.substring(1,2).toInt();
              MangTTDen[2] = aData.substring(2,3).toInt();
              MangTTDen[3] = aData.substring(3,4).toInt();
              BatDen();
              //Serial.print(""); Serial.println(aData);  
          }
          ///////////////
          if(UpStas){ UpStas = false; }
          if(aData.substring(5,6) == "1"){ OnlineUpdate(); } 
          ///////////////
          ///////////////
      }
  }else{ LedOff(); }
  ///////////////
  delay(500);
}
//////////////////////////////////////////////////////////////////////////////////
void SuLyGiaTriChuoiNhan(String sData)
{
  String sDDau = sData.substring(0, 5);
  String sDSau = sData.substring(6, sData.length());
  if(sDDau == "WNAME"){ Wifi_LuuName(sDSau); }
  if(sDDau == "WPASS"){ Wifi_LuuPass(sDSau); }
  if(sDDau == "RESET"){ Restart(); }
  Serial.println(sDSau);
}
//////////////////////////////////////////////////////////////////////////////////
void OTA_Face()
{
  if (!MDNS.begin(host)){ Serial.println("Error setting up MDNS responder!"); while(1){ delay(1000); }  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
//  server.on("/ota", HTTP_GET, []() {
//    server.sendHeader("Connection", "close");
//    server.send(200, "text/html", loginIndex);
//  });
  server.on("/ota", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
}
//////////////////////////////////////////////////////////////////////////////////
