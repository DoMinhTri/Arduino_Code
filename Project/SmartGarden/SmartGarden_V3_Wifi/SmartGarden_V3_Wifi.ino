

#include <EEPROM.h>
uint8_t MangTGHen[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
byte Symbol_Mua[8]      = {B00100,B00100,B00100,B01110,B10001,B10101,B10001,B01110};
byte Symbol_Nang[8]     = {B00100,B10001,B01110,B11010,B01011,B01110,B10001,B00100};
byte Symbol_TuoiCay1[8] = { 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00, 0x00, 0x00  };
byte Symbol_TuoiCay2[8] = { 0x00, 0x00, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00  };
////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature CamBienNhietDo(&oneWire);
int NhietDoThoiGianThuc  = millis()/1000;
boolean ChoPhepTuoiCay   = true;
int ThoiDiemCaiDatUuTien = 0;
int ThoiGianTuoiUuTien   = 0;
uint8_t NhietDo, DoAmDat, TroiMua, SoThoiDiemHen = 0;
///////////////////////////////////////////
#include <Sodaq_DS3231.h>
#include <Wire.h>
String NgayCuaTuan = "";
int year;
uint8_t second, minute, hour, date, month, Temperature; 
String ThuTrongTuanEn[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"    };
String ThuTrongTuan[8]   = {"CN" , "T2" , "T3" , "T4" , "T5" , "T6" , "T7","CN"};
///////////////////////////////////////////
#define RESET      2
#define RELAY      4
#define RESETWIFI  A0
#define TROIMUA    A2
#define DOAMDAT    A3
///////////////////////////////////////////
#include <SoftwareSerial.h>
SoftwareSerial Esp8266(6, 5); // RX, TX 
///////////////////////////////////////////
boolean HeThongHoatDong = true;
void setup() 
{
   Serial.begin(38400);
   Wire.begin();
   Esp8266.begin(38400);   // ESP8266
   ResetWifi();
   ////////////////
   pinMode(RELAY, OUTPUT);
   pinMode(RESET, OUTPUT);
   pinMode(RESETWIFI, OUTPUT);
   ////////////////
   lcd.createChar(0, Symbol_Nang);
   lcd.createChar(1, Symbol_Mua);
   lcd.createChar(2, Symbol_TuoiCay1);
   lcd.createChar(3, Symbol_TuoiCay2);
   lcd.begin(16, 2); 
   ////////////////
   rtc.begin();
   ////////////////
   CamBienNhietDo.begin();
   ////////////////
   LayNhietDo();
   LayThoiGianHienTai();
   LayMangTongThoiGianHen();
   
}
//////////////////////////////////////////////////////////////////////////////
String ChuoiTinNhan  = "";
String ChuoiWifiData = "";
void loop() 
{
  DoAmDat = DocDoAmDat();
  TroiMua = DocTroiMua();
  if(HeThongHoatDong){ KiemTraTuoiCay(); }
  KiemTraRelay();
  Desktop();
  /////////////
  while (Esp8266.available() > 0 ) 
  {
      char KyTu = Esp8266.read();  uint8_t nbKyTu = (int)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiWifiData+= KyTu; } delay(5);
  }
  /////////////
  if( ChuoiWifiData != "")
  { 
      SuLyGiaTriChuoiNhan(ChuoiWifiData);
      ChuoiWifiData = "";  
  }
  String sChuoiSS = "WF_SENSORDATA:" + (String)NhietDo + "," + (String)DoAmDat + "," + (String)TroiMua + "," + (String)ChoPhepTuoiCay;
  Esp8266.print(sChuoiSS);
  Serial.println(sChuoiSS);
  /////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read(); uint8_t nbKyTu = (int)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiTinNhan+= KyTu; }   delay(5);
  }
  /////////////
  if( ChuoiTinNhan != "")
  { 
      SuLyGiaTriChuoiNhan(ChuoiTinNhan); 
      ChuoiTinNhan = "";  
  }
  /////////////
  LayThoiGianHienTai();
  TinhNhietDoThoiGianThuc();
  delay(1000);
}
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
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////        Kiem Tra - Desktop       ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Desktop()
{
     String NgayThangNam = (String)date + "/" + (String)month  + "/" + (String)year   + " " + (String)NgayCuaTuan   ;
     String GioPhutGiay  = (String)hour + ":" + (String)minute + ":" + (String)second ;
     LCD_2Dong(NgayThangNam,GioPhutGiay);
     LCD_XemNhietDo();
     LCD_TuoiCay();
     LCD_XemTroiMua();
     LCD_XemDoAmDat();
}
//////////////////////////////////////////////////////////////////////////////
void KiemTraRelay(){  if(ChoPhepTuoiCay){ digitalWrite(RELAY, HIGH); } else { digitalWrite(RELAY, LOW); } }
void ResetHeThong(){ digitalWrite(RESET, HIGH); }
void ResetWifi(){ digitalWrite(RESETWIFI, HIGH); delay(1000); digitalWrite(RESETWIFI, LOW); }
//////////////////////////////////////////////////////////////////////////////
void KiemTraTuoiCay()
{
    ChoPhepTuoiCay = false;
    uint8_t nGSoioHen = Load_SoLuongThoiDiemHen();
    for(uint8_t i=0; i<nGSoioHen; i++)
    {
        uint8_t nKieuTuoi = MangTGHen[1 + 9*i];
        uint8_t nGio      = MangTGHen[2 + 9*i];
        uint8_t nPhut     = MangTGHen[3 + 9*i];
        uint8_t nTGian    = MangTGHen[4 + 9*i];
        uint8_t nAmMax    = MangTGHen[5 + 9*i];
        uint8_t nAmMin    = MangTGHen[6 + 9*i];
        uint8_t nNhietMax = MangTGHen[7 + 9*i];
        uint8_t nNhietMin = MangTGHen[8 + 9*i];
        
        if(nGio == hour)
        {
            if(nPhut <= minute)
            { 
                if(minute < (nPhut + nTGian)){  ChoPhepTuoiCay = true; }
            }
            if(nKieuTuoi > 0)
            {
                if(DoAmDat >= nAmMax) { ChoPhepTuoiCay = false; }
                if(nKieuTuoi == 2)
                {
                    if(NhietDo > nNhietMax ) { ChoPhepTuoiCay = false; }
                    if(NhietDo < nNhietMin ) { ChoPhepTuoiCay = false; }
                }
            }
        }
        if(TroiMua > 75) { ChoPhepTuoiCay = false; }
    }
    KiemTraGioUuTien();
    if(ThoiGianTuoiUuTien > 0) { ChoPhepTuoiCay = true; }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///////////////////////////   Cai Dat Gia Tri   //////////////////////////////////
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
  LCD_1DongTrenDelay(ChuoiNhanDang, 500); 
  if(ChuoiNhanDang      == "MG_DATE") { PhanTichChuoiGioVaCaiDat(ChuoiGiaTri);    }
  else if(ChuoiNhanDang == "MG_WIFI") { PhanTichChuoiWiFiVaCaiDat(ChuoiGiaTri);   }
  else if(ChuoiNhanDang == "MG_TIMER"){ PhanTichChuoiHenGioVaCaiDat(ChuoiGiaTri); }
  else if(ChuoiNhanDang == "MG_CLEAR_TIMER") { ThoiGianClearAll();       }
  else if(ChuoiNhanDang == "MG_SYSTEM_RESET"){ ResetHeThong();           }
  else if(ChuoiNhanDang == "MG_RELAY")       { BatTatRelay(ChuoiGiaTri); }
  else if(ChuoiNhanDang == "MG_TUOICAY")     { CaiDatUuTien(ChuoiGiaTri);}
  else if(ChuoiNhanDang == "MG_WIFIRESET")   { ResetWifi();              }
  else if(ChuoiNhanDang == "WIFI_IP")        { LCD_1DongTrenDelay(ChuoiGiaTri, 1000);}
  else if(ChuoiNhanDang == "WF_SENSORDATA")  { CapNhatSensor(ChuoiGiaTri);}
  //else                                       { ATCOMMAND(ChuoiGiaTri);   }
  //else{ LCD_1DongTrenDelay(ChuoiNhan, 2000); BlinkRelay();}
}
//////////////////////////////////////////////////////////////////////////////////
void KiemTraGioUuTien()
{
  if(ThoiGianTuoiUuTien > 0)
  {
      if((millis()/1000 - ThoiDiemCaiDatUuTien) > ThoiGianTuoiUuTien){ ThoiGianTuoiUuTien = 0; }
  }
}
//////////////////////////////////////////////////////////////////////////////////
void CaiDatUuTien(String nTimer)
{
   ThoiGianTuoiUuTien   = nTimer.toInt()*60;
   ThoiDiemCaiDatUuTien = millis()/1000;
}
//////////////////////////////////////////////////////////////////////////////////
void BatTatRelay(String sRelay)
{
  String ON = sRelay.substring(0, 2);
  if(ON == "ON"){ digitalWrite(RELAY, HIGH); delay(5000); }
  else { digitalWrite(RELAY, LOW);}
}
//////////////////////////////////////////////////////////////////////////////////
void ThoiGianClearAll(){ Luu_SoLuongThoiDiemHen(0); }
//////////////////////////////////////////////////////////////////////////////////
void PhanTichChuoiHenGioVaCaiDat(String sChuoi)
{
  uint8_t i;
  uint8_t nThuTu = 1;
  uint8_t nDauPhay[10] = {0,0,0,0,0,0,0,0,0,0}; 
  String ChuoiTG[9]   = {"","","","","","","","",""};
  for( i=0; i< sChuoi.length(); i++)
  {
    if(sChuoi.substring(i, i+1) == ",")
    { 
      nDauPhay[nThuTu] = i; 
      nThuTu++;
    }
  }
  nDauPhay[nThuTu] = sChuoi.length();
  uint8_t GTDem = 0;
  for( i=0; i< nThuTu  ;i++)
  {
    ChuoiTG[i] = sChuoi.substring(nDauPhay[i] + GTDem, nDauPhay[i+1]);
    GTDem = 1;
  }
  ///////////
  uint8_t nSoGioHen = Load_SoLuongThoiDiemHen();
  
  if(nSoGioHen > 4 ){ nSoGioHen = 0; }
  int nDiaChiLuu = nSoGioHen*9 + 71;
  for(i=0; i< nThuTu; i++)
  {
    uint8_t nGTTian = String2Int(ChuoiTG[i]);
    EEPROM_WN(nDiaChiLuu + i, nGTTian);
  }
  if(nSoGioHen < 4 ) nSoGioHen++;
  Luu_SoLuongThoiDiemHen(nSoGioHen);
}
//////////////////////////////////////////////////////////////////////////////
void PhanTichChuoiWiFiVaCaiDat(String sChuoi)
{
     uint8_t i;
     for( i=0; i< sChuoi.length(); i++)
     {
       if(sChuoi.substring(i, i+1) == ","){break;}
     }
    String sWifiName = sChuoi.substring(0, i);
    String sWifiPass = sChuoi.substring(i+1, sChuoi.length());
    Wifi_LuuName(sWifiName); Esp8266.print("WF_NAME:" + sWifiName);
    Wifi_LuuPass(sWifiPass); Esp8266.print("WF_PASSWORD:" + sWifiPass);
}
//////////////////////////////////////////////////////////////////////////////
void PhanTichChuoiGioVaCaiDat(String ChuoiGio)
{
     uint8_t i = 0;
     uint8_t nThuTu = 0;
     uint8_t nDauPhay[6] = {0,0,0,0,0,0}; 
     String Nam, Thang, Ngay, Gio, Phut, Giay, Thu;
     for(i=0; i< ChuoiGio.length(); i++)
     {
       if(ChuoiGio.substring(i, i+1) == ","){ nDauPhay[nThuTu] = i; nThuTu++; }
     }
     
     Nam   = ChuoiGio.substring(0,  nDauPhay[0]);
     Thang = ChuoiGio.substring(nDauPhay[0]+1, nDauPhay[1]);
     Ngay  = ChuoiGio.substring(nDauPhay[1]+1, nDauPhay[2]);
     Gio   = ChuoiGio.substring(nDauPhay[2]+1, nDauPhay[3]);
     Phut  = ChuoiGio.substring(nDauPhay[3]+1, nDauPhay[4]);
     Giay  = ChuoiGio.substring(nDauPhay[4]+1, nDauPhay[5]);
     Thu   = ChuoiGio.substring(nDauPhay[5]+1, i);

     //LCD_1DongTrenDelay(Nam, 1000); LCD_1DongTrenDelay(Thang, 1000); LCD_1DongTrenDelay(Ngay, 1000); 
     DateTime dt(Nam.toInt(),Thang.toInt(),Ngay.toInt(),Gio.toInt(),Phut.toInt(),Giay.toInt()+3,Thu.toInt());
     rtc.setDateTime(dt);
     delay(1000);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////        Cam Bien  - Thoi Gian    ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LayNhietDo()
{
  CamBienNhietDo.requestTemperatures(); 
  NhietDo = CamBienNhietDo.getTempCByIndex(0);
}
///////////////////////////////////////////////////////////////////////////////////////
void TinhNhietDoThoiGianThuc()
{
  if((millis()/1000 - NhietDoThoiGianThuc) > 15){ NhietDoThoiGianThuc = millis()/1000 ; LayNhietDo(); }
}
//////////////////////////////////////////////////////////////////////////////////
int DocDoAmDat()
{
  int nDoAmDat = analogRead(DOAMDAT);
  return  map(nDoAmDat, 0, 1023, 0, 100);
}
//////////////////////////////////////////////////////////////////////////////////
int DocTroiMua()
{
  int nTroiMua = analogRead(TROIMUA);
  return  map(nTroiMua, 0, 1023, 0, 100);
}
//////////////////////////////////////////////////////////////////////////////
void LayThoiGianHienTai()
{
    DateTime now = rtc.now(); 
    second = now.second();
    minute = now.minute();
    hour   = now.hour();
    date   = now.date();
    month  = now.month();
    year   = now.year();
    NgayCuaTuan = ThuTrongTuan[now.dayOfWeek()];
    //rtc.convertTemperature();
    //Temperature = rtc.getTemperature();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////       LCD     ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_TuoiCay()
{
      lcd.setCursor(9,1);  
      uint8_t ChangLe = millis()/1000;
      if(ChoPhepTuoiCay)
      {  
        if( ChangLe%2 == 0 )
            lcd.write(byte(2));  
        else
            lcd.write(byte(3));  
      }
}
//////////////////////////////////////////////////////////////////////////////
void LCD_XemNhietDo()
{
    lcd.setCursor(13,0);
    lcd.print(NhietDo);
    lcd.print((char)223);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_XemTroiMua()
{
    lcd.setCursor(11,1);
    if(TroiMua < 50)
     {  
        lcd.write(byte(0)); 
        lcd.noBlink();
     }
     else
     {
        lcd.write(byte(1));
        lcd.setCursor(11,1);
        lcd.noBlink();
        delay(100);
        lcd.blink();
     }
}
//////////////////////////////////////////////////////////////////////////////
void LCD_XemDoAmDat()
{
    if(DoAmDat >  9) lcd.setCursor(13,1);
    if(DoAmDat < 10) lcd.setCursor(14,1);
    lcd.print(DoAmDat);
    lcd.print("%");
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_2Dong(String Chuoi_1,String Chuoi_2)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(Chuoi_1);
    lcd.setCursor(0,1);
    lcd.print(Chuoi_2);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_2DongDelay(String Chuoi_1,String Chuoi_2, int nDelay)
{
    LCD_2Dong(Chuoi_1, Chuoi_2);
    delay(nDelay);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTren(String sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenDelay(String sChuoi, int nTimer)
{
    LCD_1DongTren(sChuoi);
    delay(nTimer);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenN(int sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenNDelay(int nChuoi, int tDelay)
{
    LCD_1DongTrenN(nChuoi);
    delay(tDelay);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoi(String sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiDelay(String sChuoi, int nTimer)
{
    LCD_1DongDuoi(sChuoi);
    delay(nTimer);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiN(int sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiNDelay(int sChuoi, int nDelay)
{
    LCD_1DongDuoiN(sChuoi);
    delay(nDelay);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DDViDelay(String sChuoi, uint8_t nViTri, int nDelay)
{
    lcd.setCursor(nViTri,1);
    lcd.print(sChuoi);
    delay(nDelay);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      EEPROM      ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void LuuEEPROM(int nDiaChi, int nGiaTri){ EEPROM.write(nDiaChi, nGiaTri);  delay(10); }
//////////////////////////////////////////////////////////////////////////////////
int LoadEEPROM(int nDiaChi){ delay(10);  return EEPROM.read(nDiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){ EEPROM.write( DiaChi, KyTu );  delay(20); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){ EEPROM.write( DiaChi, SoLuu ); delay(20); }
//////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
int EEPROM_RN(int DiaChi){ return EEPROM.read(DiaChi); }
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
  for(int i = _BatDau; i< _BatDau + _DoDai; i++){ EEPROM_WN( i,245);}
  EEPROM_WS(_BatDau, _ChuoiLuu);
}
//////////////////////////////////////////////////////////////////////////////////
String LoadVungDuLieu(int _BatDau, int _DoDai)
{
  String _strChuoi = "";
  for(int i = _BatDau; i< _BatDau + _DoDai; i++)
  { 
      int tmpChar = EEPROM_RN(i);
      if(tmpChar != 245){ _strChuoi+=  (char)tmpChar ; }
  }
  return _strChuoi;
}
////////////////////////////////////////////////////////////////////////////////// Name
void Wifi_LuuName(String _WName){ LuuVungDuLieu(21, 30, _WName); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadName(){ return LoadVungDuLieu(21, 30); }
////////////////////////////////////////////////////////////////////////////////// Pass
void Wifi_LuuPass(String _WPass){ LuuVungDuLieu(51, 16, _WPass); }
//////////////////////////////////////////////////////////////////////////////////
String Wifi_LoadPass(){ return LoadVungDuLieu(51, 16); }
////////////////////////////////////////////////////////////////////////////////// So luong thoi diem hen
uint8_t Load_SoLuongThoiDiemHen(){ return EEPROM_RN(70); }
//////////////////////////////////////////////////////////////////////////////////
void Luu_SoLuongThoiDiemHen(uint8_t nSoLuong){ EEPROM_WN(70, nSoLuong); };
//////////////////////////////////////////////////////////////////////////////////
void LayMangTongThoiGianHen()
{
  SoThoiDiemHen = Load_SoLuongThoiDiemHen();
  if(SoThoiDiemHen > 4 ) 
  { 
      Luu_SoLuongThoiDiemHen(0); 
      LCD_1DongDuoiDelay("TIMER ERROR", 1000);  
  }
  else 
  {
     if( SoThoiDiemHen > 0 ) 
     {
       for(uint8_t i=0; i< SoThoiDiemHen*9; i++){ MangTGHen[i] = EEPROM_RN(71+i); }
     }
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

