#include <EEPROM.h>
uint8_t MangTGHen[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
byte Symbol_Mua[8]      = {B00100,B00100,B00100,B01110,B10001,B10101,B10001,B01110};
byte Symbol_Nang[8]     = {B00100,B10001,B01110,B11010,B01011,B01110,B10001,B00100};
byte Symbol_TuoiCay1[8] = { 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00, 0x00, 0x00  };
byte Symbol_TuoiCay2[8] = { 0x00, 0x00, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00  };
byte Symbol_So_U[8]     = {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b11111};
byte Symbol_So_1[8]     = {0b11111,0b11011,0b10011,0b11011,0b11011,0b11011,0b10001,0b11111};
byte Symbol_So_2[8]     = {0b11111,0b10001,0b10101,0b11101,0b11011,0b10111,0b10001,0b11111};
byte Symbol_So_3[8]     = {0b11111,0b10001,0b11101,0b10001,0b11101,0b11101,0b10001,0b11111};
////////////////////////////////////////////
bool ChoPhepTuoiCay  = true;
uint8_t NhietDo, DoAmDat_1, DoAmDat_2, DoAmDat_3, DoAm_Mua;
///////////////////////////////////////////
#include "Sodaq_DS3231.h"
#include <Wire.h>
uint16_t  _nam;
uint8_t   _ThuTuVanTuoi; 
uint8_t   _thu, _giay, _phut, _gio, _ngay, _thang; 
String ThuTrongTuanEn[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
String ThuTrongTuan[8]   = {"" , "CN", "T2", "T3", "T4", "T5", "T6", "T7"};
///////////////////////////////////////////
#define RESET       2

#define RELAY_1     4
#define RELAY_2     4
#define RELAY_3     4
#define RELAY_AC    4

#define DOAM_MUA    A2
#define DOAMDAT_P1  A3
#define DOAMDAT_P2  A3
#define DOAMDAT_P3  A3
//////////////////////////////////////////////////////////////////////////////
DateTime dt(2016, 4, 26, 11, 1, 15, 3);
uint32_t  ThoiGianThuc;
void setup() {
   Serial.begin(38400);
   Wire.begin();
   rtc.begin();
   //rtc.setDateTime(dt);
   ////////////////
   pinMode(RELAY_1, OUTPUT);
   pinMode(RESET, OUTPUT);
   digitalWrite(RELAY_1,HIGH);
   ////////////////
   lcd.createChar(0, Symbol_So_U);
   lcd.createChar(1, Symbol_So_1);
   lcd.createChar(2, Symbol_So_2);
   lcd.createChar(3, Symbol_So_3);
   lcd.createChar(6, Symbol_Nang);
   lcd.createChar(7, Symbol_Mua);
   //lcd.createChar(8, Symbol_TuoiCay1);
   //lcd.createChar(9, Symbol_TuoiCay2);
   lcd.begin(16, 2); 
}
//////////////////////////////////////////////////////////////////////////////
String ChuoiTinNhan  = "";
void loop() {
  ThoiGianThuc = millis()/1000;
  LayThoiGianVaCamBienMoiTruong();
  Desktop();
  ///////////// Serial input
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
  //delay(700);
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
  //else if(ChuoiNhanDang == "MG_CLEAR_TIMER") { ThoiGianClearAll();       }
  //else if(ChuoiNhanDang == "MG_SYSTEM_RESET"){ ResetHeThong();           }
  //else if(ChuoiNhanDang == "MG_RELAY")       { BatTatRelay(ChuoiGiaTri); }
  //else if(ChuoiNhanDang == "MG_TUOICAY")     { CaiDatUuTien(ChuoiGiaTri);}
  //else if(ChuoiNhanDang == "MG_WIFIRESET")   { ResetWifi();              }
  //else if(ChuoiNhanDang == "WIFI_IP")        { LCD_1DongTrenDelay(ChuoiGiaTri, 1000);}
  //else                                       { ATCOMMAND(ChuoiGiaTri);   }
  //else{ LCD_1_DongTrenDelay(ChuoiNhan, 2000); BlinkRelay();}
}

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
   //Wifi_LuuName(sWifiName); Esp8266.print("WF_NAME:" + sWifiName);
   // Wifi_LuuPass(sWifiPass); Esp8266.print("WF_PASSWORD:" + sWifiPass);
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
/////////////////////////        Kiem Tra - Desktop       ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Desktop()
{
     String NgayThangNam = (String)_ngay + "/" + (String)_thang  + "-" + (String)_nam  ;
     String GioPhutGiay  = (String)_gio  + ":" + (String)_phut   + ":" + (String)_giay ;
     LCD_2Dong(NgayThangNam,GioPhutGiay);
     lcd.setCursor(9,0); lcd.print(ThuTrongTuan[_thu]);             // Thứ
     lcd.setCursor(12,0); lcd.print(NhietDo); lcd.print((char)223);  // Nhiệt dộ
     LCD_XemDoAmDat();
     LCD_XemTroiMua();
     LCD_TuoiCay();
     
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////        Cam Bien  - Thoi Gian    ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LayThoiGianVaCamBienMoiTruong()
{
    DateTime now = rtc.now(); 
    _giay  = now.second();
    _phut  = now.minute();
    _gio   = now.hour();
    _ngay  = now.date();
    _thang = now.month()+10;
    _nam   = now.year()-2000;
    _thu   = now.dayOfWeek();
    //////
    rtc.convertTemperature();
    NhietDo = rtc.getTemperature();
    //////
    LayDoAmMoiTruong();
}
//////////////////////////////////////////////////////////////////////////////
void LayDoAmMoiTruong()
{
  int nChiSoDoAm = analogRead(DOAM_MUA);
  DoAm_Mua     = map(nChiSoDoAm, 0, 1023, 0, 100);
    nChiSoDoAm = analogRead(DOAMDAT_P1);
  DoAmDat_1    = map(nChiSoDoAm, 0, 1023, 0, 100);
    nChiSoDoAm = analogRead(DOAMDAT_P2);
  DoAmDat_2    = map(nChiSoDoAm, 0, 1023, 0, 100);
    nChiSoDoAm = analogRead(DOAMDAT_P3);
  DoAmDat_3    = map(nChiSoDoAm, 0, 1023, 0, 100);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////       LCD     ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_TuoiCay()
{
      if(ChoPhepTuoiCay)
      {
         _ThuTuVanTuoi = 2;
         lcd.setCursor(15,1);  
         lcd.print(_ThuTuVanTuoi); 
         if(ThoiGianThuc%2 == 0)
         {
            lcd.setCursor(15,1); 
            lcd.print("*");
            delay(200);
            lcd.setCursor(15,1);  
            lcd.print(_ThuTuVanTuoi);
         }
      }
}
//////////////////////////////////////////////////////////////////////////////
void LCD_XemTroiMua()
{
    lcd.setCursor(15,0);
    if(DoAm_Mua < 50){ lcd.write(byte(6)); }
    else{ lcd.write(byte(7)); }
}
//////////////////////////////////////////////////////////////////////////////
void LCD_XemDoAmDat() 
{
    uint8_t nTpTime = ThoiGianThuc%3;
    uint8_t MangDoAm[3] = {DoAmDat_1, DoAmDat_2, DoAmDat_3};
    lcd.setCursor(9,1);
    lcd.print(nTpTime + 1);
    lcd.print(" ");
    lcd.print(MangDoAm[nTpTime]);
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
void LCD_Link(uint16_t nTime)
{
    lcd.write(byte(6)); 
    delay(nTime);
    lcd.write(byte(5)); 
}
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
/////////////////////      Text function      ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////

int String2Int(String sSo)
{
  return sSo.toInt();
}
//////////////////////////////////////////////////////////////////////////////////

int NgayTrongTuan(int d, int m, int y){
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
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
  /*
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
  */
}


