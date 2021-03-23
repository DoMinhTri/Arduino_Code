/////////////////////
#include <Wire.h>                // I2C library
#include <RtcDS3231.h>           // RTC library
 //RtcDS3231 DS3231;             // Uncomment for version 1.0.1 of the rtc library
RtcDS3231<TwoWire> DS3231(Wire); // Uncomment for version 2.0.0 of the rtc library
time_t Nam,Thang,Ngay,Gio,Phut,Giay;
uint8_t NhietDo = 30, DoAm = 35;

/////////////////////
#include <IPAddress.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
ESP8266WebServer server(80);

/////////////////////  Relay
#define Relay1  16
#define Relay2  14
#define Relay3  12
#define Relay4  13

///////////////////// Thời tiết
uint8_t TTOnline      = 1;
uint8_t TroiMua       = 0; // 1 = Mua, 0 = Nang

///////////////////// HMI
bool Load_1_Lan       = false;
uint8_t HMIAction     = 0;
uint8_t HMITextDislay = 0;
String  TenManHinh[5] = {"pHMIMAIN","pHMIWIFI","pHMISETT","pHMIDATE","pHMIHELP"};

///////////////////// CHỉ số phần cứng
String UartData      = "";
String TTCongTac     = "";
boolean KetNoiLanDau = false;
uint8_t MangTTDen[4] = {0,0,0,0};
bool DeviceUpdate    = 0; // Trạng thái thiết bị với Server, nếu chưa đồng bộ thì không cập nhật 

