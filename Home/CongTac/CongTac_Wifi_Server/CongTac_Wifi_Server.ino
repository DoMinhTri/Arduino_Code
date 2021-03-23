#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
String TTCongTac = "";
String UartData  = "";
String myMac     = "";
String LanIP     = ""; 
String sURL      = "";
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
void    Wifi_LuuName(String _WName){ LuuVungDuLieu(1, 30, _WName); }
String  Wifi_LoadName(){ return LoadVungDuLieu(1, 30); }
////////////////////////////////////////////////////////////////////////////////// Pass - 20
void    Wifi_LuuPass(String _WPass){ LuuVungDuLieu(31, 20, _WPass); }
String  Wifi_LoadPass(){ return LoadVungDuLieu(31, 20); }
////////////////////////////////////////////////////////////////////////////////// URL  - 50
void    Wifi_LuuURL(String _WUrl){ LuuVungDuLieu(51, 50, _WUrl); }
String  Wifi_LoadURL(){ return LoadVungDuLieu(51, 50); }
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
void PageIndex() {
  ///////////////////////
  if (server.hasArg("cw4"))
  {    
    Serial.print(server.arg("cw4").c_str()); 
  }
  ///////////////////////
  server.send(200, "text/html",  Wifi_LoadName() + "," + TTCongTac );
}
//////////////////////////////////////////////////////////////////////////////////
void PageSetup() 
{
  ///////////////////////
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
  if (server.hasArg("wrs"))
  {    
     String wrs = server.arg("wrs").c_str(); 
     if(wrs == "ok"){ delay(1000); Restart(); }
  }
 ///////////////////////
   if (server.hasArg("wurl"))
   {    
      String wurl = server.arg("wurl").c_str(); 
      if(wurl != "")
      { 
         Wifi_LuuURL(wurl); 
         delay(3000); 
         Restart();
      } 
   }
  //////////////////////
  server.send(200, "text/html", LanIP );
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
  EEPROM.begin(128);
  delay(100);
  myMac = WiFi.softAPmacAddress();
  myMac.replace(":", "");
  ///////////// ACCESS POINT
  WiFi.mode(WIFI_AP);
  WiFi.softAP("UTS_CW4_" + myMac, "dmt@zzz@123");
  ///////////// WIFI
  //String WifiName = Wifi_LoadName();
  //String WifiPass = Wifi_LoadPass();
//  if(WifiNameSearch(WifiName))
  {
     //WiFi.mode(WIFI_AP_STA);
     //WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
     WiFi.begin("Robo-T", "dmt@123abc");
     /////////////
     //delay(4000);
     /////////////
     //sURL  = Wifi_LoadURL();
     LanIP = LayChuoiIP(WiFi.localIP());
  }
  ///////////// SERVER
  server.on("/",      PageIndex);
  server.on("/setup", PageSetup);
  server.begin();
  /////////////
  Serial.println(LanIP);
}
/////////////////////////////////////////////////////////////////////////////

void loop() { 
   server.handleClient();
   //////////////////////////////////// Lay trang thai cong tac gui den ATMega
   String oData = LayDataLink();
   if(oData != ""){ TTCongTac = oData ; Serial.println(oData); }
   //////////////////////////////////// Load trang thai cong tac tu ATMega
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();
        if( 31<KyTu && KyTu < 127 ){ UartData+= KyTu; } delay(5);
   }
   /////
   if(UartData != "")
   { 
      TTCongTac = UartData; 
      UartData  = ""; 
   }
   ////////////////////////////////////
   delay(500);
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
String LayDataLink()
{
        HTTPClient http;
        String payload   = "";
        String SVLink    = sURL + "?wstas=" + TTCongTac + "&wip=" + LanIP + "&wmac=" + myMac;
         http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){  payload = http.getString();  }
        } 
        http.end();
        Serial.println(SVLink);
        //////////
        return payload;
}
/////////////////////////////////////////////////////////////////////////////
