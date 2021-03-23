#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
#include <Servo.h>
Servo myServo;
//////////////////////////////////////////////////////////////////////////////////
#define DCA  0 //  motor A direction
#define PWMA 5 //  motor A speed

#define DCB  2 //  motor B direction
#define PWMB 4 //  motor B speed

int TocDo = 7;
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
  if (server.hasArg("rbtr"))
  {    
    String getVal = server.arg("rbtr").c_str(); 
    if(getVal =="toi")  { Xe_Toi();        }
    if(getVal =="lui")  { Xe_Lui();        }
    if(getVal =="ngung"){ Xe_Ngung();      }
    if(getVal =="trai") { Xe_Trai();       }
    if(getVal =="phai") { Xe_Phai();       }
    if(getVal =="dong") { ThungRac_Dong(); }
    if(getVal =="mo")   { ThungRac_Mo();   }
  }
  ////////
  if (server.hasArg("rbtr_speed"))
  {    
    String SpeedVal = server.arg("rbtr_speed").c_str(); 
    if(SpeedVal != ""){ TocDo  = SpeedVal.toInt(); }
  }
  ///////////////////////
  server.send(200, "text/html", "<div align=center>" + Wifi_LoadName() + " : " + TocDo + "</div>" );
}
//////////////////////////////////////////////////////////////////////////////////
void PageSetup() 
{
  if (server.hasArg("rbtr_wname"))
  {    
    String wname = server.arg("rbtr_wname").c_str(); 
    if(wname != ""){ Wifi_LuuName(wname); } 
  }
  ///////////////////////
  if (server.hasArg("rbtr_wpass"))
  {    
    String wpass = server.arg("rbtr_wpass").c_str(); 
    if(wpass != ""){ Wifi_LuuPass(wpass); } 
  }
  ///////////////////////
  ///////////////////////
  if (server.hasArg("rbtr_rs"))
  {    
    String rs = server.arg("rbtr_rs").c_str(); 
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
  /////////////
  WiFi.mode(WIFI_AP);
  WiFi.softAP("RBTRC20_" + myMac, "dmt@zzz@123");
  /////////////
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
  pinMode(PWMA, OUTPUT); 
  pinMode(DCA, OUTPUT); 
  pinMode(PWMB, OUTPUT); 
  pinMode(DCB, OUTPUT); 
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  /////////////
  myServo.attach(16);
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
void ThungRac_Mo()  {  myServo.write(180); }
void ThungRac_Dong(){  myServo.write(0); }

void Xe_Toi()  { DongCoA_Toi();   DongCoB_Toi();   }
void Xe_Ngung(){ DongCoA_Ngung(); DongCoB_Ngung(); }
void Xe_Lui()  { Xe_Toi(); DongCoA_Lui();  DongCoB_Lui(); }
void Xe_Trai() { Xe_Toi(); DongCoA_Lui();  DongCoB_Toi(); }
void Xe_Phai() { Xe_Toi(); DongCoA_Toi();  DongCoB_Lui(); }
/////////////////////////////////////////////////////////////////////////////
void DongCoA_Lui()
{
  analogWrite(PWMA, (TocDo*1023)/10); 
  digitalWrite(DCA, LOW); 
}
/////////////////////////
void DongCoA_Toi()
{
  analogWrite(PWMA, (TocDo*1023)/10); 
  digitalWrite(DCA, HIGH);
}
/////////////////////////
void DongCoA_Ngung(){ analogWrite(PWMA, 0); }
/////////////////////////////////////////////////////////////////////////////
void DongCoB_Lui()
{
  analogWrite(DCB, (TocDo*1023)/10); 
  digitalWrite(DCB, LOW); 
}
/////////////////////////
void DongCoB_Toi()
{
  analogWrite(PWMB, (TocDo*1023)/10); 
  digitalWrite(DCB, HIGH); 
}
/////////////////////////
void DongCoB_Ngung(){  analogWrite(PWMB, 0); }
/////////////////////////////////////////////////////////////////////////////
