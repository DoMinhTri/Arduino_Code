
//  0xE5,0x2E,0x56,0x9D,0x01,0xB0,0x88,0xF9,0x12,0x63,0x4E,0xD8,0x32,0x51,0xA9,0xBC,0x32,0x4C,0x0C,0x0E   // fingerprint https/nfc2cloud.com (SHA1)

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

String myMac  = WiFi.softAPmacAddress();
String mySSID = "TAG4YU_SPW1_";
String myPASS = "0918180212";
////////////////////////////////////////////////////////
#define GPIO2  2

void Led_On() {  digitalWrite(GPIO2, HIGH); } // Serial.println("Led 2 on");  }
void Led_Off(){  digitalWrite(GPIO2, LOW);  } // Serial.println("Led 2 off"); }

const uint8_t fingerprint[20] = { 0xE5,0x2E,0x56,0x9D,0x01,0xB0,0x88,0xF9,0x12,0x63,0x4E,0xD8,0x32,0x51,0xA9,0xBC,0x32,0x4C,0x0C,0x0E };
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
void Luu_TrangThai(String _WName){ LuuVungDuLieu(2, 1, _WName);    }
////////////////////////////////////////////////////////////////////////////////// 
String Load_TrangThai(){ return LoadVungDuLieu(2, 1); }
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
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void handleRoot() {
  if (server.hasArg("led"))
  { 
    String  myled = server.arg("led").c_str(); 
    if(myled == "on")  Led_On();
    if(myled == "off") Led_Off();
     Serial.println(myled);
  }
  server.send(200, "text/html", "ok");
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

  Serial.println(tagssid);
  Serial.println(tagpass);
  Serial.println(tagsend);

  if(tagssid != "" && tagsend == "ok")
  {
     Wifi_LuuName(tagssid);
     Wifi_LuuPass(tagpass);
     WifiLogonStart();
     Serial.println("OK Send");
     Restart();
  }

  server.send(200, "text/html", myMac);
}
//////////////////////////////////////////////////////////////////////////////////
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
void AccessPointSetup()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(String2Char(mySSID + myMac), String2Char(myPASS));
    delay(5000);
    //IPAddress myIP = WiFi.softAPIP();
    Serial.println("Access point");
}
//////////////////////////////////////////////////////////////////////////////////
void WifiSetup()
{
      WiFi.mode(WIFI_STA);
      mySSID = Wifi_LoadName();
      myPASS = Wifi_LoadPass();
      WiFiMulti.addAP(String2Char(mySSID) , String2Char(myPASS));
      delay(5000);
      Serial.println("Online");
      Serial.println(WiFi.localIP());
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
  digitalWrite(GPIO2, LOW);
  /////////
  if(Load_TrangThai() == "1")
  {
      WifiSetup();
  }
  else
  {
     AccessPointSetup();
     server.on("/setting", Setting);
  }
  /////////
  server.on("/", handleRoot);
  server.begin();
  /////////
  sUrl = "https://nfc2cloud.com/spw1/?id=" + myMac;
  Serial.println(myMac);
  /////////
}
//////////////////////////////////////////////////////////////////////////////////
String TTCongTac = "";
void loop() {
   
  server.handleClient();
  ///////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ TTCongTac+= KyTu; } delay(5);
  }
  if(TTCongTac == "restart"){ Restart(); }
  if(TTCongTac == "reset")  { WifiLogonStop(); delay(100); Restart(); }
  ///////////////////////
  if (WiFiMulti.run() == WL_CONNECTED) 
  {
    uint8_t Denled = GetHTTPSData(sUrl).substring(0,1).toInt();
    if(Denled == 0) { Led_Off(); }
    if(Denled == 1) { Led_On();  } 
    if(Denled == 2) { WifiLogonStop(); delay(100); Restart(); }
    Serial.println(Denled);
  }
  ///////////////////////
  delay(1000);
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
      else 
      {
          payload = https.errorToString(httpCode).c_str();
      }
      https.end();
    }
    else {  payload = "[HTTPS] Unable to connect\n";   }

    return payload;
}
//////////////////////////////////////////////////////////////////////////////////
