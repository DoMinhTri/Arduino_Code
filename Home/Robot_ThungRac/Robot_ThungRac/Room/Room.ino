#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
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
  uint8_t DoDai  = ChuoiLuu.length();
  for (uint8_t i = 0; i < DoDai ; i++){ EEPROM_WC(DiaChi + i,ChuoiTam[i]); }
  delete ChuoiTam;
  EEPROM_WC(DiaChi + DoDai, 255);
  EEPROM.commit();
}
//////////////////////////////////////////////////////////////////////////////////
String EEPROM_RS(uint8_t BatDau, uint8_t KetThuc)
{
    String ChuoiDoc = "";
    for(int i = BatDau; i< (BatDau + KetThuc); i++) 
    { 
       char tmpChar = (char)EEPROM.read(DiaChi);
      if((tmpChar > 31) && (tmpChar < 127)){ _strChuoi+= tmpChar ; }
      ChuoiDoc+= EEPROM_RC(i); 
    }
    return ChuoiDoc;
}
//////////////////////////////////////////////////////////////////////////////////
void LuuVungDuLieu(int _BatDau, int _DoDai, String _ChuoiLuu)
{
  //Serial.println(_ChuoiLuu);
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
  if (server.hasArg("rbtr"))
  {    
    String getVal = server.arg("rbtr").c_str(); 
    Serial.println(getVal);
  }
  ///////////////////////
  server.send(200, "text/html", "<div align=center>" + Wifi_LoadName() + "</div>" );
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
  //////////////////////
  String OnlineIP = "";
  if(WiFi.status() == WL_CONNECTED) { OnlineIP = LayChuoiIP(WiFi.localIP()); }
  ///////////////////////
  server.send(200, "text/html", OnlineIP );
}
//////////////////////////////////////
boolean WifiNameSearch (String sName)
{
   uint8_t n = WiFi.scanNetworks();
   boolean kiemtra = false;
   for (uint8_t i = 0; i < n; ++i) 
   {
      if(sName == WiFi.SSID(i)){ kiemtra = true;}
      delay(100);
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
  /////////////
  WiFi.mode(WIFI_AP);
  WiFi.softAP("RBTRC20_" + myMac, "dmt@zzz@123");
  /////////////
  //Wifi_LuuName("Robo-T");
  //Wifi_LuuPass("dmt@123abc");
  String WifiName = Wifi_LoadName();
  String WifiPass = Wifi_LoadPass();
  if(WifiNameSearch(WifiName))
  {
     WiFi.mode(WIFI_AP_STA);
     WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
     delay(4000);
  }
  /////////////
  server.on("/",      PageIndex);
  server.on("/setup", PageSetup);
  server.begin();
  /////////////
  Serial.println(""); 
  Serial.println(WifiName); 
  Serial.println(WifiPass); 
  Serial.println(WiFi.localIP()); 
}
/////////////////////////////////////////////////////////////////////////////
void loop() { 
   server.handleClient();
   ////////////////////////////////////
   delay(25);
}

/////////////////////////////////////////////////////////////////////////////
void ThungRac_Mo()  {  Serial.print("M"); }
void ThungRac_Dong(){  Serial.print("D"); }
void Xe_Toi()  { Serial.print("U"); }
void Xe_Ngung(){ Serial.print("N");  }
void Xe_Lui()  { Serial.print("L"); }
void Xe_Trai() { Serial.print("T");  }
void Xe_Phai() { Serial.print("P");  }
