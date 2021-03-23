#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;
//////////////////////////////////////////////////////////////////////////////////
#include <WiFiClientSecure.h>
WiFiClientSecure client;
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
uint8_t Version  = 1;
uint8_t PortNum  = 4;     // So cong su dung cua thiet bi
int  Rows        = 0;     // Dong luu data tren Google Sheet
boolean WiSearch = false; // So sanh ten wifi luu va wifi tim thay
boolean UpStas   = false; // Update Stast tu device len server, de cap nhat len server
//////////////////////////////////////////////////////////////////////////////////
String WifiName  = "";
String WifiPass  = "";
String ListSSID  = "";
String myMac     = WiFi.macAddress();
/////////////////////////////////////////////////////////////////////////////
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
void BatDen()
{
    digitalWrite(Relay1, MangTTDen[0]); 
    digitalWrite(Relay2, MangTTDen[1]); 
    digitalWrite(Relay3, MangTTDen[2]); 
    //uint8_t nStas = (MangTTDen[3] + 1)%2;
    digitalWrite(Relay4, MangTTDen[3]); 
}
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
void WifiNameSearch()
{
   ListSSID  = "";
   WiSearch  = false;
   uint8_t n = WiFi.scanNetworks();
   ///////////////
   for (uint8_t i = 0; i < n; ++i)
   { 
      if(WifiName == WiFi.SSID(i)){ WiSearch = true; }
      ListSSID = ListSSID + WiFi.SSID(i) + "," ;
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
void SuLyBLEData(String sData)
{
  String sDDau = sData.substring(0, 5);
  String sDSau = sData.substring(6, sData.length());
  if(sDDau == "GETWL"){  WifiNameSearch(); SerialBT.print(ListSSID); }
  if(sDDau == "WNAME"){ Wifi_LuuName(sDSau); }
  if(sDDau == "WPASS"){ Wifi_LuuPass(sDSau); }
  if(sDDau == "RESET"){ Restart(); }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  myMac.replace(":", "");
  WifiName  = Wifi_LoadName();
  WifiPass  = Wifi_LoadPass();
  ////////////////////////
  WifiNameSearch();
  /////////////
  if(WiSearch)
  {
     uint8_t nConn = 0;
     WiFi.mode(WIFI_STA);
     WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
     /////////////// Kiem tra ket noi trong 6s
     while (WiFi.status() != WL_CONNECTED){ nConn++; delay(500); if(nConn == 12){ break; } }
  }
  /////////////
  if (WiFi.status() == WL_CONNECTED){ SerialBT.begin("UTS_SW4_" + myMac); }
  /////////////
  pinMode(Relay1 , OUTPUT); 
  pinMode(Relay2 , OUTPUT); 
  pinMode(Relay3 , OUTPUT); 
  pinMode(Relay4 , OUTPUT);
  /////////////
}
//////////////////////////////////////////////////////////////////////////////////
String BLEData = "";
void loop()
{
  ///////////////
  while (SerialBT.available() > 0 ) 
  {
        char KyTu = SerialBT.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ BLEData+= KyTu; } delay(5);
  }
  if(BLEData != ""){ SuLyBLEData(BLEData); BLEData = ""; }
  ///////////////
  if(WiFi.status() == WL_CONNECTED) 
  { 
      String TTCT   = TTCongTac();
      String GLink  = "/macros/s/AKfycbwyBgei2vkaCQmMG_o1VzU0GnbrhQZpSYibsuvWpSYm-GFUfB5W/exec?id=" + myMac  + "&stas=" + TTCT + "&rows=" + Rows + "&up=" + UpStas+ "&ip=" + LayChuoiIP(WiFi.localIP()) + "&version=" + String(Version) + "&nport=" + String(PortNum) ;
      String aData  = GoogleSheet_Data(GLink);
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
          }
          ///////////////
          if(UpStas){ UpStas = false; }
          //if(aData.substring(5,6) == "1"){ OnlineUpdate(); } 
      }
  }
  ///////////////
  delay(500);
}
