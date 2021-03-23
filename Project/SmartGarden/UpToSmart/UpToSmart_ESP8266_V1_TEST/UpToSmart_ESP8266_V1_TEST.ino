
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <WiFiClient.h> 
#include <IPAddress.h>
#include <PubSubClient.h>

ESP8266WebServer server(80);
// ESP8266 GPIO pins
#define GPIO2  2
#define GPIO0  0
#define GPIO5  5
#define GPIO4  4
#define GPIO15 15
/////////////////////  
// Wifi
String ChuoiDuLieuMCU;
uint8_t RL1, RL2, RL3, RL4, SS1, SS2, SS3, SS4, NhietDo, TroiMua ;
String WifiName, WifiPass, WifiIP, WifiPort, WifiGetway, APName, APPass, APIP, AdmName, AdmPass;
// Tiêu đề và End HTML 
String Header    = "<html><head><title>IoT  Viet Nam</title></head> <body style='background-color:#c0c0c0' onload=''> ";
String EndHeader = "</body></html>";

void Restart(){ ESP.restart(); }
void CallRestart(){ if (server.hasArg("restart")){ String  sStas = server.arg("restart").c_str();  if( sStas == "ok"){ Serial.println(""); Serial.print("Restart"); Restart(); }}}
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
void EEPROM_WC(int DiaChi, char KyTu){ EEPROM.write( DiaChi, KyTu );  delay(20);  Serial.print("Address: "); Serial.print(DiaChi); Serial.print(" - Char: "); Serial.println(KyTu); }
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
void Wifi_LuuName(String _WName){ LuuVungDuLieu(21, 30, _WName); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadName(){ return LoadVungDuLieu(21, 30); }
////////////////////////////////////////////////////////////////////////////////// Pass - 20
void Wifi_LuuPass(String _WPass){ LuuVungDuLieu(51, 20, _WPass); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadPass(){ return LoadVungDuLieu(51, 20); }
////////////////////////////////////////////////////////////////////////////////// IP - 15
void Wifi_LuuIP(String _WIP){ LuuVungDuLieu(71, 15, _WIP); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadIP(){ return LoadVungDuLieu(71, 15); }
////////////////////////////////////////////////////////////////////////////////// Wifi Port - 5
void Wifi_LuuPort(String _WPort){ LuuVungDuLieu(86, 5, _WPort); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadPort(){ return LoadVungDuLieu(86, 5); }
////////////////////////////////////////////////////////////////////////////////// Getway - 15
void Wifi_LuuGetway(String _WGW){ LuuVungDuLieu(91, 15, _WGW); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadGetway(){ return LoadVungDuLieu(91, 15); }
////////////////////////////////////////////////////////////////////////////////// AP Name - 30
void AP_LuuName(String _APName){ LuuVungDuLieu(106, 30, _APName); }
//////////////////////////////////////////////////////////////////////////////////
String AP_LoadName(){ return LoadVungDuLieu(106, 30); }
////////////////////////////////////////////////////////////////////////////////// AP Pass - 20
void AP_LuuPass(String _APPass){ LuuVungDuLieu(136, 20, _APPass); }
//////////////////////////////////////////////////////////////////////////////////
String AP_LoadPass(){ return LoadVungDuLieu(136, 20); }
////////////////////////////////////////////////////////////////////////////////// AP IP - 15
void AP_LuuIP(String _APIP){ LuuVungDuLieu(156, 15, _APIP); }
//////////////////////////////////////////////////////////////////////////////////
String AP_LoadIP(){ return LoadVungDuLieu(156, 15); }
////////////////////////////////////////////////////////////////////////////////// Admin Name - 30
void Admin_LuuName(String _AdmName){ LuuVungDuLieu(171, 30, _AdmName); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadName(){ return LoadVungDuLieu(171, 30); }
////////////////////////////////////////////////////////////////////////////////// Admin Pass - 20
void Admin_LuuPass(String _AdmPass){ LuuVungDuLieu(201, 20, _AdmPass); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadPass(){ return LoadVungDuLieu(201, 20); }
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void ResetDefault(){ LuuEEPROM(0,100); }
//////////////////////////////////////////////////////////////////////////////////
void LuuThongTinCaiDat()
{
  Wifi_LuuName( WifiName );   
  Wifi_LuuPass( WifiPass );   
  Wifi_LuuIP( WifiIP ); 
  Wifi_LuuPort( WifiPort );  
  Wifi_LuuGetway( WifiGetway );
  ///////////////////////////// 
  AP_LuuName( APName ); 
  AP_LuuPass( APPass ); 
  AP_LuuIP( APIP ); 
  /////////////////////////////
  Admin_LuuName( AdmName ); 
  Admin_LuuPass( AdmPass ); 
}
//////////////////////////////////////////////////////////////////////////////////
void LoadThongTinCaiDat()
{
    //Serial.print("EEPROM [0]"); Serial.println(LoadEEPROM(0));
  if(LoadEEPROM(0) > 0)
  {
      WifiName   = "admin"; 
      WifiPass   = "admin";
      WifiIP     = "";
      WifiPort   = "80"; 
      WifiGetway = "";
      /////////////////////////////
      APName = "UpToSmart.vn";
      APPass = "12345678";
      APIP   = "192.168.10.1";
      /////////////////////////////
      AdmName = "admin";
      AdmPass = "admin";
      /////////////////////////////
      LuuEEPROM(0,0);
      LuuThongTinCaiDat();
      delay(500);
  }
  else
  {
      WifiName   = Wifi_LoadName(); 
      WifiPass   = Wifi_LoadPass();
      WifiIP     = Wifi_LoadIP();
      WifiPort   = Wifi_LoadPort();
      WifiGetway = Wifi_LoadGetway();
      /////////////////////////////
      APName     = AP_LoadName();
      APPass     = AP_LoadPass();
      APIP       = AP_LoadIP();
      /////////////////////////////
      AdmName    = Admin_LoadName();
      AdmPass    = Admin_LoadPass();
  }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      Text function      ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String LayChuoiIP(IPAddress _IP)
{
    String _ChuoiIP = (String)_IP[0] + "." + (String)_IP[1] + "." + (String)_IP[2] + "." + (String)_IP[3] ;
    return _ChuoiIP;
}
//////////////////////////////////////////////////////////////////////////////////
IPAddress String2IP(String _ChuoiIP)
{
  _ChuoiIP+= ".";
  String _strChuoi = "";
  uint8_t nThuTu   = 0;
  uint8_t nIP[4]   = {0,0,0,0};
  for(int i=0; i< _ChuoiIP.length(); i++)
  {
    String sKyTu = _ChuoiIP.substring(i, i+1);
    if(sKyTu == ".")
    {
      String tmpSTR = _strChuoi;
      nIP[nThuTu] = tmpSTR.toInt();
      _strChuoi = "";
      nThuTu++;
    }
    else{ _strChuoi+= sKyTu; }
  }
  if(nThuTu != 4){ nIP[0]=0; nIP[1]=0; nIP[2]=0; nIP[3]=0; }
  IPAddress tmpIP(nIP[0], nIP[1], nIP[2], nIP[3]); 
  return tmpIP;
}
//////////////////////////////////////////////////////////////////////////////////
String LayTienToIP(String IP)
{
  uint8_t nKyTu  = IP.length();
  String tmpIP = "";
  for(uint8_t i = 0; i< nKyTu; i++)
  {
    uint8_t k = nKyTu - i;
    String sKyTu = IP.substring(k - 1, k);
    if(sKyTu == "."){ tmpIP = IP.substring(0, k) ;  break;  }
  }
  return tmpIP;
}
//////////////////////////////////////////////////////////////////////////////////
String LayGetWayIP(String IP){  return (LayTienToIP(IP)+ "1"); }
//////////////////////////////////////////////////////////////////////////////////
void CaiDatGetway(String IP)
{
  WifiGetway = LayGetWayIP(IP);
  Wifi_LuuGetway(WifiGetway);
}
//////////////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////
int String2Int(String sSo){  return sSo.toInt(); }
//////////////////////////////////////////////////////////////////////////////////
///////////    XDATA1_,DATA2_,DATA3_, ... = DATA1  DATA2  DATA3  ...//////////////
String ThuGonChuoiData(String _ChuoiData) 
{
  int i; 
  for( i=0; i < _ChuoiData.length(); i++)
  {
      String sKyTu = _ChuoiData.substring(i, i+1);
      if(sKyTu == "X"){ break; }
  } 
  String tmpData = _ChuoiData.substring(i+1, _ChuoiData.length());
  return tmpData;
}
////////////////////////////////////////////////////////////////////////////////////
void CaiDatWifiData(String _ChuoiData)
{
  String tmpSTR      = ThuGonChuoiData(_ChuoiData);
  String _strChuoi   = "";
  uint8_t nThuTu     = 0;
  String MangData[10] = {"","","","","","","","", "", ""};
  for(uint8_t i=0; i< tmpSTR.length(); i++)
  {
    String sKyTu = tmpSTR.substring(i, i+1);
    if( sKyTu == "," )
    {
      MangData[nThuTu] = _strChuoi;
      _strChuoi = "";
      nThuTu++;
    }
    else{ _strChuoi+= sKyTu; }
  }

  // WifiName, WifiPass, WifiIP, WifiPort, WifiGetway, APName, APPass, APIP, AdmName, AdmPass;
  APName   = MangData[0]; APPass   = MangData[1]; APIP   = MangData[2];
  AdmName  = MangData[3]; AdmPass  = MangData[4];
  WifiName = MangData[5]; WifiPass = MangData[6]; WifiIP = MangData[7]; WifiPort  = MangData[8]; WifiGetway  = MangData[9];
}
////////////////////////////////////////////////////////////////////////////////////
void LoadDuLieu(String ChuoiData)
{
    ChuoiDuLieuMCU = ChuoiData;
    /*
    //RL1, RL2, RL3, RL4, SS1, SS2, SS3, SS4, NhietDo, TroiMua ;
    RL1 = ChuoiData.substring(9,10).toInt();
    RL2 = ChuoiData.substring(11,12).toInt();
    RL3 = ChuoiData.substring(13,14).toInt();
    RL4 = ChuoiData.substring(15,16).toInt();

    NhietDo = ChuoiData.substring(17,19).toInt();
   
    SS1 = ChuoiData.substring(20,22).toInt();
    SS2 = ChuoiData.substring(23,25).toInt();
    SS3 = ChuoiData.substring(26,28).toInt();
    SS4 = ChuoiData.substring(29,31).toInt();
    if(SS4 > 50){ TroiMua = 1; }
    else{ TroiMua = 0; }
    */
}
//////////////////////////////////////////////////////////////////////////////////
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
char* JScriptSuLyChung()
{
    char* JVScript = "<script type='application/javascript'> var internetIP = \"\";\
                     function SendURL(URL){ var xhttp = new XMLHttpRequest(); xhttp.open('GET', URL, true); xhttp.send();}\
                     function OpenLink(URL){ window.location.href = URL ; }\
                     function ReloadWeb(){ window.location.reload(); }\
                     function ReloadWebTimer(_timer){ setTimeout('ReloadWeb()',_timer); }\
                     function getIP(json) { if(internetIP == \"\")  { internetIP = json.ip; }      }\
                     </script><script type='application/javascript' src='https://api.ipify.org?format=jsonp&callback=getIP'></script>";
  return JVScript;
}
////////////////////////////////////////////////////////////////////////////////// Script Access point
String JScriptAdmins()
{
    String JVScript = "<script>\
                      function WRestart(){ SendURL('/wifi?restart=1'); }\
                      function SRestart(){ SendURL('/setting?restart=1'); }\
                      function IPCheck(_IDName, MSG){ var _IP = document.getElementById(_IDName).value ;  if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(_IP)){ return true; } alert(MSG); return false; }\
                      function NameCheck(IDName, nNumber, MSG){var Text = document.getElementById(IDName).value ;  var nCount = Text.length; if(nNumber > nCount ){ alert(MSG); return false; } return true;}\
                      </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String JScriptTrangChu()
{
    String JVScript = "<script>\
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
String JScriptTrangWifiConnect()
{
    String JVScript = "<script>\
                        function GetWifiName(_Value){ document.getElementById('WifiName').value = _Value; }\      
                        function ShowPass(){ document.getElementById('password').type  = 'text'; }\
                        function HiddenPass(){ document.getElementById('password').type  = 'password'; }\ 
                        function WifiSetup(){ \
                          var _Name = document.getElementById('WifiName').value; \
                          var _Pass = document.getElementById('password').value; \
                          var _URL  = '/wifi?ssid=' + _Name + '&password=' + _Pass; \
                          if(_Name == \"\"){ elert('Select wifi !'); return false; }\
                          else{ SendURL(_URL); ReloadWebTimer(500);} \
                        }\
                       </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String JScriptTrangSetting()
{
  //  APName, APPass , APIP , DName , DPass , WIP, WPort, WGetway
  String JVScript = "<script>\
                     function AnNutSave()  { document.getElementById('btSave').style.visibility = 'hidden'; }\ 
                     function HienNutSave(){ document.getElementById('btSave').style.visibility = 'visible'; }\ 
                     function LuuSetting(){\
                        var aname   = document.getElementById('APName').value;\
                        var apass   = document.getElementById('APPass').value;\
                        var aip     = document.getElementById('APIP').value;\
                        var dname   = document.getElementById('DName').value;\
                        var dpass   = document.getElementById('DPass').value;\
                        var wip     = document.getElementById('WIP').value;\
                        var wport   = document.getElementById('WPort').value;\
                        if(aname == '') { alert('Please input access point name !'); return false; }\
                        if(NameCheck('APPass', 8, 'Access point password 8 character or more !') == false) { return false; }\
                        if(IPCheck('APIP', 'Access point IP error !') == false)  { return false; }\
                        if(NameCheck('DName', 5, 'Admin name 5 character or more !') == false) { return false; }\
                        if(NameCheck('DPass', 5, 'Admin password 5 character or more !') == false) { return false; }\
                        if(IPCheck('WIP', 'Wifi IP error !') == false)  { return false; }\
                        if(wport == '') { alert('Please input wifi port !'); return false; }\
                        var URL = '/setting?APName=' + aname + '&APPass=' + apass + '&APIP=' + aip + '&DName=' + dname + '&DPass=' + dpass + '&WIP=' + wip + '&WPort=' + wport + '&DeviceS=OK' ;\
                        SendURL(URL); AnNutSave(); setTimeout('HienNutSave()',2000);}\
                        </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String CStyleCSS()
{
   String Button = "<style type='text/css'>\
                    .shadow{ text-shadow: 2px 2px 5px #000000;}\
                     body{background: url( http://wallpoper.com/images/00/45/05/47/green-background-2_00450547.jpg ) fixed;}\
                     body{background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #0688fa), color-stop(1, #ffffff));background:-moz-linear-gradient(top, #0688fa 5%, #ffffff 100%);background:-webkit-linear-gradient(top, #0688fa 5%, #ffffff 100%);background:-o-linear-gradient(top, #0688fa 5%, #ffffff 100%);background:-ms-linear-gradient(top, #0688fa 5%, #ffffff 100%);background:linear-gradient(to bottom, #0688fa 5%, #ffffff 100%);filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#0688fa', endColorstr='#ffffff',GradientType=0);background-color:#0688fa;} \
                    .bogoc{ border-radius: 25px;}\
                    .textinput{width:100%; height:100px;font-size:50px}\
                    .button{background: #73AD21; font-size:60px; text-align: center; }\
                    .tieude{color:blue;font-size:80px}\
                    .radio{width:50px; height:50px;}\
                    .text{color:white;font-size:60px}\
                    .TimerDown{color:blue;font-size:80px}\
                    .ComBoBox{width:200px; font-size:60px; }\
                    .myButton {-moz-box-shadow:inset 0px 39px 0px -24px #e67a73;-webkit-box-shadow:inset 0px 39px 0px -24px #e67a73;box-shadow:inset 0px 39px 0px -24px #e67a73;background-color:#e4685d;-moz-border-radius:4px;-webkit-border-radius:4px;border-radius:4px;border:1px solid #ffffff;display:inline-block;cursor:pointer;color:#ffffff;font-family:Arial;font-size:60px;padding:30px 60px;text-decoration:none;text-shadow:0px 1px 0px #b23e35;}\
                    .myButton:hover {background-color:#eb675e;}\
                    .myButton:active {position:relative;top:1px;} \
                    </style>";
  return Button;
}
//////////////////////////////////////////////////////////////////////////////////
String TieuDe(String _TieuDe)
{
   return  "<h1 class='tieude' ><center>" + _TieuDe + "</center></h1>";
}

//////////////////////////////////////////////////////////////////////////////////
String TieuDeLink(String _TieuDe, String _Link)
{
   return  "<center> <a href='"+ _Link +"'> <h1 class='tieude' > " + _TieuDe + " </h1> </a> </center>";
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void BatTatDen(uint8_t _Pin, uint8_t nTrangThai)
{
  if (nTrangThai == 1){  digitalWrite(_Pin, HIGH); } else{ digitalWrite(_Pin, LOW);  }
}
//////////////////////////////////////////////////////////////////////////////////
void KiemTraTrangThaiDen(String _Name, uint8_t _Pin )
{
  if (server.hasArg(_Name))
  { 
    String stas = server.arg(_Name) ;
    if (stas == "1"){  digitalWrite(_Pin, HIGH); Serial.println("UTS_RELAY:1,1"); } else{ digitalWrite(_Pin, LOW);  Serial.println("UTS_RELAY:1,0");}
    Serial.println(_Name + stas);
  }
}
//////////////////////////////////////////////////////////////////////////////////
String GiaoDienNutBam()
{
  String HTML = "<div align=center>\
                          <table border='0' cellspacing='5'>\
                              <tr>\
                                  <td > <a class='text'> Tuoi cay </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut2'  onclick=\"GPIO_Control('Nut2','gpio0')\"> </td>\
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
   KiemTraTrangThaiDen("gpio2",  GPIO2  );
   KiemTraTrangThaiDen("gpio0",  GPIO0  );
   KiemTraTrangThaiDen("gpio5",  GPIO5  );
   KiemTraTrangThaiDen("gpio4",  GPIO4  );
   KiemTraTrangThaiDen("gpio15", GPIO15 );
   
  String HTML = Header + JScriptSuLyChung() + JScriptTrangChu() + CStyleCSS() + GiaoDienNutBam() + EndHeader ;
  server.send(200, "text/html", HTML);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void SearchWifi()
{
  if (server.hasArg("ssid"))
  {    
     WifiName = server.arg("ssid").c_str(); 
     WifiPass = server.arg("password").c_str();
     Wifi_LuuName(WifiName);
     Wifi_LuuPass(WifiPass);
    if (server.hasArg("password"))
    {
      WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
    }
    else
    {
      WiFi.begin(String2Char(WifiName));
    }
    while (WiFi.status() != WL_CONNECTED){ delay(500);  Serial.print("."); } Serial.println(""); delay(500);
    if(WiFi.status()== WL_CONNECTED){ CaiDatGetway(LayChuoiIP(WiFi.localIP())) ;}
  }

  String HTML  = Header + JScriptSuLyChung() + JScriptAdmins() + JScriptTrangWifiConnect() + CStyleCSS() + TieuDe("Wifi connect");
         HTML += "<div align=center><a align=center class='text'>Name: " + WiFi.SSID() + "</a><br>";
         HTML += "<a align=center class='text'>IP: " + LayChuoiIP(WiFi.localIP()) + "</a><br></div>";
  int ap_count = WiFi.scanNetworks();
  if (ap_count == 0)
  {
       HTML += "No access points found.<br>";
  }
  else
  {
    HTML += "<form method='get' name='WifiSetting'>";
    for (uint8_t ap_idx = 0; ap_idx < ap_count; ap_idx++)
    {
      HTML += "<input type='radio' name='ssid'  class='text radio' value=''  onclick=\"GetWifiName('" + String(WiFi.SSID(ap_idx)) + "')\"> <text class='text shadow' > ";
      HTML += String(WiFi.SSID(ap_idx)) + " (" + (100+ WiFi.RSSI(ap_idx)) +"%)";
      (WiFi.encryptionType(ap_idx) == ENC_TYPE_NONE) ? HTML += " " : HTML += "<font color='red'>*</font> </text>";
      HTML += "<p>";
    }
    
    HTML += "<input type='hidden' id='WifiName' value=''>";
    HTML += "<text class='text shadow' style='color:red'>(*) Password: </text><br>";
    HTML += "<input type='radio' name='PSelect' class='text radio' onclick='HiddenPass()' checked  ><a class='text'> Hidden </a> &nbsp; &nbsp; <input type='radio' name='PSelect' class='text radio' onclick='ShowPass()'> <a class='text'> Show </a>";
    HTML += "<input type='password' id='password' class='textinput' ><br><br>";
    HTML += "<input type='button' value='Connect' class='myButton' onclick='WifiSetup()'>";
    HTML += "</form>";
    HTML += TieuDeLink("Setting","/setting");
    HTML += TieuDeLink("Home","/");
  }
  HTML += EndHeader;
  server.send(200, "text/html", HTML);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////  
void Setting()
{
  // String WifiName, WifiPass, WifiIP, WifiPort, WifiGetway, APName, APPass, APIP, AdmName, AdmPass;
  // server.arg("ssid").c_str();    ----    APName, APPass , APIP , DName , DPass , WIP, WPort, WGetway
  String ssAPName  = server.arg("APName").c_str();  Serial.println(ssAPName);
  String ssAPPass  = server.arg("APPass").c_str();  Serial.println(ssAPPass);
  String ssAPIP    = server.arg("APIP").c_str();    Serial.println(ssAPIP);
  String ssDName   = server.arg("DName").c_str();   Serial.println(ssDName);
  String ssDPass   = server.arg("DPass").c_str();   Serial.println(ssDPass);
  String ssWIP     = server.arg("WIP").c_str();     Serial.println(ssWIP);
  String ssWPort   = server.arg("WPort").c_str();   Serial.println(ssWPort);
  String DeviceS   = server.arg("DeviceS").c_str(); Serial.println(DeviceS); Serial.println("-----------------------------------");

  if(DeviceS =="OK")
  {
      AP_LuuName( ssAPName ); APName = ssAPName;
      AP_LuuPass( ssAPPass ); APPass = ssAPPass;
      AP_LuuIP( ssAPIP );     APIP   = ssAPIP;
      /////////////////////////////
      Admin_LuuName( ssDName ); AdmName = ssDName;
      Admin_LuuPass( ssDPass ); AdmPass = ssDPass;
      /////////////////////////////
      //Wifi_LuuName( WifiName );   
      //Wifi_LuuPass( WifiPass );   
      Wifi_LuuIP( ssWIP );     WifiIP   = ssWIP;
      Wifi_LuuPort( ssWPort ); WifiPort = ssWPort;
      ///////////////////////////// 
  }
  
  CallRestart();
  String HTML  = Header + JScriptSuLyChung() + JScriptAdmins() + JScriptTrangSetting()+ CStyleCSS();

         HTML += TieuDe("Access point");
         HTML += "<text class='text shadow' style='color:red' > Name </text><br>";
         HTML += "<p><input type='text' class='textinput' id='APName' value='"+ APName +"'></p>";
         HTML += "<text class='text shadow' style='color:red'> Password </text><br>";
         HTML += "<p><input type='text' class='textinput' id='APPass' value='"+ APPass +"'></p>";
         HTML += "<text class='text shadow' style='color:red'> IP </text><br>";
         HTML += "<p><input type='text' class='textinput' id='APIP' value='"+ APIP +"'></p>";       
         HTML += "<text class='text shadow' style='color:red'> MAC: </text> <text class='text shadow'> " +  WiFi.softAPmacAddress() + " </text><br>";  
         
         HTML += TieuDe("Admin");
         HTML += "<text class='text shadow' style='color:red'> Name </text><br>";
         HTML += "<p><input type='text' class='textinput' id='DName'  value='"+ AdmName +"'></p>";
         HTML += "<text class='text shadow' style='color:red'> Password </text><br>";
         HTML += "<p><input type='text' class='textinput' id='DPass'  value='"+ AdmPass +"'></p>";
         HTML += TieuDe("Wifi");
         HTML += "<text class='text shadow' style='color:red'> Name: </text> <text class='text shadow'> " + WiFi.SSID() + "</text><br>";
         HTML += "<text class='text shadow' style='color:red'> Password: </text> <text class='text shadow'> " + WifiPass  + " </text><br>";
         HTML += "<text class='text shadow' style='color:red'> IP </text><br>";
         HTML += "<p><input type='text' class='textinput' id='WIP' value='"+ LayChuoiIP(WiFi.localIP()) +"'></p>";
         HTML += "<text class='text shadow' style='color:red'> Port </text><br>";
         HTML += "<p><input type='text' class='textinput' id='WPort'  value='"+ WifiPort +"'></p>";
         HTML += "<text class='text shadow' style='color:red'> Getway: </text> <text class='text shadow'> " +  LayChuoiIP(WiFi.gatewayIP())  + " </text><br>";
         HTML += "<text class='text shadow' style='color:red'> MAC: </text> <text class='text shadow'> "    +  WiFi.macAddress() + " </text><br>";         
         HTML += "<input type='button' value=' Save ' class='myButton' id='btSave' onclick='LuuSetting()'> &nbsp; &nbsp; &nbsp;";
         HTML += "<input type='button' value=' Restart ' class='myButton'  onclick='SRestart()'>";
         HTML += TieuDeLink("Wifi connect","/wifi");
         HTML += TieuDeLink("Home","/");
         HTML += EndHeader;
  server.send(200, "text/html", HTML);
}
//////////////////////////////////////////////////////////////////////////////////
void Login()
{
  String HTML  = Header + JScriptSuLyChung() + CStyleCSS();
         HTML += TieuDe("Login");
         HTML += "<text class='text shadow' style='color:red'> Name </text><br>";
         HTML += "<p><input type='text' class='textinput' id='AName'></p>";
         HTML += "<text class='text shadow' style='color:red'> Password </text><br>";
         HTML += "<p><input type='text' class='textinput' id='APass'></p>";
         HTML += "<input type='button' value=' Save ' class='myButton' id='btSave' onclick='LuuSetting()'>";
         HTML += "</body></html>";//EndHeader;
  server.send(200, "text/html", HTML);
}
//////////////////////////////////////////////////////////////////////////////////
void UpToSmartVN() {
  String DuLieu = server.arg("UTS_DATA").c_str();  
  if(DuLieu   !=""){ Serial.println(DuLieu);   } // Gửi thông tin cài đặt [Timer] qua MCU
  server.send ( 200, "text/html", ChuoiDuLieuMCU );
}
//////////////////////////////////////////////////////////////////////////////////
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

const char *wi_ssid  = ""; // Wifi 
const char *wi_pass  = "";
const char *ap_ssid  = ""; // Access point 
const char *ap_pass  = "";
// WifiName, WifiPass, WifiIP, WifiPort, WifiGetway, APName, APPass, APIP, AdmName, AdmPass;


void setup() 
{
  pinMode(GPIO2,  OUTPUT);
  pinMode(GPIO0,  OUTPUT);
  pinMode(GPIO5,  OUTPUT);
  pinMode(GPIO4,  OUTPUT);
  pinMode(GPIO15, OUTPUT);
  /////////////////////////////
  EEPROM.begin(512);
  delay(100);
  Serial.begin(115200);
  Serial.println(" ");
  LoadThongTinCaiDat();
  
  WiFi.mode(WIFI_AP_STA);
  IPAddress Subnet = IPAddress(255,255,255,0);
  /////////////////////////// Wifi connect
  Serial.println("Configuring wifi ...");
  wi_ssid = String2Char(WifiName);  
  wi_pass = String2Char(WifiPass); 
  WiFi.begin(wi_ssid, wi_pass);
  delay(3000);
  if(WiFi.status() == WL_CONNECTED) 
  {
        String OnlineIP = LayChuoiIP(WiFi.localIP());
        if(LayTienToIP(OnlineIP) != LayTienToIP(WifiIP)){ WifiIP = OnlineIP; Wifi_LuuIP( WifiIP ); }

        WifiGetway        = LayGetWayIP(WifiIP);
        IPAddress wip     = String2IP(WifiIP);
        IPAddress wgetway = String2IP(WifiGetway);
        WiFi.config( wip, wgetway, Subnet);
        Serial.println("WIFI_IP: " + WifiIP);
  }
  else{ Serial.println("Not connect wifi"); }
  ////////////////////////// Access point setup
  Serial.println("Configuring access point...");
  Serial.println("Name: "+ APName);
  ap_ssid = String2Char(APName);  
  ap_pass = String2Char(APPass);
  IPAddress APlocalIP = String2IP(APIP);
  WiFi.softAPConfig( APlocalIP, APlocalIP, Subnet);
  WiFi.softAP(ap_ssid, ap_pass);  //   WiFi.softAP(ap_ssid);  // Cài đặt chế độ password hoặc không password 
  Serial.println("AP_IP: " + APIP);
  // Các trang mặc định 
  server.on("/",          Desktop);
  server.on("/wifi",      SearchWifi);
  server.on("/login",     Login);
  server.on("/setting",   Setting);
  server.on("/uptosmart", UpToSmartVN);
  server.onNotFound(LoiKoThayTrang);
  server.begin();
  Serial.println("HTTP server started");
}
//////////////////////////////////////////////////////////////////////////////////
String ChuoiDuLieu    = "";
void loop() 
{
   server.handleClient();
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
   if(ChuoiDuLieu != "")
   {
      SuLyGiaTriChuoiNhan(ChuoiDuLieu);
      ChuoiDuLieu = "";
   }
   //if(WiFi.status() != WL_CONNECTED) { Serial.println("Restart"); Restart(); }
   ////////////////////////////
   //if(WiFi.status() == WL_CONNECTED) 
   //{
      //Serial.println(getTime());
      //delay(1000);
   //}
   ////////////////////////////
}

//////////////////////////////////////////////////////////////////////////////////
void SuLyGiaTriChuoiNhan(String ChuoiNhan)
{
  bool timthay         = false;
  String ChuoiGiaTri   = "";
  String ChuoiNhanDang = "";
  uint8_t nKyTu = ChuoiNhan.length(), i;
  for( i = 0; i< nKyTu; i++)
  {
    if(ChuoiNhan.substring(i, i+1) == ":"){ timthay = true; break; }
  }
  if(timthay)
  {
      ChuoiNhanDang = ChuoiNhan.substring(0, i);
      ChuoiGiaTri   = ChuoiNhan.substring(i+1, nKyTu);
  } else { ChuoiNhanDang = ChuoiNhan; }
  
  if(ChuoiNhanDang == "UTS_WPASS")   { Wifi_LuuPass(ChuoiGiaTri); }
  if(ChuoiNhanDang == "UTS_WNAME")   { Wifi_LuuName(ChuoiGiaTri); }
  if(ChuoiNhanDang == "UTS_RESTART") { Serial.println("RESTART");  Restart(); }
  if(ChuoiNhanDang == "UTS_RESET")   { Serial.println("RESET DEFAULT"); ResetDefault();  Restart(); }
  if(ChuoiNhanDang == "UTS_MCU")     { LoadDuLieu(ChuoiGiaTri);  }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String getTime() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {  Serial.println("connection failed, retrying..."); }
  client.print("HEAD / HTTP/1.1\r\n\r\n");
   while(!!!client.available()) { yield(); }

  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
} 
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
