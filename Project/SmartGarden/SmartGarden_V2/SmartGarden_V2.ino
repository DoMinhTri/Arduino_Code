
//String  MangGT[50]      = {"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""};
//uint8_t MangDauPhay[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

 
#include <EEPROM.h>
uint8_t MangTGHen[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature CamBienNhietDo(&oneWire);
int NhietDoThoiGianThuc = millis()/1000;
uint8_t NhietDo ;
uint8_t DoAmDat, TroiMua;
uint8_t SoThoiDiemHen  = 0;
boolean ChoPhepTuoiCay = false;
///////////////////////////////////////////
#include <Sodaq_DS3231.h>
#include <Wire.h>

uint32_t old_ts;
String dayofweek  = "";
int second, minute, hour, date, month, year, Temperature; 
String weekDay[7]      = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
String ThuTrongTuan[7] = {"CN" , "T2" , "T3" , "T4" , "T5" , "T6" , "T7"  };
///////////////////////////////////////////
#define RESET     2
#define RELAY     4
#define TROIMUA   A2
#define DOAMDAT   A3

void setup() 
{
   Serial.begin(38400);
   Wire.begin();
   ////////////////
   pinMode(RELAY, OUTPUT);
   pinMode(RESET, OUTPUT);
   //pinMode(TROIMUA, INPUT);
   ////////////////
   lcd.begin(16, 2); 
   ////////////////
   rtc.begin();
   ////////////////
   CamBienNhietDo.begin();
   ////////////////
   LayNhietDo();
   LayThoiGianHienTai();
   Load_MangTongThoiGianHen();
}
//////////////////////////////////////////////////////////////////////////////
String ChuoiTinNhan = "";
void loop() 
{
  DoAmDat = DocDoAmDat();
  TroiMua = DocTroiMua();
  //KiemTraTuoiCay();
  //KiemTraRelay();
  //Desktop();
    lcd.setCursor(0,0);
    lcd.print("A");
    lcd.setCursor(0,1);
    lcd.print("B");
  /////////////
   while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read(); if(KyTu != '\n' && KyTu != ' '){ ChuoiTinNhan+= KyTu; } delay(5);
  }
  if( ChuoiTinNhan != "")
  { 
      SuLyGiaTriChuoiNhan(ChuoiTinNhan); 
      ChuoiTinNhan = "";  
  }
  /////////////
  //LayThoiGianHienTai();
  //TinhNhietDoThoiGianThuc();
  delay(1000);
}
//////////////////////////////////////////////////////////////////////////////
void KiemTraRelay()
{
  if(ChoPhepTuoiCay) { digitalWrite(RELAY, HIGH); }
  else { digitalWrite(RELAY, LOW); }
}

void ResetHeThong(){ digitalWrite(RESET, HIGH); }
//////////////////////////////////////////////////////////////////////////////
void KiemTraTuoiCay()
{
    ChoPhepTuoiCay = false;
    uint8_t nGio    = MangTGHen[2];
    uint8_t nPhut   = MangTGHen[3];
    uint8_t nTGian  = MangTGHen[4];
    uint8_t nAmMax  = MangTGHen[5];
    uint8_t nAmMin  = MangTGHen[6];
    uint8_t nMuaMax = MangTGHen[7];
    uint8_t nMuaMin = MangTGHen[8];
    if(nGio == hour)
    {
      if((nPhut <= minute) && (minute <= (nPhut + nTGian))){ ChoPhepTuoiCay = true; }
    }
    if(DoAmDat >= nAmMax) { ChoPhepTuoiCay = false; }
    if(TroiMua >= 85)     { ChoPhepTuoiCay = false; }
    //LCD2DongDelay((String)DoAmDat, (String)nAmMax, 2000);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LayThoiGianHienTai()
{
    DateTime ThoiGian = rtc.now(); 
    uint32_t ts  = ThoiGian.getEpoch();
    if (old_ts == 0 || old_ts != ts) 
    {
        old_ts = ts;
        second = ThoiGian.second();
        minute = ThoiGian.minute();
        hour   = ThoiGian.hour();
        date   = ThoiGian.date();
        month  = ThoiGian.month();
        year   = ThoiGian.year();
        dayofweek = ThuTrongTuan[ThoiGian.dayOfWeek()];
        rtc.convertTemperature();
        Temperature = rtc.getTemperature();
    }
}
//////////////////////////////////////////////////////////////////////////////
void Desktop()
{
     String NgayThangNam = (String)date + "/" + (String)month  + "/" + (String)year   + " " + (String)dayofweek   ;
     String GioPhutGiay  = (String)hour + ":" + (String)minute + ":" + (String)second ;
     LCD_2Dong(NgayThangNam,GioPhutGiay);
     //LCD_XemNhietDo();
     //LCD_XemDoAmDat();
     //LCD_XemTroiMua();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
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
  LCD_1DongTrenDelay(ChuoiNhanDang, 2000); 
  if(ChuoiNhanDang      == "DMT_DATE"){ PhanTichChuoiGioVaCaiDat(ChuoiGiaTri);    }
  else if(ChuoiNhanDang == "DMT_WIFI"){ PhanTichChuoiWiFiVaCaiDat(ChuoiGiaTri);   }
  else if(ChuoiNhanDang == "DMT_TIMER")  { PhanTichChuoiHenGioVaCaiDat(ChuoiGiaTri); }
  else if(ChuoiNhanDang == "DMT_CLEAR_TIMER") { ThoiGianClearAll(); }
  else if(ChuoiNhanDang == "SYSTEM_RESET")      { ResetHeThong();     }
  //else{ LCD_1_DongTrenDelay(ChuoiNhan, 2000); BlinkRelay();}
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///////////////////////////   Cai Dat Gia Tri   //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void ThoiGianClearAll()
{
    Luu_SoLuongThoiDiemHen(0);
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
  //LCD2DongDelay("So gio hen Tong:",(String)nSoGioHen,2000);
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
    Wifi_LuuName(sWifiName);
    Wifi_LuuPass(sWifiPass);
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
     DateTime dt(Nam.toInt(),Thang.toInt(),Ngay.toInt(),Gio.toInt(),Phut.toInt(),Giay.toInt()+3,Thu.toInt());
     rtc.setDateTime(dt);
     delay(1000);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////  Nhiet do  - Do am  ////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////       LCD     ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_XemNhietDo()
{
    lcd.setCursor(13,0);
    lcd.print(NhietDo);
    lcd.print((char)223);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_XemDoAmDat()
{
    lcd.setCursor(9,1);
    lcd.print(TroiMua);
    lcd.print("%");
}
//////////////////////////////////////////////////////////////////////////////
void LCD_XemTroiMua()
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
void Load_MangTongThoiGianHen()
{
  SoThoiDiemHen = Load_SoLuongThoiDiemHen();
  if( SoThoiDiemHen > 0 )
  {
     for(uint8_t i=0; i< SoThoiDiemHen*9; i++)
     {
        MangTGHen[i] = EEPROM_RN(71+i);
     }
  }
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
