
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);


// ESP8266 GPIO pins
#define GPIO2  2
#define GPIO0  0
#define GPIO5  5
#define GPIO4  4
#define GPIO15 15

// Thông tin cấu hình
// Access point
const char *ap_ssid     = "Minh Tri";
const char *ap_password = "12345678";
// Wifi
String WifiName, WifiPass, WifiIP, WifiPort, WifiGetway, AdminName, AdminPass, AdminIP;
// Tiêu đề và End HTML 
String Header    = "<html><head><title>IoT  Viet Nam</title></head> <body style='background-color:#c0c0c0' > ";
String EndHeader = "</body></html>";

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
   delay(10);
}
//////////////////////////////////////////////////////////////////////////////////
uint8_t LoadEEPROM(int nDiaChi)
{
   delay(10);
   return EEPROM.read(nDiaChi);
}
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){ EEPROM.write( DiaChi, KyTu );  EEPROM.commit(); delay(20); }
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
  for(int i = _BatDau; i< _BatDau + _DoDai; i++){ EEPROM.write( i,245); delay(10); }
  EEPROM_WS(_BatDau, _ChuoiLuu);
}
//////////////////////////////////////////////////////////////////////////////////
String LoadVungDuLieu(int _BatDau, int _DoDai)
{
  String _strChuoi = "";
  for(int i = _BatDau; i< _BatDau + _DoDai; i++)
  { 
      char tmpChar = EEPROM_RC(i);
      if(tmpChar != 245){ _strChuoi+=  (String)tmpChar ; }
  }
  return _strChuoi;
}
////////////////////////////////////////////////////////////////////////////////// Name
void Wifi_LuuName(String _WName){ LuuVungDuLieu(21, 30, _WName); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadName(){ return LoadVungDuLieu(21, 30); }
////////////////////////////////////////////////////////////////////////////////// Pass
void Wifi_LuuPass(String _WPass){ LuuVungDuLieu(51, 20, _WPass); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadPass(){ return LoadVungDuLieu(51, 20); }
////////////////////////////////////////////////////////////////////////////////// IP
void Wifi_LuuIP(String _WIP){ LuuVungDuLieu(71, 15, _WIP); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadIP(){ return LoadVungDuLieu(71, 15); }
////////////////////////////////////////////////////////////////////////////////// Wifi Port
void Wifi_LuuPort(String _WPort){ LuuVungDuLieu(86, 5, _WPort); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadPort(){ return LoadVungDuLieu(86, 5); }
////////////////////////////////////////////////////////////////////////////////// Getway
void Wifi_LuuGetway(String _WGW){ LuuVungDuLieu(91, 15, _WGW); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadGetway(){ return LoadVungDuLieu(91, 15); }
////////////////////////////////////////////////////////////////////////////////// Admin Name
void Admin_LuuName(String _AName){ LuuVungDuLieu(106, 30, _AName); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadName(){ return LoadVungDuLieu(106, 30); }
////////////////////////////////////////////////////////////////////////////////// Admin Pass
void Admin_LuuPass(String _APass){ LuuVungDuLieu(136, 20, _APass); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadPass(){ return LoadVungDuLieu(136, 20); }
////////////////////////////////////////////////////////////////////////////////// Admin IP
void Admin_LuuIP(String _AIP){ LuuVungDuLieu(156, 15, _AIP); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadIP(){ return LoadVungDuLieu(156, 15); }
//////////////////////////////////////////////////////////////////////////////////
void LuuThongTinCaiDat()
{
  Wifi_LuuName( WifiName );   
  Wifi_LuuPass( WifiPass );   
  Wifi_LuuIP( WifiIP ); 
  Wifi_LuuPort( WifiPort );  
  Wifi_LuuGetway( WifiGetway );
  Admin_LuuName( AdminName ); 
  Admin_LuuPass( AdminPass ); 
  Admin_LuuIP( AdminIP ); 
}

void LoadThongTinCaiDat()
{
  WifiName   = Wifi_LoadName(); 
  WifiPass   = Wifi_LoadPass();
  WifiIP     = Wifi_LoadIP();
  WifiPort   = Wifi_LoadPort();
  WifiGetway = Wifi_LoadGetway();
  
  AdminName  = Admin_LoadName();
  AdminPass  = Admin_LoadPass();
  AdminIP    = Admin_LoadIP();
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      Text function      ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}

int String2Int(String sSo)
{
  return sSo.toInt();
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////   Web function   ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// document.getElementById('CbbTuoiNhanh').value = 1; 
// document.getElementById('NutTuoi').innerHTML
String JavaScriptUser()
{
    String JVScript = "<script>\
                     var nTimerdown = 0;\
                     function Desktop(){ SendURL('/desktop'); ReloadWebTimer(2000); }\
                     function SendURL(URL){  var xhttp = new XMLHttpRequest(); xhttp.open('GET', URL, true); xhttp.send();}\
                     function OpenLink(URL){ window.location.href = URL ; }\
                     function ReloadWeb(){ SendURL('/refesh'); window.location.reload(); }\
                     function ReloadWebTimer(_timer){ setTimeout('ReloadWeb()',_timer); }\
                     function HoatDong(_value){ SendURL('/hoatdong/?T:' + _value + '$' ); }\
                     function TuoiNhanh(_value){ var url = '/tuoinhanh/?T:' + _value + '$'; SendURL(url); nTimerdown = -1;  setTimeout('KiemTraTuoiNhanh(' + _value + ')',1000);  }\
                     function KiemTraTuoiNhanh(nTime){ nTimerdown = nTime*60; ThoiGianLui(); }\
                     function ThoiGianLui(){ if(nTimerdown>=0){ var phut = parseInt(nTimerdown/60); var giay = nTimerdown%60; document.getElementById('TimerDown').innerHTML = phut + ':' + giay ; nTimerdown--; var sVal = 'ThoiGianLui('+nTimerdown+')'; setTimeout(sVal,1000); }} \
                     function GetWifiName(_Value){ document.getElementById('WifiName').value = _Value; }\
                     function GPIO_Control( _Name, _Pin){\
                          var Value_Name = document.getElementById(_Name).value;\
                          var PinOn  = '/?' + _Pin + '=1' ; \
                          var PinOff = '/?' + _Pin + '=0' ; \
                          if(Value_Name == 'On')  { SendURL(PinOff); document.getElementById(_Name).value = 'Off'; } \
                          if(Value_Name == 'Off') { SendURL(PinOn);  document.getElementById(_Name).value = 'On';  } \
                      }\
                     </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String Javascript()
{
  String JVScript = "<script>\
                     function AnNutSave()  { document.getElementById('btSave').style.visibility = 'hidden'; }\ 
                     function HienNutSave(){ document.getElementById('btSave').style.visibility = 'visible'; }\ 
                     function LuuSetting(){\
                        var wname   = document.getElementById('wifiname').value;\
                        var wpass   = document.getElementById('wifipass').value;\
                        var wip     = document.getElementById('wifiip').value;\
                        var wgetway = document.getElementById('wifigetway').value;\
                        var admname = document.getElementById('admname').value;\
                        var admpass = document.getElementById('admpass').value;\
                        var admip   = document.getElementById('admip').value;\
                        var admport = document.getElementById('admport').value;\
                        if(wname !='' && wpass != '' && wip !='' && wgetway != '' && admname !='' && admpass != '' && admip !='' && admport != ''){\
                          var URL = '/dmtLuuCaiDat/?ABCX' + wname + '\,' + wpass + '\,' + wip + '\,' + wgetway + '\,' + admname + '\,' + admpass + '\,' + admip + '\,' + admport + '\,$';\
                          SendURL(URL);  AnNutSave(); setTimeout('HienNutSave()',10000);\
                          document.getElementById('dmtkk').value = URL ;\
                        }else{alert('Error input !'); return ;}\
                     }</script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String CStyleCSS()
{
   String Button = "<style type='text/css'>\
                    .shadow{ text-shadow: 2px 2px 5px #000000;}\
                    body{background: url( http://wallpoper.com/images/00/45/05/47/green-background-2_00450547.jpg ) fixed;}\
                    .bogoc{ border-radius: 25px;}\
                    .textinput{width:100%; height:100px;font-size:50px}\
                    .button{background: #73AD21; font-size:60px; text-align: center; }\
                    .tieude{color:blue;font-size:80px}\
                    .radio{width:50px; height:50px;}\
                    .text{color:white;font-size:60px}\
                    .TimerDown{color:blue;font-size:80px}\
                    .ComBoBox{width:200px; font-size:60px; }\
                    .myButton {-moz-box-shadow:inset 0px 1px 0px 0px #97c4fe;-webkit-box-shadow:inset 0px 1px 0px 0px #97c4fe;box-shadow:inset 0px 1px 0px 0px #97c4fe;background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #3d94f6), color-stop(1, #1e62d0));background:-moz-linear-gradient(top, #3d94f6 5%, #1e62d0 100%);background:-webkit-linear-gradient(top, #3d94f6 5%, #1e62d0 100%);background:-o-linear-gradient(top, #3d94f6 5%, #1e62d0 100%);background:-ms-linear-gradient(top, #3d94f6 5%, #1e62d0 100%);background:linear-gradient(to bottom, #3d94f6 5%, #1e62d0 100%);filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#3d94f6', endColorstr='#1e62d0',GradientType=0);background-color:#3d94f6;-moz-border-radius:14px;-webkit-border-radius:14px;border-radius:14px;border:1px solid #337fed;display:inline-block;cursor:pointer;color:#ffffff;font-family:Arial;font-size:40px;font-weight:bold;padding:25px 50px;text-decoration:none;text-shadow:0px 1px 0px #1570cd;}\
                    .myButton:hover {background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #1e62d0), color-stop(1, #3d94f6));background:-moz-linear-gradient(top, #1e62d0 5%, #3d94f6 100%);background:-webkit-linear-gradient(top, #1e62d0 5%, #3d94f6 100%);background:-o-linear-gradient(top, #1e62d0 5%, #3d94f6 100%);background:-ms-linear-gradient(top, #1e62d0 5%, #3d94f6 100%);background:linear-gradient(to bottom, #1e62d0 5%, #3d94f6 100%);filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#1e62d0', endColorstr='#3d94f6',GradientType=0);background-color:#1e62d0;}\
                    .myButton:active {position:relative;top:1px;}\
                    </style>";
  return Button;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void KiemTraTrangThaiDen(String _Name, uint8_t _Pin )
{
  if (server.hasArg(_Name))
  { 
    if (server.arg(_Name) == "1"){  digitalWrite(_Pin, HIGH); }
    else{ digitalWrite(_Pin, LOW);  }
  }
}
//////////////////////////////////////////////////////////////////////////////////
void CaiDat_GPIO()
{
    KiemTraTrangThaiDen("gpio2",  GPIO2  );
    KiemTraTrangThaiDen("gpio0",  GPIO0  );
    KiemTraTrangThaiDen("gpio5",  GPIO5  );
    KiemTraTrangThaiDen("gpio4",  GPIO4  );
    KiemTraTrangThaiDen("gpio15", GPIO15 );
}
//////////////////////////////////////////////////////////////////////////////////
String GiaoDienNutBam()
{
  String HTML ="<div align=center>\
                          <table border='0' cellspacing='5'>\
                              <tr>\
                                  <td > <a class='text'> Light 1 </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut1'  onclick=\"GPIO_Control('Nut1','gpio2')\"> </td>\
                              </tr>\
                              <tr>\
                                  <td > <a class='text'> Light 2 </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut2'  onclick=\"GPIO_Control('Nut2','gpio0')\"> </td>\
                              </tr>\
                              <tr>\
                                  <td > <a class='text'> Light 3 </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut3'  onclick=\"GPIO_Control('Nut3','gpio4')\"> </td>\
                              </tr>\
                              <tr>\
                                  <td > <a class='text'> Light 4 </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut4'  onclick=\"GPIO_Control('Nut4','gpio5')\"> </td>\
                              </tr>\                                                                                          
                          </table>\
                      </div>";
  return HTML;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Desktop() 
{
  
  CaiDat_GPIO();
  String HTML  = Header + JavaScriptUser() + CStyleCSS();
         HTML += GiaoDienNutBam();
  HTML += "<center><a href='/admin' class='tieude'> WLAN settings </a></center>";
  HTML += EndHeader;
  server.send(200, "text/html", HTML);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void WifiConnect()
{
  
  String HTML  = Header + JavaScriptUser() + CStyleCSS() + "<h1 class='tieude' ><center>WLAN Settings</center></h1>";

  int ap_count = WiFi.scanNetworks();
  if (ap_count == 0)
  {
       HTML += "No access points found.<br>";
  }
  else
  {
    HTML += "<form method=\"get\">";
    for (uint8_t ap_idx = 0; ap_idx < ap_count; ap_idx++)
    {
      HTML += "<input type='radio' name='ssid'  class='text radio' value=''  onclick=\"GetWifiName('" + String(WiFi.SSID(ap_idx)) + "')\"> <text class='text shadow' > ";
      HTML += String(WiFi.SSID(ap_idx)) + " (" + (100+ WiFi.RSSI(ap_idx)) +"%)";
      (WiFi.encryptionType(ap_idx) == ENC_TYPE_NONE) ? HTML += " " : HTML += "<font color='red'>*</font> </text>";
      HTML += "<p>";
    }
    
    HTML += "<input type='hidden' id='WifiName' value=''>";
    HTML += "<text class='text shadow' style='color:red'>(*) Password: </text><br>";
    HTML += "<input type='text' name='password'   class='textinput' ><br><br>";
    HTML += "<input type='submit' value='Connect' class='myButton'>";
    HTML += "</form>";
  }
  HTML += EndHeader;
  server.send(200, "text/html", HTML);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void LoiKoThayTrang()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(GPIO2,  OUTPUT);
  pinMode(GPIO0,  OUTPUT);
  pinMode(GPIO5,  OUTPUT);
  pinMode(GPIO4,  OUTPUT);
  pinMode(GPIO15, OUTPUT);
  /////////////////////////////
  Serial.begin(115200);
  Serial.println("Configuring access point...");
  // Cài đặt chế độ password hoặc không password 
  WiFi.softAP(ap_ssid, ap_password);  //   WiFi.softAP(ap_ssid); 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  // Các trang mặc định 
  server.on("/",      Desktop);
  server.on("/wifi", WifiConnect);
  server.onNotFound(LoiKoThayTrang);
  server.begin();
  Serial.println(WifiPass);
  Serial.println("HTTP server started");
}

void loop() 
{
  server.handleClient();
}
