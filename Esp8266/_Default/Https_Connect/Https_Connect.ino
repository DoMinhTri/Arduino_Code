#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;
//  echo | openssl s_client -connect nfc2cloud.com:443 | openssl x509 -fingerprint -noout
const uint8_t fingerprint[20] = { 0x57, 0xD6, 0xDB, 0x07, 0x17, 0x9F, 0x1C, 0xF8, 0xCF, 0x22, 0x8A, 0x4C, 0x1B, 0xB7, 0x7A, 0x73, 0x4D, 0xCC, 0xA0, 0xB4 };
String sUrl  = "https://nfc2cloud.com/iot/demo.php";
String myMac = WiFi.softAPmacAddress();
//////////////////////////////////////////////////////////////////////////////////
#define GPIO2 2
void Led_2_On() {  digitalWrite(GPIO2, LOW);   }
void Led_2_Off(){  digitalWrite(GPIO2, HIGH);  }
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      EEPROM      ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include <EEPROM.h>
void LuuEEPROM(int nDiaChi, uint8_t nGiaTri)
{
   EEPROM.write(nDiaChi, nGiaTri);
   EEPROM.commit();
   delay(20);
}
//////////////////////////////////////////////////////////////////////////////////
uint8_t LoadEEPROM(int nDiaChi){  delay(10);  return EEPROM.read(nDiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){ EEPROM.write( DiaChi, KyTu ); }  // delay(20);  Serial.print("Address: "); Serial.print(DiaChi); Serial.print(" - Char: "); Serial.println(KyTu); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){ LuuEEPROM(DiaChi, SoLuu); }
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
////////////////////////////////////////////////////////////////////////////////// Name - 30
void    Wifi_LuuName(String _WName){ LuuVungDuLieu(21, 30, _WName); }
String  Wifi_LoadName(){ return LoadVungDuLieu(21, 30); }
////////////////////////////////////////////////////////////////////////////////// Pass - 20
void    Wifi_LuuPass(String _WPass){ LuuVungDuLieu(51, 20, _WPass); }
String  Wifi_LoadPass(){ return LoadVungDuLieu(51, 20); }
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
    String getVal = server.arg("cw4").c_str(); 
  }
  ///////////////////////
  server.send(200, "text/html", "<div align=center>" + Wifi_LoadName() + "</div>" );
}
//////////////////////////////////////////////////////////////////////////////////
void PageSetup() 
{
  if (server.hasArg("wname"))
  {    
    String wname = server.arg("wname").c_str(); 
    if(wname != ""){ Wifi_LuuName(wname); } 
  }
  ///////////////////////
  if (server.hasArg("wpass"))
  {    
    String wpass = server.arg("wpass").c_str(); 
    if(wpass != ""){ Wifi_LuuPass(wpass); } 
  }
  ///////////////////////
  ///////////////////////
  if (server.hasArg("wrs"))
  {    
    String wrs = server.arg("wrs").c_str(); 
    if(wrs == "ok"){ delay(2000); Restart(); }
  }
  //////////////////////
  String OnlineIP = "";
  if(WiFi.status() == WL_CONNECTED) { OnlineIP = LayChuoiIP(WiFi.localIP()); }
  ///////////////////////
  server.send(200, "text/html", OnlineIP );
}
//////////////////////////////////////
boolean WifiNameSearch (String sName)
{
   int n = WiFi.scanNetworks();
   boolean kiemtra = false;
   for (int i = 0; i < n; ++i) 
   {
      if(sName == WiFi.SSID(i)){ kiemtra = true;}
      delay(100);
       Serial.println( WiFi.SSID(i)); 
   }
   return kiemtra;
}
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  EEPROM.begin(512);
  delay(100);
  String myMac = WiFi.softAPmacAddress();
  myMac.replace(":", "");
  /////////////UTS_CW4_
  WiFi.mode(WIFI_AP);
  WiFi.softAP("RBTRC20_" + myMac, "dmt@zzz@123");
  /////////////
  String WifiName = Wifi_LoadName();
  String WifiPass = Wifi_LoadPass();
  if(WifiNameSearch(WifiName))
  {
     WiFi.mode(WIFI_AP_STA);
     //WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
     WiFiMulti.addAP(String2Char(WifiName), String2Char(WifiPass));
     delay(4000);
  }
  /////////////
  server.on("/",      PageIndex);
  server.on("/setup", PageSetup);
  server.begin();
  /////////////
  pinMode(GPIO2,  OUTPUT);
  /////////////
  Serial.println(""); 
  Serial.print(WifiName + " - " + WifiPass); 
//  Serial.println(WiFiMulti.localIP()); 
}
/////////////////////////////////////////////////////////////////////////////
void loop() { 
   server.handleClient();
   ////////////////////////////////////
   if ((WiFiMulti.run() == WL_CONNECTED)) 
   {
     String data = GetHTTPSData(sUrl);
     Serial.println(data); 
     if(data == "1") { Led_2_On();  } 
     if(data == "0") { Led_2_Off(); }
     Serial.println(data);
   }
   ////////////////////////////////////
   delay(500);
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
      if (httpCode > 0){ if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){ payload = https.getString(); }}
      else{ payload = https.errorToString(httpCode).c_str(); }
      https.end();
    } else {  payload = "[HTTPS] Unable to connect\n"; }
    return payload;
}
/////////////////////////////////////////////////////////////////////////////
