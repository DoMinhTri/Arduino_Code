

// 0xE5,0x2E,0x56,0x9D,0x01,0xB0,0x88,0xF9,0x12,0x63,0x4E,0xD8,0x32,0x51,0xA9,0xBC,0x32,0x4C,0x0C,0x0E  // fingerprint : https/nfc2cloud.com/ssl.php (SHA1)
//  Tag4yu_Guest - 62605606
//********** CHANGE PIN FUNCTION  TO GPIO **********
//GPIO 1 (TX) swap the pin to a GPIO.
//GPIO 3 (RX) swap the pin to a GPIO.
//**************************************************

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
ESP8266WiFiMulti WiFiMulti;

String myMac     = WiFi.softAPmacAddress();
uint8_t LedStas  = 2      ;
String sUrl      = ""     ;
String mySSID    = ""     ;
String myPASS    = ""     ;
String myOnIP    = "noip" ;
boolean TimWifi  = false  ; 
uint8_t cGio = 0, cPhut = 0, cGiay = 0;
////////////////////////////////////////////////////////
#define GPIO2  2
void Led_On()  {  LedStas = 1; digitalWrite(GPIO2, LOW);  }
void Led_Off() {  LedStas = 2; digitalWrite(GPIO2, HIGH); }

const uint8_t fingerprint[20] = { 0xE5,0x2E,0x56,0x9D,0x01,0xB0,0x88,0xF9,0x12,0x63,0x4E,0xD8,0x32,0x51,0xA9,0xBC,0x32,0x4C,0x0C,0x0E  };
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      EEPROM      ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include <EEPROM.h>
void LuuEEPROM(int nDiaChi, uint8_t nGiaTri){  EEPROM.write(nDiaChi, nGiaTri);   EEPROM.commit();   delay(20); }
//////////////////////////////////////////////////////////////////////////////////
uint8_t LoadEEPROM(int nDiaChi){  delay(10);  return EEPROM.read(nDiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){ EEPROM.write( DiaChi, KyTu );  delay(20);  }//Serial.print("Address: "); Serial.print(DiaChi); Serial.print(" - Char: "); Serial.println(KyTu); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, uint8_t SoLuu){ LuuEEPROM(DiaChi, SoLuu); }
//////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
uint8_t EEPROM_RN(int DiaChi){ return EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WS(int DiaChi, String ChuoiLuu)
{
  char* ChuoiTam = String2Char( ChuoiLuu );
  int DoDai  = ChuoiLuu.length();
  for (int i = 0; i < DoDai ; i++){ EEPROM_WC(DiaChi + i,ChuoiTam[i]); }
  delete ChuoiTam;
  EEPROM.commit();
}
//////////////////////////////////////////////////////////////////////////////////
String EEPROM_RS(int BatDau, int KetThuc)
{
    String ChuoiDoc = "";
    for(int i = BatDau; i< (BatDau + KetThuc); i++) { ChuoiDoc+= EEPROM_RC(i); }
    return ChuoiDoc;
}
//////////////////////////////////////////////////////////////////////////////////
void LuuVungDuLieu(int _BatDau, int _DoDai, String _ChuoiLuu)
{
  for(int i = _BatDau; i< _BatDau + _DoDai; i++){ EEPROM.write( i,255); delay(10); }
  EEPROM_WS(_BatDau, _ChuoiLuu);
}
//////////////////////////////////////////////////////////////////////////////////
String LoadVungDuLieu(int _BatDau, int _DoDai)
{
  String _strChuoi = "";
  for(int i = _BatDau; i< _BatDau + _DoDai; i++)
  { 
      char tmpChar = EEPROM_RC(i);
      if((tmpChar > 31) && (tmpChar < 127)){ _strChuoi+=  (String)tmpChar ; }
  }
  return _strChuoi;
}
////////////////////////////////////////////////////////////////////////////////// TrangThai : 2 - 1
void Luu_TrangThai(uint8_t TT){ LuuEEPROM(2, TT); }
////////////////////////////////////////////////////////////////////////////////// 
uint8_t Load_TrangThai(){ return LoadEEPROM(2);   }
////////////////////////////////////////////////////////////////////////////////// Name : 21 - 30
void Wifi_LuuName(String _WName){ LuuVungDuLieu(21, 30, _WName); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadName(){ return LoadVungDuLieu(21, 30); }
////////////////////////////////////////////////////////////////////////////////// Pass : 51 - 20
void Wifi_LuuPass(String _WPass){ LuuVungDuLieu(51, 20, _WPass); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadPass(){ return LoadVungDuLieu(51, 20); }
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void SerialPrint(String sText)  { Serial.print(sText);    }
void SerialPrintLn(String sText){ Serial.println(sText);  }

void Restart()     {  SerialPrintLn("MCU_RESTART"); delay(1000); ESP.restart(); }
void CallRestart() { if (server.hasArg("restart")){ String  sStas = server.arg("restart").c_str();  if( sStas == "ok"){   Restart(); }}}
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
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String tagssid = "";
String tagpass = "";
String StrHTML = "";
void InputPage()
{
  String tagrstr = "";
  String tagleds = "";
  boolean Reset  = false;
  ////////////////////////
  if (server.hasArg("tag4yu_ssid")){ tagssid = server.arg("tag4yu_ssid").c_str(); SerialPrintLn(tagssid); Wifi_LuuName(tagssid); }
  if (server.hasArg("tag4yu_pass")){ tagpass = server.arg("tag4yu_pass").c_str(); SerialPrintLn(tagpass); Wifi_LuuPass(tagpass); }
  if (server.hasArg("tag4yu_leds")){ tagleds = server.arg("tag4yu_leds").c_str(); SerialPrintLn(tagleds); }
  if (server.hasArg("tag4yu_rstr")){ tagrstr = server.arg("tag4yu_rstr").c_str(); SerialPrintLn(tagrstr); }
  ////////////////////////
  if(tagleds == "on") { Led_On();  }
  if(tagleds == "off"){ Led_Off(); }
  if(tagrstr == "ok") { Restart(); }
  //////////////////////// Trường họp đã cài đặt mạng
  if(myOnIP != "noip"  )
  {
     if((tagssid == mySSID) && (tagpass == myPASS))
     {  StrHTML = tagssid; }
     else 
     { 
        if((tagssid != "") && (tagpass != ""))
        {
          StrHTML = tagssid; 
          Reset   = true; 
        }
        else{  StrHTML = "noip"; }
     }
     /////////////
  }
  else
  {
      if((tagssid != "") && (tagpass != "")){ StrHTML = tagssid; Reset = true; }
  }
  ////////////////////////
  server.send(200, "text/html", StrHTML);
  //////////////////////// 
  if(Reset){  delay(6000); Restart(); }
  //////////////////////// 
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void WifiSetup()
{
   WiFi.mode(WIFI_AP_STA);
   uint8_t SoLanReConn = 0;
   uint8_t nWifi = WiFi.scanNetworks();
   if(nWifi > 0)
   {
       mySSID = Wifi_LoadName();
       myPASS = Wifi_LoadPass();
       for(uint8_t i=0; i < nWifi; i++ )
       {
          if(mySSID == WiFi.SSID(i))
          {
              TimWifi = true ;
              WiFiMulti.addAP(String2Char(mySSID) , String2Char(myPASS));
              SerialPrintLn("Wifi - " + mySSID + " - " + myPASS);
              while( WiFiMulti.run() != WL_CONNECTED ) 
              {  
                 delay(500); SoLanReConn++; 
                 if(SoLanReConn > 12) break; 
              }
           }
       }
       ///////////
       if(TimWifi){  if( WiFiMulti.run() != WL_CONNECTED ) { WiFi.disconnect(); } }
   }
}
//////////////////////////////////////////////////////////////////////////////////
void AccessPointSetup()
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("TAG4YU_SPW1_" + myMac, "0918180212");
    //IPAddress myIP = WiFi.softAPIP();
    //SerialPrintLn("Access point");
}
//////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  EEPROM.begin(128);
  Serial.println("");
  delay(100);
  /////////////
  myMac.replace(":", "");
  delay(1000);
  /////////////
  pinMode(GPIO2,  OUTPUT);
  Led_On();
  /////////
  WifiSetup();
  AccessPointSetup();
  /////////
  server.on("/input", InputPage);
  server.begin();
  /////////
}
//////////////////////////////////////////////////////////////////////////////////
String UartData   = "";
uint8_t ConnCount = 0;

void loop() {
   
  server.handleClient();
  sUrl = "https://nfc2cloud.com/spw1/?id=" + myMac + "&stas=" + LedStas + "&dvip=" + myOnIP;
  ///////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ UartData+= KyTu; } delay(5);
  }
  if(UartData == "restart")   { Restart();         }
  if(UartData == "disconnect"){ WiFi.disconnect(); }
  ///////////////////////
  if( WiFiMulti.run() == WL_CONNECTED ) 
  {
       ConnCount = 0;
       myOnIP    = LayChuoiIP(WiFi.localIP());
       uint8_t Denled = GetHTTPSData(sUrl).substring(0,1).toInt();
       
       if(Denled == 1) { Led_On();  }
       if(Denled == 2) { Led_Off(); }
       if(Denled == 3) { Restart(); }
       Serial.println( myOnIP + " - " + Denled); 
  } 
  else 
  { 
    ConnCount++;
    myOnIP = "noip";  
    if(ConnCount > 240 ){ ConnCount = 0; WifiSetup();  }
  }
  ///////////////////////
  TinhThoiGianReset();
  ///////////////////////
  UartData = "";
  delay(1000);
}
//////////////////////////////////////////////////////////////////////////////////
void TinhThoiGianReset()
{
  cGiay++;
  if(cGiay == 60){ cGiay = 0; cPhut++; }
  if(cPhut == 60){ cPhut = 0; cGio++;  }
  if( cGio > 23) { Restart(); }
}
//////////////////////////////////////////////////////////////////////////////////
String GetHTTPSData(String sUrl)
{
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;
    String payload = "";
    if (https.begin(*client, sUrl)) 
    { 
      int httpCode = https.GET();
      if (httpCode > 0)
      {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {  payload = https.getString();   }
      }
      //else{  payload = https.errorToString(httpCode).c_str();  }
      https.end();
    }
    //else {  payload = "[HTTPS] Unable to connect\n";   }
    return payload;
}
//////////////////////////////////////////////////////////////////////////////////
