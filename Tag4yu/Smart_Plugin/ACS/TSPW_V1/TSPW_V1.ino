#include <Arduino.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);
#define CongTac_Pin 2
//////////////////////
String myMac  = WiFi.softAPmacAddress();
String mySSID = "TAG4YU_SPW1";
String myPASS = "0918180212";
const uint8_t fingerprint[20] = { 0x86, 0x0D, 0xBC, 0xE8, 0x78, 0x33, 0xCA, 0xB7, 0x6B, 0x5F, 0x05, 0x88, 0x23, 0xC2, 0x3F, 0x61, 0xA2, 0xB0, 0xAD, 0xE5 };
String sUrl = "https://nfc2cloud.com/iot/?id=02E2009FD8A8AA";
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
////////////////////////////////////////////////////////////////////////////////// TrangThai : 1 - 1
void Luu_TrangThai(String _WName){ LuuVungDuLieu(1, 1, _WName);    }
////////////////////////////////////////////////////////////////////////////////// 
String Load_TrangThai(){ return LoadVungDuLieu(1, 1); }

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
void WifiLogonStop() { Luu_TrangThai("0"); }
void WifiLogonStart(){ Luu_TrangThai("1"); }

void Restart()     {  Serial.println("MCU_RESTART"); delay(1000); ESP.restart(); }
void CallRestart() { if (server.hasArg("restart")){ String  sStas = server.arg("restart").c_str();  if( sStas == "ok"){   Restart(); }}}
//////////////////////
void handleRoot() {
  if (server.hasArg("led"))
  { 
    String  myled = server.arg("led").c_str(); 
    if(myled == "on")  digitalWrite(CongTac_Pin, 1);
    if(myled == "off") digitalWrite(CongTac_Pin, 0);
     Serial.println(myled);
  }
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

//////////////////////////////////////////////////////////////////////////////////
void Setting()
{
  String tagssid = "";
  String tagpass = "";
  String tagsend = "";

  if (server.hasArg("tag4yu_ssid")){ tagssid = server.arg("tag4yu_ssid").c_str(); }
  if (server.hasArg("tag4yu_pass")){ tagpass = server.arg("tag4yu_pass").c_str(); }
  if (server.hasArg("tag4yu_send")){ tagsend = server.arg("tag4yu_send").c_str(); }

  if(tagssid != "" && tagsend == "ok")
  {
     Wifi_LuuName(tagssid);
     Wifi_LuuPass(tagpass);
     WifiLogonStart();
  }
  if(tagsend == "ok") Restart();
  server.send(200, "text/html", myMac);
}
////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
////////////////////////////////////////////////////

void setup() {
  
  Serial.begin(9600);
  EEPROM.begin(512);
  delay(100);
  myMac.replace(":", "");
  mySSID = "TAG4YU_SPW1_" + myMac;
  
  pinMode(CongTac_Pin, OUTPUT);
  //WiFi.mode(WIFI_AP_STA);  // WIFI_AP
  //sUrl   = "https://nfc2cloud.com/iot/tspw1/";
  Serial.println("");
  /////////////
  if(Load_TrangThai() == "1")
  {
      WiFi.mode(WIFI_STA);
      mySSID = Wifi_LoadName();
      myPASS = Wifi_LoadPass();
      WiFiMulti.addAP(String2Char(mySSID) , String2Char(myPASS));
      //WiFi.begin(String2Char(mySSID) , String2Char(myPASS));
      delay(5000);
      Serial.println("Online");
      Serial.println(WiFi.localIP());
  }
  else
  {
      WiFi.mode(WIFI_AP);
      WiFi.softAP(String2Char(mySSID) , String2Char(myPASS));
      delay(5000);
      IPAddress myIP = WiFi.softAPIP();
      Serial.println("Access point");
  }
  
  server.on("/"       , handleRoot);
  server.on("/setting", Setting);
  server.begin();
  ////////////////////
}
String TTCongTac = "";
void loop() {
  server.handleClient();
  ////////////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ TTCongTac+= KyTu; } delay(5);
  }
  if(TTCongTac == "restart") { Restart(); }
  if(TTCongTac == "reset")   { WifiLogonStop(); delay(100); Restart(); }
 
  if(WiFiMulti.run() == WL_CONNECTED) 
  {
    String html   = GetHTTPSData(sUrl);
    String PinOut = html.substring(0,1);

    if(PinOut == "1") { Serial.println("Led on");  } 
    if(PinOut == "0") { Serial.println("Led off"); }
    
  }
  
  delay(200);
}

////////////////////////////////////////////////////////
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
      else 
      {
          payload = https.errorToString(httpCode).c_str();
      }

      https.end();
    }
    else {  payload = "[HTTPS] Unable to connect\n";   }

    return payload;
}
////////////////////////////////////////////////////////
