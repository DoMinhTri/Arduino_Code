
boolean KetNoiWifi = false;
boolean CaiDatSensor = false;
uint8_t TrangThaiHoatDong = 0;
uint8_t NhietDo, DoAmDat, TroiMua, TuoiCay;
String WifiName, WifiPass, WifiIP, WifiGetway, AdminName, AdminPass, AdminIP, AdminPort;
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
String ComBoxTuoiNhanh()
{
  String cbb = "<select size='1' class='ComBoBox' id='CbbTuoiNhanh' onchange='TuoiNhanh(this.value)'>";
                for(uint8_t i=0;i<=45;i++){ cbb += "<option value='"+ (String)i+ "'>"+ (String)i+ "</option>"; }
                cbb += "</select>";
  return cbb;
}
//////////////////////////////////////////////////////////////////////////////////
String ComBoxCheDoHoatDong(uint8_t nKieuHoatDong)
{
  String sKieu1 = "", sKieu2 = "", sKieu3 = "";
  if(nKieuHoatDong == 2) 
        sKieu2 = "selected";
  else if(nKieuHoatDong == 3) 
        sKieu3 = "selected";
  else 
        sKieu1 = "selected";
        
  String cbb =  (String)nKieuHoatDong+ "<select size='1' class='ComBoBox' id='CbbKieuHoatDong' onchange='HoatDong(this.value)'><option value='1' "+ sKieu1 +" >Auto</option><option value='2' "+ sKieu2 +" >On</option><option value='3' "+ sKieu3 +">Off</option></select>";
  return cbb;
}
//body{background: #c0c0c0; background: -webkit-linear-gradient(-90deg, #c0c0c0, white); background: -o-linear-gradient(-90deg, #c0c0c0, white); background: -moz-linear-gradient(-90deg, #c0c0c0, white);  background: linear-gradient(-90deg, #c0c0c0, white);} \ 
//////////////////////////////////////////////////////////////////////////////////
String CStyleCSS()
{
   String Button = "<style type='text/css'>\
                    .shadow{ text-shadow: 2px 2px 5px #000000;}\
                    body{background: url( http://wallpoper.com/images/00/45/05/47/green-background-2_00450547.jpg ) fixed;}\
                    .bogoc{ border-radius: 25px;}\
                    .textinput{width:90%; height:60px;font-size:50px}\
                    .button{background: #73AD21; font-size:60px; text-align: center; }\
                    .tieude{color:blue;font-size:80px}\
                    .text{color:white;font-size:80px}\
                    .TimerDown{color:blue;font-size:80px}\
                    .ComBoBox{width:200px; font-size:60px; }\
                    </style>";
  return Button;
}
//////////////////////////////////////////////////////////////////////////////////
String Online_Header()
{
  String strHeader ="<!DOCTYPE HTML><html><head><title>Smart Garden</title>\
                     <link type='text/css' rel='stylesheet' href='http://khicongnghiepvenmer.com/css/style.css' />\
                     </head><body><script> window.onload = function () {  }</script>\
                     <div align='center'>\
                     <style>body{background: url( http://wallpoper.com/images/00/45/05/47/green-background-2_00450547.jpg ) fixed;}</style>";
  return strHeader;
}
//////////////////////////////////////////////////////////////////////////////////
String Header()
{
  String strHeader ="<!DOCTYPE HTML><html><head><title>Green Leap</title></head>";
         strHeader+= Javascript();
         strHeader+= CStyleCSS();
         strHeader+="<body ><div align=center>";
  return strHeader;
}
//////////////////////////////////////////////////////////////////////////////////
String Font(String sChuoi)
{
  String tpString = "<font class='text shadow' > "+ sChuoi +" </font>";
  return tpString;
}
//////////////////////////////////////////////////////////////////////////////////
String TaoNut(String sTen)
{
    return "<input type='button' value='"+ sTen +"' class='button'>";  
}
//////////////////////////////////////////////////////////////////////////////////
String TableCamBien(String sTieuDe, String nChiSo)
{
  String sTieude = "&nbsp;" + Font(sTieuDe);
  String sChiSo  = "&nbsp;" + Font(nChiSo);
  return Table(sTieude,sChiSo);
}
//////////////////////////////////////////////////////////////////////////////////
String TableButton(String sNut1, String sNut2)
{
  String sNut_1 = TaoNut(sNut1);
  String sNut_2 = TaoNut(sNut2);
  return Table(sNut_1,sNut_2);
}
//////////////////////////////////////////////////////////////////////////////////
String Table(String _Chuoi1, String _Chuoi2)
{
  return "<table border='0' width='100%' cellspacing='15' cellpadding='15' ><tr> <td height='100'  valign='bottom'>"+ _Chuoi1 +" </td><td height='100' align='right' valign='bottom'>"+ _Chuoi2 +"</td> </tr></table>";
}
//////////////////////////////////////////////////////////////////////////////////
String TableCombo(String _Chuoi1, String _Chuoi2)
{
  _Chuoi1 = Font("&nbsp;" + _Chuoi1);
  return Table(_Chuoi1, _Chuoi2);
}
//////////////////////////////////////////////////////////////////////////////////
String TableInput(String _Ten, String _ID, String _GiaTri)
{
  String _TieuDe = "<font class='text shadow'> "+ _Ten +" </font>";
  String _Input  = "<input type='text'  id='"+ _ID +"' value='"+ _GiaTri +"' class='textinput bogoc shadow'>";
  return Table(_TieuDe, _Input);
}
//////////////////////////////////////////////////////////////////////////////////
String TablePass(String _Ten, String _ID, String _GT)
{
  String _TieuDe = "<font class='text shadow'> "+ _Ten +" </font>";
  String _Input  = "<input type='password'  id='"+ _ID +"' value='"+ _GT +"' class='textinput bogoc shadow'>";
  return Table(_TieuDe, _Input);
}
//////////////////////////////////////////////////////////////////////////////////
String TableText(String _Ten1, String _Ten2)
{
  String _TieuDe1 = "<font class='text shadow'> "+ _Ten1 +" </font>";
  String _TieuDe2  = "<font class='text shadow' style='color:blue'> "+ _Ten2 +" </font>";
  return Table(_TieuDe1, _TieuDe2);
}
//////////////////////////////////////////////////////////////////////////////////
String TablePutPass(String _TieuDe, String _Ten1, String _ID1, String _GT1, String _Ten2, String _ID2, String _GT2)
{
  String tblData = "<font size='18' class='tieude shadow'>" + _TieuDe + "</font>" + TableInput(_Ten1, _ID1, _GT1) + TablePass(_Ten2,  _ID2, _GT2) ;
  return tblData;
}
//////////////////////////////////////////////////////////////////////////////////
String Table2Put(String _TieuDe,String _Ten1, String _ID1, String _GT1, String _Ten2, String _ID2, String _GT2)
{
  String tblData = "<font size='18' class='tieude shadow'>" + _TieuDe + "</font>" + TableInput(_Ten1, _ID1, _GT1) + TableInput(_Ten2,  _ID2, _GT2) ;
  return tblData;
}
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
  String MangData[8] = {"","","","","","","",""};
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
     WifiName  = MangData[0]; WifiPass  = MangData[1]; WifiIP  = MangData[2]; WifiGetway = MangData[3];
     AdminName = MangData[4]; AdminPass = MangData[5]; AdminIP = MangData[6]; AdminPort  = MangData[7];
}
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
////////////////////////////////////////////////////////////////////////////////// Getway
void Wifi_LuuGetway(String _WGW){ LuuVungDuLieu(86, 15, _WGW); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadGetway(){ return LoadVungDuLieu(86, 15); }
////////////////////////////////////////////////////////////////////////////////// Admin Name
void Admin_LuuName(String _AName){ LuuVungDuLieu(101, 30, _AName); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadName(){ return LoadVungDuLieu(101, 30); }
////////////////////////////////////////////////////////////////////////////////// Admin Pass
void Admin_LuuPass(String _APass){ LuuVungDuLieu(131, 20, _APass); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadPass(){ return LoadVungDuLieu(131, 20); }
////////////////////////////////////////////////////////////////////////////////// Admin IP
void Admin_LuuIP(String _AIP){ LuuVungDuLieu(151, 15, _AIP); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadIP(){ return LoadVungDuLieu(151, 15); }
////////////////////////////////////////////////////////////////////////////////// Admin Port
void Admin_LuuPort(String _APort){ LuuVungDuLieu(166, 5, _APort); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadPort(){ return LoadVungDuLieu(166, 5); }
//////////////////////////////////////////////////////////////////////////////////
void LuuThongTinCaiDat()
{
  Wifi_LuuName( WifiName );   
  Wifi_LuuPass( WifiPass );   
  Wifi_LuuIP( WifiIP );   
  Wifi_LuuGetway( WifiGetway );
  Admin_LuuName( AdminName ); 
  Admin_LuuPass( AdminPass ); 
  Admin_LuuIP( AdminIP ); 
  Admin_LuuPort( AdminPort );
}

void LoadThongTinCaiDat()
{
  WifiName   = Wifi_LoadName(); 
  WifiPass   = Wifi_LoadPass();
  WifiIP     = Wifi_LoadIP();
  WifiGetway = Wifi_LoadGetway();
  AdminName  = Admin_LoadName();
  AdminPass  = Admin_LoadPass();
  AdminIP    = Admin_LoadIP();
  AdminPort  = Admin_LoadPort();
  
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <IPAddress.h>
#include <PubSubClient.h>

WiFiServer server(80);
//////////////////////////////////////////////////////////////////////////////////
//String LayChuoiIP(IPAddress _IP)
//{
 //   String _ChuoiIP = (String)_IP[0] + "." + (String)_IP[1] + "." + (String)_IP[2] + "." + (String)_IP[3] ;
 //   return _ChuoiIP;
//}
//////////////////////////////////////////////////////////////////////////////////
/*
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

*/
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String TimGiaTri(String ChuoiNhan)
{
  String ChuoiGiaTri   = "";
  String ChuoiNhanDang = "";
  int8_t nKyTu = ChuoiNhan.length();
  for(int8_t i = 0; i< nKyTu; i++)
  {
    String sKyTu = ChuoiNhan.substring(i, i+1);
    if(sKyTu == ":")
    {
       ChuoiNhanDang = ChuoiNhan.substring(0, i);
       ChuoiGiaTri   = ChuoiNhan.substring(i+1, nKyTu);
       break;
    }
  }
  return ChuoiGiaTri;
}
//////////////////////////////////////////////////////////////////////////////////
void LuuKieuHoatDong(String sKieu)
{
   if(sKieu == "1"){ LuuEEPROM(1, 1); }
   if(sKieu == "2"){ LuuEEPROM(1, 2); }
   if(sKieu == "3"){ LuuEEPROM(1, 3); }
}
//////////////////////////////////////////////////////////////////////////////////
void System_Reset(){  Serial.println("MG_WIFIRESET:1"); }
//////////////////////////////////////////////////////////////////////////////////
void TuoiCayNhanh(String _ChuoiData)
{
    String sKieu = TimGiaTri(_ChuoiData);
    Serial.print("MG_TUOICAY:" + sKieu); 
}
//////////////////////////////////////////////////////////////////////////////////
void KieuHoatDong(String _ChuoiData)
{ 
    String sKieu = TimGiaTri(_ChuoiData);
    Serial.print("MG_SYSTEMSTAS:" + sKieu); 
    LuuKieuHoatDong(sKieu);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CapNhatSensor(String sSensor)
{
    String sChuoi      = "";
    uint8_t nThuTu     = 0;
    uint8_t nViTriDau  = 0;
    String GTSensor[4] = {"","","","" };
    uint8_t nKyTu      = sSensor.length();
    
    for(uint8_t i = 0; i< nKyTu; i++)
    {
       String sKyTu = sSensor.substring(i, i+1);
       if(sKyTu == ",")
       {
            sChuoi = sSensor.substring(nViTriDau, i);
            nViTriDau = i+1;
            GTSensor[nThuTu] = sChuoi;
            nThuTu++;
        }
        
    }
    GTSensor[nThuTu] = sSensor.substring(nViTriDau, nKyTu);
    String sNhietDo = GTSensor[0];
    String sDoAmDat = GTSensor[1];
    String sTroiMua = GTSensor[2]; 
    String sTuoiCay = GTSensor[3];
    NhietDo = sNhietDo.toInt();
    DoAmDat = sDoAmDat.toInt();
    TroiMua = sTroiMua.toInt();
    TuoiCay = sTuoiCay.toInt();
    
    if(CaiDatSensor == false)
    {
        LuuEEPROM(2,NhietDo);
        LuuEEPROM(3,DoAmDat);
        LuuEEPROM(4,TroiMua);
        LuuEEPROM(5,TuoiCay);
        CaiDatSensor = true;
    }
}

//////////////////////////////////////////////////////////////////////////////
void CapNhatWifiPass(String sPass)
{
   Wifi_LuuPass(sPass); 
}
//////////////////////////////////////////////////////////////////////////////
void CapNhatWifiName(String sName)
{
   Wifi_LuuName(sName);   
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void SuLyGiaTriChuoiNhan(String ChuoiNhan)
{
  String ChuoiGiaTri   = "";
  String ChuoiNhanDang = "";
  int8_t nKyTu = ChuoiNhan.length();
  for(int8_t i = 0; i< nKyTu; i++)
  {
    String sKyTu = ChuoiNhan.substring(i, i+1);
    if(sKyTu == ":")
    {
       ChuoiNhanDang = ChuoiNhan.substring(0, i);
       ChuoiGiaTri   = ChuoiNhan.substring(i+1, nKyTu);
       break;
    }
  }
  if(ChuoiNhanDang == "WF_SENSORDATA") { CapNhatSensor(ChuoiGiaTri); }
  if(ChuoiNhanDang == "WF_PASSWORD")   { CapNhatWifiPass(ChuoiGiaTri); }
  if(ChuoiNhanDang == "WF_NAME")       { CapNhatWifiName(ChuoiGiaTri); }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//   WifiName, WifiPass, WifiIP, WifiGetway, AdminName, AdminPass, AdminIP, AdminPort;
////////////////////////////////////////////////////////////////////////////////// 
char *wname = "Caphe sang";
char *wpass = "khongbiet";

void setup() {
  EEPROM.begin(512);
  delay(100);
  ///////////////////////////
  Serial.begin(115200);
  delay(100);
  ///////////////////////////
  //LoadThongTinCaiDat();
  TrangThaiHoatDong = LoadEEPROM(1); ////  1 = AOTO, 2 = ON, 3 = OFF
  /////////////////////////// Wifi connect
  //IPAddress wip     = String2IP(WifiIP);
  //IPAddress wgetway = String2IP(WifiIP);
  //IPAddress wsubnet = IPAddress(255,255,255,0);
  //WiFi.config( wip, wgetway, wsubnet);
  //wname = String2Char(Wifi_LoadName());
  //wpass = String2Char(Wifi_LoadPass());
  WiFi.mode(WIFI_STA);
  WiFi.begin(wname, wpass);
   delay(5000);
  //while (WiFi.status() != WL_CONNECTED){ delay(500);}
  KetNoiWifi = true;
  server.begin();
  //////////////////////
  //String sChuoiIP = "WIFI_IP:" + LayChuoiIP(WiFi.localIP());
  Serial.println(WiFi.localIP());
  //////////////////////
  NhietDo = LoadEEPROM(2);
  DoAmDat = LoadEEPROM(3);
  TroiMua = LoadEEPROM(4);
  TuoiCay = LoadEEPROM(5);
 }
////////////////////////////////////////////////////////////////////////////////// 
String ChuoiDuLieu = "";
String html        = "";
String linkdata    = "";
int KieuManHinh    = 0; // 

void loop() 
{
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
   if(ChuoiDuLieu != ""){ SuLyGiaTriChuoiNhan(ChuoiDuLieu); ChuoiDuLieu = ""; }
   ////////////////////////////////////////////////////////
   WiFiClient client = server.available();
   if (!client){ return; }
   if(KetNoiWifi){ while(!client.available()){ delay(5); } }
   linkdata = client.readStringUntil('$');
   client.flush();
   client.println("HTTP/1.1 200 OK");
   client.println("Content-Type: text/html");
   client.println(""); 
   if(linkdata.indexOf("/desktop")   != -1){ KieuManHinh = 0;}
   if(linkdata.indexOf("/refesh")    != -1){ System_Reset(); }
   if(linkdata.indexOf("/hoatdong")  != -1){ Serial.print("MG_SYSTEMSTAS:" + TimGiaTri(linkdata)); }
   if(linkdata.indexOf("/tuoinhanh") != -1){ Serial.print("MG_TUOICAY:"    + TimGiaTri(linkdata)); }
   //if(linkdata.indexOf("/setting")   != -1){ KieuManHinh = 1;}
   //if(linkdata.indexOf("/offline")!= -1){ KieuManHinh = 2;} 
   //if(linkdata.indexOf("/dmtLuuCaiDat")!= -1){ CaiDatWifiData(linkdata); LuuThongTinCaiDat();}
  /////////
//  String DiaChiIP     = LayChuoiIP(WiFi.localIP());
  String CbbTuoiNhanh = "<font class='TimerDown shadow' id='TimerDown'></font> &nbsp; &nbsp; " + ComBoxTuoiNhanh();
  String CbbHoatDong  = ComBoxCheDoHoatDong( TrangThaiHoatDong );
  String sNhietDo     = (String)NhietDo + "&deg;";
  String sDoAmDat     = (String)DoAmDat + "%";
  String sTroiMua     = "KH&Ocirc;NG"; if(TroiMua > 60) { sTroiMua = "M&#431;A"; } ;
  String sTuoiCay     = "OFF"; if(TuoiCay >0) { sTuoiCay = "ON"; };
  if(KieuManHinh == 0)
  {
      client.println(Online_Header());
      client.println(JavaScriptUser());
      client.println(CStyleCSS());
      //client.println(ChuoiDuLieu);
      
      html = TableCamBien("&#272;&#7896; &#7848;M",sDoAmDat);
      html+= TableCamBien("NHI&#7878;T &#272;&#7896;",sNhietDo);
      html+= TableCamBien("T&#431;&#7898;I C&Acirc;Y",sTuoiCay);
      html+= TableCamBien("TR&#7900;I M&#431;A",sTroiMua);
      //html+= TableCombo("HO&#7840;T &#272;&#7896;NG", CbbHoatDong);
      html+= TableCombo("T&#431;&#7898;I", CbbTuoiNhanh);
  }
  /*
  if(KieuManHinh == 1)
  {
      client.println(Header());
      ////////////////////////////////////
      String btSave = "<button class='button bogoc' onclick='LuuSetting()' id='btSave'> Save </button>";
      String btExit = "<button class='button bogoc' onclick='Desktop()'> Exit </button>" ;
      String ConnectStas = "";
      if(KetNoiWifi){ ConnectStas = " - (Connect)"; }else{ ConnectStas = " - (Not connect)";}
      
      html = TablePutPass("Wifi" + ConnectStas ,"Name","wifiname",WifiName,"Password","wifipass",WifiPass);
      html+= TableInput("IP", "wifiip", WifiIP );
      html+= TableInput("Getway", "wifigetway", WifiGetway);
      
      html+= TablePutPass("Admin","Name","admname",AdminName,"Password","admpass",AdminPass);
      html+= TableInput("IP", "admip", AdminIP);
      html+= TableInput("PORT", "admport", AdminPort);
      html+= Table("&nbsp;", btSave + btExit);
      html+= "<textarea rows='4' cols='100' style='width:100%;height:200px' id='dmtkk' style='font-size:20px'></textarea>";
     //html+= Table(NutTrangChu,NutSave);
  }
  if(KieuManHinh == 2)
  {
      client.println(Header());
      ////////////////////////////////////
      html = Table2Put("Login","Namen","logname","admin","Password","logpass","admin");
      html+= Table("&nbsp;","<button class='button bogoc' > Login </button>");
  }
  */
  /////////
  client.println(html);
  client.println("</div></html>");
  delay(1000);
}


