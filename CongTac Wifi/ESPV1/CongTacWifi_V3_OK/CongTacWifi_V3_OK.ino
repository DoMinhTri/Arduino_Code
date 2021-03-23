#include <IPAddress.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);
/////////////////////  
// Wifi

String WifiName, WifiPass, WifiIP, WifiPort, APName, SVID, DVNUM, APPass, APIP, AdmName, AdmPass;
String Header    = "<html><head><title>UpToSmart.VN</title></head> <body style='background-color:#c0c0c0' onload=''> ";

void Restart(){  Serial.println("NoWifi"); delay(500); Serial.println("MCU_RESTART"); delay(1000); ESP.restart(); }
void ResetDefault(){ LuuEEPROM(0,100); }
void CallRestart(){ if (server.hasArg("restart")){ String  sStas = server.arg("restart").c_str();  if( sStas == "ok"){   Restart(); }}}

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
////////////////////////////////////////////////////////////////////////////////// Admin Pass - 20
void SV_LuuSVID(String _AdmSVID){ LuuVungDuLieu(221, 16, _AdmSVID); }
//////////////////////////////////////////////////////////////////////////////////
String SV_LoadSVID(){ return LoadVungDuLieu(221, 16); }
//////////////////////////////////////////////////////////////////////////////////
void SV_LuuDVNUM(String _AdmDVNUM){ LuuVungDuLieu(240, 3, _AdmDVNUM); }
//////////////////////////////////////////////////////////////////////////////////
String SV_LoadDVNUM(){ return LoadVungDuLieu(240, 3); }
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      Text function      ////////////////////////////////////
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
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////
int String2Int(String sSo){  return sSo.toInt(); }

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////   Web function   ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String JScriptTrangHome()
{
    String JVScript = "<script>\
                          function TTNutBam(TenNut){ var TT = document.getElementById(TenNut).value ;\
                            if(TT == 'On') { document.getElementById(TenNut).style.background='#ff0000'; return 1;}  \
                            if(TT == 'Off'){ document.getElementById(TenNut).style.background='#c0c0c0'; return 0;} }\
                          function NutBam( _Name){\
                          var Value_Name = document.getElementById(_Name).value;\
                          if(Value_Name == 'On')  { document.getElementById(_Name).value = 'Off'; } \
                          if(Value_Name == 'Off') { document.getElementById(_Name).value = 'On';  } \
                           var TT1 = TTNutBam('Nut1');  \
                           var TT2 = TTNutBam('Nut2');  \
                           var TT3 = TTNutBam('Nut3');  \
                           var TT4 = TTNutBam('Nut4');  \
                           var url = '/home?TTCT=' + TT1 + ',' + TT2 + ',' + TT3 + ',' + TT4 ; \
                           SendURL(url); \
                      }\
                     </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
// document.getElementById('CbbTuoiNhanh').value = 1; 
// document.getElementById('NutTuoi').innerHTML
String JScriptAdmins()
{
    String JVScript = "<script type='application/javascript'> function SendURL(URL){ var xhttp = new XMLHttpRequest(); xhttp.open('GET', URL, true); xhttp.send();}\
                     function OpenLink(URL){ window.location.href = URL ; } \
                     function ReloadWeb(){ window.location.reload(); } \
                     function ReloadWebTimer(_timer){ setTimeout('ReloadWeb()',_timer); }\
                     function WRestart(){ SendURL('/wifi?restart=ok'); }\
                     function SRestart(){ SendURL('/setting?restart=ok'); }\
                     function IPCheck(_IDName, MSG){ var _IP = document.getElementById(_IDName).value ;  if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(_IP)){ return true; } alert(MSG); return false; }  \
                     function NameCheck(IDName, nNumber, MSG){ var Text = document.getElementById(IDName).value ;  var nCount = Text.length; if(nNumber > nCount ){ alert(MSG); return false; } return true;} </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String JScriptTrangWifiConnect()
{
    String JVScript = "<script>  function GetWifiName(_Value){ document.getElementById('WifiName').value = _Value; } function ShowPass(){ document.getElementById('password').type   = 'text'; }  function HiddenPass(){ document.getElementById('password').type = 'password'; }\ 
                        function WifiSetup(){ var _Name = document.getElementById('WifiName').value; var _Pass = document.getElementById('password').value; var _URL  = '/wifi?ssid=' + _Name + '&password=' + _Pass; if(_Name == \"\"){ elert('Select wifi !'); return false; } else { SendURL(_URL); ReloadWebTimer(500); }} </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String JScriptTrangSetting()
{
  // APName - APPass - APIP *** AdmName - AdmPass *** SVID *** WIP - WPort *** DeviceS
  String JVScript = "<script> function AnNutSave()  { document.getElementById('btSave').style.visibility = 'hidden'; }   function HienNutSave(){ document.getElementById('btSave').style.visibility = 'visible'; }\
                        function LuuSetting(){\
                        var aname = document.getElementById('APName').value;\
                        var apass = document.getElementById('APPass').value; \
                        var aip   = document.getElementById('APIP').value; \
                        var dname = document.getElementById('AdmName').value; \
                        var dpass = document.getElementById('AdmPass').value; \
                        var svid  = document.getElementById('SVID').value; \
                        var dvnum = document.getElementById('DVNUM').value; \
                        var wip   = document.getElementById('WIP').value; \
                        var wport = document.getElementById('WPort').value;\
                        if(aname == '') { alert('Please input access point name !'); return false; } \
                        if(NameCheck('APPass', 8, 'Access point password 8 character or more !') == false) { return false; }\
                        if(IPCheck('APIP', 'Access point IP error !') == false)  { return false; }\   
                        if(NameCheck('AdmName', 5, 'Admin name 5 character or more !') == false) { return false; }\ 
                        if(NameCheck('AdmPass', 5, 'Admin password 5 character or more !') == false) { return false; }\
                        if(NameCheck('SVID', 16, 'ID need 16 character !') == false) { return false; }\  
                        if(IPCheck('WIP', 'Wifi IP error !') == false)  { return false; }\
                        if(wport == '') { alert('Please input wifi port !'); return false; }\
                        var URL = '/setting?APName=' + aname + '&APPass=' + apass + '&APIP=' + aip + '&AdmName=' + dname + '&AdmPass=' + dpass + '&SVID=' + svid + '&DVNUM=' +  dvnum + '&WIP=' + wip + '&WPort=' + wport + '&DeviceS=OK' ;\
                        SendURL(URL);  AnNutSave(); setTimeout('HienNutSave()',1000) ;} </script>";
  return JVScript;
}
//////////////////////////////////////////////////////////////////////////////////
String CStyleCSS()
{
   String Button = "<style type='text/css'> .shadow{ text-shadow: 2px 2px 5px #000000;}  body{background: url(  ) fixed;}\
                    .bogoc{ border-radius: 25px;} .textinput{width:100%; height:100px;font-size:50px} .button{background: #73AD21; font-size:60px; text-align: center; }\
                    .tieude{color:blue;font-size:80px} .radio{width:50px; height:50px;} .text{color:white;font-size:60px} .TimerDown{color:blue;font-size:80px} .ComBoBox{width:200px; font-size:60px; }\
                    .myButton {-moz-box-shadow:inset 0px 39px 0px -24px #e67a73;-webkit-box-shadow:inset 0px 39px 0px -24px #e67a73;box-shadow:inset 0px 39px 0px -24px #e67a73;background-color:#e4685d;-moz-border-radius:4px;-webkit-border-radius:4px;border-radius:4px;border:1px solid #ffffff;display:inline-block;cursor:pointer;color:#ffffff;font-family:Arial;font-size:30px;padding:15px 30px;text-decoration:none;text-shadow:0px 1px 0px #b23e35;}\
                    .myButton:hover {background-color:#eb675e;} .myButton:active {position:relative;top:1px; \
                    .myButton:active {position:relative;top:1px;} </style>";
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
/////////////////////   Web Interface   //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Desktop() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}
//////////////////////////////////////////////////////////////////////////////////
String GiaoDienNutBam()
{
  String HTML =  "<div align=center>\
                          <table border='0' cellspacing='5'>\
                              <tr>\
                                  <td > <a class='text' style='font-size:20px'> Thiet bi 1 </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut1'  onclick=\"NutBam('Nut1')\"> </td>\
                              </tr>\
                              <tr>\
                                  <td > <a class='text' style='font-size:20px'> Thiet bi 2 </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut2'  onclick=\"NutBam('Nut2')\"> </td>\
                              </tr>\
                              <tr>\
                                  <td > <a class='text' style='font-size:20px'> Thiet bi 3 </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut3'  onclick=\"NutBam('Nut3')\"> </td>\
                              </tr>\
                              <tr>\
                                  <td > <a class='text' style='font-size:20px'> Thiet bi  4 </a> </td>\
                                  <td > <input type='button' class='myButton' value='Off'  ID='Nut4'  onclick=\"NutBam('Nut4')\"> </td>\
                              </tr>\ 
                          </table>\
                      </div>";
  return HTML;
}
//////////////////////////////////////////////////////////////////////////////////
void TrangHome()
{
    
    if (server.hasArg("TTCT"))
    {
       String  ChuoiTT = server.arg("TTCT").c_str(); 
       if (ChuoiTT != "")
       { 
           Serial.println("{CT:\"" + ChuoiTT + "\"}");
       }
    }
    //////////////////
    String HTML  = Header + JScriptAdmins() + JScriptTrangHome() + CStyleCSS() ;
           HTML += GiaoDienNutBam();
           HTML += "</body></html>";
           //HTML += TieuDeLink("Wifi connect","/wifi");
           //HTML += TieuDeLink("Setting","/setting");
    server.send(200, "text/html", HTML);
}

//////////////////////////////////////////////////////////////////////////////////
void SearchWifi()
{
  if (server.hasArg("ssid"))
  {    
      WifiName = server.arg("ssid").c_str(); 
      if(WifiName != "")
      {
          Wifi_LuuName(WifiName);
          WiFi.disconnect();
          if (server.hasArg("password"))
          {
              WifiPass = server.arg("password").c_str();
              Wifi_LuuPass(WifiPass);
              WiFi.begin(String2Char(WifiName), String2Char(WifiPass));
          }else{ WiFi.begin(String2Char(WifiName)); }
          ///////////////
          delay(3000);
          Restart();
      }
  }
  ///////////////
  String HTML  = Header + JScriptAdmins() + JScriptTrangWifiConnect() + CStyleCSS() + TieuDe("Wifi connect");
         HTML += "<div align=center><a align=center class='text'>Name: " + WiFi.SSID() + "</a><br>";
         HTML += "<a align=center class='text'>IP: " + LayChuoiIP(WiFi.localIP()) + "</a><br></div>";
  uint8_t ap_count = WiFi.scanNetworks();
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
    //////////////////
    HTML += "<input type='hidden' id='WifiName' value=''>";
    HTML += "<text class='text shadow' style='color:red'>(*) Password: </text><br>";
    HTML += "<input type='radio' name='PSelect' class='text radio' onclick='HiddenPass()' checked  ><a class='text'> Hidden </a> &nbsp; &nbsp; <input type='radio' name='PSelect' class='text radio' onclick='ShowPass()'> <a class='text'> Show </a>";
    HTML += "<input type='password' id='password' class='textinput' ><br><br>";
    HTML += "<input type='button' value='Connect' class='myButton' onclick='WifiSetup()'> &nbsp; &nbsp; <input type='button' value='Restart' class='myButton' onclick='WRestart()'> </form>";
    HTML += TieuDeLink("Setting","/setting");
    HTML += TieuDeLink("Home","/home");
  }
  HTML += "</body></html>";
  server.send(200, "text/html", HTML);
}
//////////////////////////////////////////////////////////////////////////////////  
void Setting()
{
  //String WifiName, WifiPass, WifiIP, WifiPort, APName, SVID, APPass, APIP, AdmName, AdmPass;
  // APName - APPass - APIP *** AdmName - AdmPass *** SVID *** WIP - WPort *** DeviceS
  CallRestart();
  //////////////
  String DeviceS  = server.arg("DeviceS").c_str();

  if(DeviceS == "OK")
  {
      APName   = server.arg("APName").c_str();
      APPass   = server.arg("APPass").c_str();
      APIP     = server.arg("APIP").c_str(); 
      AdmName  = server.arg("AdmName").c_str(); 
      AdmPass  = server.arg("AdmPass").c_str();
      SVID     = server.arg("SVID").c_str();
      DVNUM    = server.arg("DVNUM").c_str();
      WifiIP   = server.arg("WIP").c_str();
      WifiPort = server.arg("WPort").c_str();
      //////////////
      AP_LuuName( APName ); 
      AP_LuuPass( APPass ); 
      AP_LuuIP( APIP );    
      //////////////
      Admin_LuuName( AdmName ); 
      Admin_LuuPass( AdmPass ); 
      //////////////
      SV_LuuSVID( SVID ); 
      SV_LuuDVNUM( DVNUM );
      //////////////
      Wifi_LuuIP( WifiIP );    
      Wifi_LuuPort( WifiPort );
      //////////////
  }
  //////////////
  
  String HTML  = Header +  JScriptAdmins() + JScriptTrangSetting()+ CStyleCSS();

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
         HTML += "<p><input type='text' class='textinput' id='AdmName'  value='"+ AdmName +"'></p>";
         HTML += "<text class='text shadow' style='color:red'> Password </text><br>";
         HTML += "<p><input type='text' class='textinput' id='AdmPass'  value='"+ AdmPass +"'></p>";
         
         HTML += TieuDe("Server");
         HTML += "<text class='text shadow' style='color:red'> ID </text><br>";
         HTML += "<p><input type='text' class='textinput' id='SVID'  value='"+ SVID +"'></p>";  
         HTML += "<text class='text shadow' style='color:red'> Device number </text><br>";
         HTML += "<p><input type='text' class='textinput' id='DVNUM' value='"+ DVNUM +"'></p>";  
                
         HTML += TieuDe("Wifi");
         HTML += "<text class='text shadow' style='color:red'> Name: </text> <text class='text shadow'> " + WiFi.SSID() + "</text><br>";
         HTML += "<text class='text shadow' style='color:red'> Password: </text> <text class='text shadow'> *** </text><br>";
         HTML += "<text class='text shadow' style='color:red'> IP </text><br>";
         HTML += "<p><input type='text' class='textinput' id='WIP' value='"+ LayChuoiIP(WiFi.localIP()) +"'></p>";
         HTML += "<text class='text shadow' style='color:red'> Port </text><br>";
         HTML += "<p><input type='text' class='textinput' id='WPort'  value='"+ WifiPort +"'></p>";
         HTML += "<text class='text shadow' style='color:red'> Getway: </text> <text class='text shadow'> " +  LayChuoiIP(WiFi.gatewayIP())  + " </text><br>";
         HTML += "<text class='text shadow' style='color:red'> MAC: </text> <text class='text shadow'> "    +  WiFi.macAddress() + " </text><br>";         
         HTML += "<input type='button' value=' Save ' class='myButton' id='btSave' onclick='LuuSetting()'> &nbsp; &nbsp; &nbsp;";
         HTML += "<input type='button' value=' Restart ' class='myButton'  onclick='SRestart()'>";
         HTML += TieuDeLink("Wifi connect","/wifi");
         HTML += TieuDeLink("Home","/home");
         HTML += "</body></html>";
  server.send(200, "text/html", HTML);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////   Load infor      //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void LuuThongTinCaiDat()
{
  Wifi_LuuName( WifiName );   
  Wifi_LuuPass( WifiPass );   
  Wifi_LuuIP( WifiIP ); 
  Wifi_LuuPort( WifiPort );  
  ///////////////////////////// 
  SV_LuuDVNUM( DVNUM );
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
      WifiName   = ""; 
      WifiPass   = "";
      WifiIP     = "";
      WifiPort   = "80"; 
      /////////////////////////////
      APName  = "UpToSmart.VN-UFHOME";
      APPass  = "12345678";
      APIP    = "192.168.10.1";
      /////////////////////////////
      DVNUM   = "4";
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
      /////////////////////////////
      APName     = AP_LoadName();
      APPass     = AP_LoadPass();
      APIP       = AP_LoadIP();
      /////////////////////////////
      SVID       = SV_LoadSVID();
      DVNUM      = SV_LoadDVNUM();
      /////////////////////////////
      AdmName    = Admin_LoadName();
      AdmPass    = Admin_LoadPass();
  }
}
////////////////////////////////////////////////////////////////////////////////////
String LayChuoiJson(String TTCongTac)
{
        HTTPClient http;
        String payload = "";
        String SVLink  = "http://fablabs.store/cttmm/inc/onoff/?UTSID=" + SVID + "&utsvalue=" + TTCongTac + "&utstype=utsdevice&utsdvnum=" + DVNUM;
        http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK)
            {  
                payload = http.getString(); 
            }
        } 
        http.end();
        //Serial.println(SVLink); 
        //////////
        return payload;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////       Main        //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
  
boolean KetNoiLanDau = false;
IPAddress Subnet = IPAddress(255,255,255,0);
// WifiName, WifiPass, WifiIP, WifiPort, APName, APPass, APIP, AdmName, AdmPass;
////////////////////////////////////////////////////////////////////////////////////
void setup() {
  EEPROM.begin(512);
  delay(100);
  Serial.begin(9600);
  delay(100);
  LoadThongTinCaiDat();
  ////////////////////////////////////////
  char *wi_ssid  = String2Char(WifiName); // Wifi 
  char *wi_pass  = String2Char(WifiPass);
  char *ap_ssid  = String2Char(APName); // Access point 
  char *ap_pass  = String2Char(APPass);
  
//  Serial.print("EEPROM ID:  "); Serial.println(LoadEEPROM(0));
//  Serial.print("WIFI SSID:  "); Serial.println(WifiName);
//  Serial.print("WIFI PASS:  "); Serial.println(WifiPass);
//  Serial.print("Local SSID: "); Serial.println(APName);
//  Serial.print("Local PASS: "); Serial.println(APPass);
//  Serial.println("Start"); 
  ////////////////////////////////////////
  WiFi.mode(WIFI_AP_STA);  // WIFI_AP
  ////////////////////////////////////////
  WiFi.begin(wi_ssid, wi_pass);
  delay(3000);
  ////////////////////////////////////////
  IPAddress APlocalIP = String2IP(APIP);
  WiFi.softAPConfig( APlocalIP, APlocalIP, Subnet);
  WiFi.softAP(ap_ssid, ap_pass);
  delay(1000);
  //////////////////////////////
  server.on("/",        Desktop);
  server.on("/wifi",    SearchWifi);
  server.on("/home",    TrangHome);
  server.on("/setting", Setting);
  server.begin();
  delay(2000);
  //////////////////////////////
  //ResetDefault();
  //////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////////
String TTCongTac = "";

void loop() 
{
  server.handleClient();
  ////////////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ TTCongTac+= KyTu; } delay(5);
  }
  ////////////////////////////
  if( WiFi.status() == WL_CONNECTED )
  {
         Serial.println("WifiOk"); 
        
        if(KetNoiLanDau == false)
        {
            KetNoiLanDau = true;
            IPAddress wip = String2IP(WifiIP);
            if(wip != WiFi.localIP())
            { 
                wip = WiFi.localIP();
                Wifi_LuuIP(LayChuoiIP(WiFi.localIP()));
            }
            WiFi.config( wip, WiFi.gatewayIP(), Subnet);
        }
        ////////////
        if(TTCongTac != "" )
        {
           String  OnlineData = LayChuoiJson(TTCongTac);
           if(OnlineData != "")  Serial.println(OnlineData);
           TTCongTac = "";
        }
        ////////////
    }
    else
    {
        Serial.println("NoWifi");
    }
    //////////////////////////////
    delay(1000);
   //////////////////////////////
}

