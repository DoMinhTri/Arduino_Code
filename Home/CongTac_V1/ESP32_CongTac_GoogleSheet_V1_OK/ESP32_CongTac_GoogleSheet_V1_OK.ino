#include "OTA_Local.h"
//////////////////////////////////////////////////////////////////////////////////
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////
uint8_t Version  = 1;
uint8_t PortNum  = 4;
String ListSSID  = "";    // List wifi tim thay
String APName    = "";    // Access point Name
uint16_t Rows    = 0;     // Dong luu data tren Google Sheet
boolean TimWifi  = false; // So snh wifi luu va wifi tim thay
boolean UpStas   = false; // Update Stast tu device len server, de cap nhat len server
String myMac     = WiFi.macAddress();
//////////////////////////////////////////////////////////////////////////////////
// Function có sử dụng nên nằm sau các biến khai báo
#include "EEPROM.h"
#include "Functions.h"
#include "OTA_Online.h"
#include "GoogleSheet.h"
#include "GPIO.h"
#include "Face_Control.h"
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  Serial.println("");
  EEPROM.begin(512);
  delay(100);
  /////////////// GPIO.h
  IOSetup();  
  ///////////// 
  myMac.replace(":", "");
  APName = "UTS_SG4_" + myMac;  
  Serial.println(APName);
  /////////////       
  String WifiName = Wifi_LoadName(); // EEPROM.h
  String WifiPass = Wifi_LoadPass(); // EEPROM.h
  WifiNameSearch(WifiName);          // Function.h
  /////////////
  if(TimWifi)
  {
     uint8_t nConn = 0;
     WifiConnect(WifiName, WifiPass);                           // Function.h
     while (WiFi.status() != WL_CONNECTED){ nConn++; delay(500); if(nConn == 12){  WiFi.disconnect(); delay(1000); break; }}
  }
  if (WiFi.status() != WL_CONNECTED){ Accesspoint(APName); }    // Function.h
  /////////////
  OTA_Function();                                               // OTA_Local.h
  server.on("/"     , PageIndex);                               // Face_Control.h
  server.on("/setup", PageSetup);                               // Face_Control.h
  server.begin();                                               // OTA_Local.h
}
//////////////////////////////////////////////////////////////////////////////////
String UartData = "";
void loop()
{
  /////////////// Server run
  server.handleClient();
  ///////////////
  while (Serial.available() > 0 ) 
  {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ UartData+= KyTu; } delay(5);
  }
  if(UartData != ""){ SuLyGiaTriChuoiNhan(UartData); UartData = ""; } // Function.h
  ///////////////
  if(WiFi.status() == WL_CONNECTED) 
  { 
      LedOn();
      String TTCT   = TTCongTac();              // GPIO.h
      String Params = "/macros/s/AKfycbwyBgei2vkaCQmMG_o1VzU0GnbrhQZpSYibsuvWpSYm-GFUfB5W/exec?id=" + myMac  + "&stas=" + TTCT + "&rows=" + Rows + "&up=" + UpStas+ "&ip=" + LayChuoiIP(WiFi.localIP()) + "&version=" + String(Version) + "&nport=" + String(PortNum) ;
      String aData  = GoogleSheet_Data(Params); // GoogleSheet.h
      uint8_t nData = aData.length();
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
          if(aData.substring(5,6) == "1"){ OnlineUpdate(); }  // OTA_Online.h
          ///////////////
      }
  }else{ LedOff(); }
  ///////////////
  delay(500);
}
//////////////////////////////////////////////////////////////////////////////////