/////////////////////  Wifi 
String WifiName, WifiPass, WifiIP, WifiPort, SVID, DVNUM, AdmPass;
void Restart(){  delay(1000); ESP.restart(); }

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
////////////////////////////////////////////////////////////////////////////////// Admin Name - 30
void Admin_LuuName(String _AdmName){ LuuVungDuLieu(171, 30, _AdmName); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadName(){ return LoadVungDuLieu(171, 30); }
////////////////////////////////////////////////////////////////////////////////// Admin Pass - 20
void Admin_LuuPass(String _AdmPass){ LuuVungDuLieu(201, 20, _AdmPass); }
//////////////////////////////////////////////////////////////////////////////////
String Admin_LoadPass(){ return LoadVungDuLieu(201, 20); }
////////////////////////////////////////////////////////////////////////////////// SVID
void SV_LuuSVID(String _AdmSVID){ LuuVungDuLieu(221, 16, _AdmSVID); }
//////////////////////////////////////////////////////////////////////////////////
String SV_LoadSVID(){ return LoadVungDuLieu(221, 16); }
////////////////////////////////////////////////////////////////////////////////// DVNUM
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
//////////////////////////////////////////////////////////////////////////////////
/////////////////////   Web Interface   //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Desktop() {
  server.send(200, "text/html", "<div align=center><h1>You are connected</h1></div>");
}
//////////////////////////////////////////////////////////////////////////////////
void DeviceID() {
  server.send(200, "text/html", SVID);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////   Fist  Load infor      ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void LuuThongTinCaiDat()
{
    Wifi_LuuName( WifiName );   
    Wifi_LuuPass( WifiPass );   
    Wifi_LuuIP( WifiIP ); 
    Wifi_LuuPort( WifiPort );  
    /////////////////////////////
    Admin_LuuPass( AdmPass );
    /////////////////////////////
    SV_LuuSVID(SVID); 
    SV_LuuDVNUM( DVNUM );
}
//////////////////////////////////////////////////////////////////////////////////
void LoadThongTinCaiDat()
{
    WifiName   = Wifi_LoadName(); 
    WifiPass   = Wifi_LoadPass();
    WifiIP     = Wifi_LoadIP();
    WifiPort   = Wifi_LoadPort();
    /////////////////////////////
    AdmPass    = Admin_LoadPass();
    /////////////////////////////
    SVID       = SV_LoadSVID();
    DVNUM      = SV_LoadDVNUM();
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////       Data online function       ///////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String LayChuoiJson()
{
        HTTPClient http;
        String WebCode  = "";
        String SVLink   = "http://fablabs.store/cttmm/inc/onoff/?UTSID=" + SVID + "&utstype=utsdevice&utsdvnum=" + DVNUM + "&utsvalue=" + TTCongTac + "&utsdvupdate=" + DeviceUpdate;
        //String SVLink = "http://fablabs.store/cttmm/inc/onoff/?UTSID=" + SVID + "&utsvalue=0,1,0,1&utstype=utsdevice&utsdvnum=" + DVNUM + "&utsdvupdate=" + DeviceUpdate;
        http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){ WebCode = http.getString(); }
        } 
        http.end();
        //Serial.println(SVLink); 
        return WebCode;
        //////////
}
//////////////////////////////////////////////////////////////////////////////////
// Hàm phân tích dữ liệu online
void SuLyChuoiTinHieu(String ChuoiData)
{

  String CNhanDang  = ChuoiData.substring(1,3);
  if(CNhanDang == "CT")
  {
        uint8_t TT1  = ChuoiData.substring(5,6).toInt();
        uint8_t TT2  = ChuoiData.substring(7,8).toInt();
        uint8_t TT3  = ChuoiData.substring(9,10).toInt();
        uint8_t TT4  = ChuoiData.substring(11,12).toInt();
        // Khi phát hiện tín hiệu từ màn hình HMI thì kiểm tra dữ liệu online
        // Kiểm tra nếu dữ liệu online đồng bộ thì cho nhập vào hệ thống
        if(DeviceUpdate == 1) KiemTraDataOnline( TT1,  TT2,  TT3,  TT4);
        if(DeviceUpdate == 0)
        {
            MangTTDen[0] = TT1;
            MangTTDen[1] = TT2;
            MangTTDen[2] = TT3;
            MangTTDen[3] = TT4;
        }
  }
  else
  {
      if(ChuoiData == "MCU_RESTART") Restart();  
  }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////       Function        //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////       Hàm kiểm tra dữ liệu online có động bộ với dữ liệu gốc không
void KiemTraDataOnline(uint8_t TT1, uint8_t TT2, uint8_t TT3, uint8_t TT4)
{
    if(TT1 == MangTTDen[0])
    {
         if(TT2 == MangTTDen[1])
         {
               if(TT3 == MangTTDen[2])
               {
                    if(TT4 == MangTTDen[3]) { DeviceUpdate = 0;  }
               }
         }
    }
}
//////////////////////////////////////////////////////////////////////////////////
void BatDen()
{
    digitalWrite(Relay1, MangTTDen[0]); 
    digitalWrite(Relay2, MangTTDen[1]); 
    digitalWrite(Relay3, MangTTDen[2]); 
    digitalWrite(Relay4, MangTTDen[3]); 
    TTCongTac = String(MangTTDen[0]) + "," + String(MangTTDen[1]) + "," + String(MangTTDen[2]) + "," + String(MangTTDen[3]);
}
//////////////////////////////////////////////////////////////////////////////////
void CaiDatIP()
{
        if(KetNoiLanDau == false)
        {
            KetNoiLanDau  = true;
            IPAddress wip = String2IP(WifiIP);
            if(wip != WiFi.localIP())
            { 
                wip = WiFi.localIP();
                Wifi_LuuIP(LayChuoiIP(WiFi.localIP()));
            }
            WiFi.config( wip, WiFi.gatewayIP(), IPAddress(255,255,255,0));
        }
}
///////////////////////////////////////////////////////////////////////////
void UartDataCheck(String UartDataX)
{
    if(UartDataX == "pESPRESTART"){ Restart(); }
    if(UartDataX == "ESPRESTART") { Restart(); }
    String SDau = UartDataX.substring(0, 6);
    String SSau = UartDataX.substring(6, UartDataX.length());
    if(SDau == "WName:"){ Wifi_LuuName( SSau ); UartDataX = "";  }
    if(SDau == "WPass:"){ Wifi_LuuPass( SSau ); UartDataX = "";  }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////       HMI         //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void LayChiSoManHinh(String UartDataX)
{
    
    for(uint8_t i=0; i < sizeof(TenManHinh); i++)
    {
      if(UartDataX == TenManHinh[i]) { HMIAction = i; HMITextDislay = i; }
    }
}
//////////////////////////////////////////////////////////////////////////////////
void HMIDataCheck(String ChuoiData)
{
    UartDataCheck(ChuoiData);
    LayChiSoManHinh(ChuoiData);
    String _KyTuDau = ChuoiData.substring(0, 3);
    String _HMIData = ChuoiData.substring(3, ChuoiData.length());
    if(ChuoiData == "pOneLoad") { Load_1_Lan = true; }
    ///////////
    switch (HMIAction) {
    case 1:
          if(_KyTuDau == "pWN"){ WifiName = _HMIData ;  Wifi_LuuName( WifiName );  }
          if(_KyTuDau == "pWP"){ WifiPass = _HMIData ;  Wifi_LuuPass( WifiPass );  Load_1_Lan = true;}
      break;
    case 2:
          if(_KyTuDau == "pPW"){ AdmPass  = _HMIData ;  Admin_LuuPass( _HMIData ); }
          if(_KyTuDau == "pID"){ SVID     = _HMIData ;  SV_LuuSVID(    _HMIData ); }
          if(_KyTuDau == "pOD"){ DVNUM    = _HMIData ;  SV_LuuDVNUM(   _HMIData ); }
          if(_KyTuDau == "pIP"){ WifiIP   = _HMIData ;  Wifi_LuuIP(    _HMIData ); }
          if(_KyTuDau == "pPO"){ WifiPort = _HMIData ;  Wifi_LuuPort(  _HMIData ); }
      break;
    default:
          if(_KyTuDau == "pN1") { MangTTDen[0] = (MangTTDen[0] + 1)%2; DeviceUpdate = 1; }
          if(_KyTuDau == "pN2") { MangTTDen[1] = (MangTTDen[1] + 1)%2; DeviceUpdate = 1; }
          if(_KyTuDau == "pN3") { MangTTDen[2] = (MangTTDen[2] + 1)%2; DeviceUpdate = 1; }
          if(_KyTuDau == "pN4") { MangTTDen[3] = (MangTTDen[3] + 1)%2; DeviceUpdate = 1; }
    break;
  }
}
//////////////////////////////////////////////////////////////////////////////////
void HMIDislay()
{
  switch (HMITextDislay) {
    case 1:
          HMISendInt("tpWifiVal", TTOnline);
          if(Load_1_Lan)
          {
              HMISendTxT("tWName", WifiName);
              HMISendTxT("tWPass", WifiPass);
              //////////////////////
              uint8_t ap_count = WiFi.scanNetworks();
              if(ap_count > 6) ap_count = 6;
              if(ap_count > 0)
              {
                  for (uint8_t ap_idx = 0; ap_idx < ap_count; ap_idx++)
                  {
                      //String TenWifi = "Duy Khanh 7"; 
                      String TenWifi = String(WiFi.SSID(ap_idx));
                      HMISendTxT("tW" + String(ap_idx) , String(TenWifi));
                  }
              }
              Load_1_Lan = false;
          }
      break;
    case 2:
          HMISendInt("tpWifiVal" , TTOnline );
          if(Load_1_Lan)
          {
            HMISendTxT("tPass"  ,AdmPass);
            HMISendTxT("tDVID"  ,SVID);
            HMISendTxT("tOrder" ,DVNUM);
            HMISendTxT("tIP"    ,WifiIP);
            HMISendTxT("tPort"  ,WifiPort);
            ///////////////
            Load_1_Lan = false;
          }
      break;
    default:
          String Date = String(Ngay) + "/" + String(Thang);
          String Time = String(Gio)  + ":" + String(Phut) + ":" + String(Giay);
          /////////////
          HMISendTxT("tDate" ,Date);
          HMISendTxT("tTimer",Time);
          HMISendTxT("tNhietDo", String(NhietDo) + "*C");
          HMISendTxT("tDoAm"   , String(DoAm)    + "%" );
          /////////////
          HMISendPic("bD1"   , MangTTDen[0] );
          HMISendPic("bD2"   , MangTTDen[1] );
          HMISendPic("bD3"   , MangTTDen[2] );
          HMISendPic("bD4"   , MangTTDen[3] );
          /////////////
          HMISendInt("tpWifiVal" , TTOnline );
          HMISendInt("vMua"      , TroiMua );
          HMISendTxT("tNam"      , String(Nam));
    break;
  }
}
//////////////////////////////////////////////////////////////////////////////////
void HMISendInt(String DoiTuong, int GiaTri)
{
  Serial.print(DoiTuong + ".val=");
  Serial.print(GiaTri);
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
////////////////////////////////////////
void HMISendTxT(String DoiTuong, String GiaTri)
{
  Serial.print(DoiTuong + ".txt=");
  Serial.print("\"" + GiaTri + "\"");
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
////////////////////////////////////////
void HMISendPic(String DoiTuong, uint8_t GiaTri)
{
  Serial.print(DoiTuong + ".picc=");
  Serial.print(GiaTri);
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
////////////////////////////////////////
void HMISendOpjectInt(String DoiTuong, uint8_t GiaTri)
{
  Serial.print(DoiTuong + "=");
  Serial.print(GiaTri);
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
////////////////////////////////////////
void HMISendOpjectStr(String DoiTuong, String GiaTri)
{
  Serial.print(DoiTuong + "=");
  Serial.print(GiaTri);
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////       Main        //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void setup() {
  EEPROM.begin(512);   delay(100);
  Serial.begin(9600);  delay(100);
  LoadThongTinCaiDat();
  ////////////////////////////////////////
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  ////////////////////////////////////////
  WiFi.mode(WIFI_AP_STA);  // WIFI_AP  -  WIFI_AP_STA
  char *wi_ssid  = String2Char(WifiName); // Wifi 
  char *wi_pass  = String2Char(WifiPass);
  WiFi.begin(wi_ssid, wi_pass);
  delay(2000);
//    Serial.print("EEPROM ID:  "); Serial.println(LoadEEPROM(0));
//    Serial.print("WIFI SSID:  "); Serial.println(WifiName);
//    Serial.print("WIFI PASS:  "); Serial.println(WifiPass);
//    Serial.println("Start"); 
  ////////////////////////////////////////
  server.on("/",         Desktop);
  server.on("/deviceid", DeviceID);
  server.begin();
  //////////////////////////////
  DS3231.Begin();     //Starts I2C
}
////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  server.handleClient();
  LayThoiGian();
  BatDen();
  ////////////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ UartData+= KyTu; } delay(5);
  }
  ////////////
  if(UartData != "") { HMIDataCheck(UartData); UartData = ""; }
  ////////////////////////////
  if( WiFi.status() == WL_CONNECTED )
  {
        CaiDatIP(); 
        TTOnline = 1;  
        ////////////
        String Webcode = LayChuoiJson();
        SuLyChuoiTinHieu(Webcode); 
        //Serial.println("");
        //Serial.print("WIFI IP-1:  "); Serial.println(WifiIP); 
        //Serial.print("WIFI IP-2:  "); Serial.println(LayChuoiIP(WiFi.localIP())); 
   }
   else
   {
        TTOnline = 0;
   }
   //////////////////////////////
   HMIDislay();
   delay(500);
   //////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////////
void LayThoiGian()
{
  //Nam,Thang,Ngay,Gio,Phut,Giay;
  RtcDateTime ThoiGian = DS3231.GetDateTime();    //get the time from the RTC
  Nam   =  ThoiGian.Year();    //get year method
  Thang =  ThoiGian.Month();   //get month method
  Ngay  =  ThoiGian.Day();     //get day method
  Gio   =  ThoiGian.Hour();    //get hour method
  Phut  =  ThoiGian.Minute();  //get minute method
  Giay  =  ThoiGian.Second();  //get second method
    
}


