#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
/////////////////////  Relay
#define Relay1  16
#define Relay2  14
#define Relay3  12
#define Relay4  13
uint8_t MangTTDen[4] = {0,0,0,0};
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
void PageStas() {
  server.send(200, "text/html", TTCongTac() );
}
//////////////////////////////////////////////////////////////////////////////////
void PageIndex() {
  ///////////////////////
  if (server.hasArg("cw4"))
  {    
    String cw4 = server.arg("cw4").c_str(); 
    if(cw4 == "10"){ MangTTDen[0] = 0; }
    if(cw4 == "11"){ MangTTDen[0] = 1; }
    
    if(cw4 == "20"){ MangTTDen[1] = 0; }
    if(cw4 == "21"){ MangTTDen[1] = 1; }

    if(cw4 == "30"){ MangTTDen[2] = 0; }
    if(cw4 == "31"){ MangTTDen[2] = 1; }

    if(cw4 == "40"){ MangTTDen[3] = 0; }
    if(cw4 == "41"){ MangTTDen[3] = 1; }
  }
  ///////////////////////
  String sTime = String(Nam) + "/" + String(Thang) + "/" +  String(Ngay) + " " +  String(Gio) + ":" +  String(Phut) + ":" +  String(Giay);
  server.send(200, "text/html", "<div align=center>" + Wifi_LoadName() + "<br>" + sTime + "</div>" );
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
  ///////////// ACCESS POINT
  WiFi.mode(WIFI_AP);
  WiFi.softAP("UTS_CW4_" + myMac, "dmt@zzz@123");
  ///////////// WIFI
  String WifiName = Wifi_LoadName();
  String WifiPass = Wifi_LoadPass();
  if(WifiNameSearch(WifiName))
  {
     IPAddress wip     = IPAddress(192,168,1,100);
     IPAddress wgetway = IPAddress(192,168,1,1);
     IPAddress Subnet  = IPAddress(255,255,255,0);
     /////////////
     WiFi.mode(WIFI_AP_STA);
     WiFi.config( wip, wgetway, Subnet);
     WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
     /////////////
     delay(4000);
  }
  ///////////// SERVER
  server.on("/",      PageIndex);
  server.on("/stas" , PageStas);
  server.on("/setup", PageSetup);
  server.begin();
  /////////////
  pinMode(Relay1, OUTPUT); 
  pinMode(Relay2, OUTPUT); 
  pinMode(Relay3, OUTPUT); 
  pinMode(Relay4, OUTPUT); 
  /////////////
  Serial.println(WifiName); 
  Serial.println(WifiPass); 
  Serial.println(WiFi.localIP()); 
}
/////////////////////////////////////////////////////////////////////////////
void loop() { 
   server.handleClient();

   BatDen();
   
   String wc4 = LayDataLink();
   if(wc4 == "10"){ MangTTDen[0] = 0; }
   if(wc4 == "11"){ MangTTDen[0] = 1; }
    
   if(wc4 == "20"){ MangTTDen[1] = 0; }
   if(wc4 == "21"){ MangTTDen[1] = 1; }

   if(wc4 == "30"){ MangTTDen[2] = 0; }
   if(wc4 == "31"){ MangTTDen[2] = 1; }

   if(wc4 == "40"){ MangTTDen[3] = 0; }
   if(wc4 == "41"){ MangTTDen[3] = 1; }

   if(wc4 == "reset"){ Restart(); }
   ////////////////////////////////////
   delay(300);
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
String TTCongTac()
{
  String TTDen =  String(MangTTDen[0]) + "," + String(MangTTDen[1]) + "," + String(MangTTDen[2]) + "," + String(MangTTDen[3]);
  return TTDen;
}
////////////////////////////////////////////////////////////////////////////////////
String LayDataLink()
{
        HTTPClient http;
        String payload   = "";
        String SVLink    = "http://192.168.1.4/home/congtac/index.php" ;
         http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){  payload = http.getString();  }
        } 
        http.end();
        //////////
        return payload;
}
//////////////////////////////////////////////////////////////////////////////////
void BatDen()
{
    digitalWrite(Relay1, MangTTDen[0]); 
    digitalWrite(Relay2, MangTTDen[1]); 
    digitalWrite(Relay3, MangTTDen[2]); 
    digitalWrite(Relay4, MangTTDen[3]); 
}
/////////////////////////////////////////////////////////////////////////////
