#include <EEPROM.h>
////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 13, 11, 7, 10, 9);
////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
////////////////////////////////////
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
const char* Thu_Vi[8]  = {  "CN" , "T2" , "T3"  , "T4" , "T5"  , "T6" , "T7"};
const char* Thu_En[8]  = {  "Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};
const char* Thang[13]  = {  "January", "February", "March", "April", "May", "June", "July", "August","September", "October", "November", "December"};
////////////////////////////////////
#define  mgBuzzer  3
#define  mgRelay   4

#define  mgGiam    A2
#define  mgChon    A1
#define  mgTang    A0

#define  mgSensor  A3
////////////////////////////////////
char KyTu;
char MenuChon    = 'M';
int KieuChinh    = 0;
boolean ChinhSua = false;
////////////////////////////////////
int KieuTuoi; // Kiểu chọn chế độ sử dụng
int NhietDo ;
int DoAmDat ;
int DoAmMin, DoAmMax;
int NhietDoMin, NhietDoMax;
int GioHen, PhutHen, GioTuoi, PhutTuoi;
int ThoiGianTong[7] = { 2015, 2, 1, 12, 1, 0 }; // -- [Y/M/D H:m:s]  
////////////////////////////////////
boolean ChoPhepTuoiCay  = false;
int NhietDoThoiGianThuc = millis()/1000;
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Wire.begin();
  LayNhietDo();
  sensors.begin();
  RTC.begin();
  if (! RTC.isrunning()) { RTC.adjust(DateTime(__DATE__, __TIME__)); }
  //////////////////
  pinMode(mgRelay,  OUTPUT);
  pinMode(mgBuzzer, OUTPUT);
  pinMode(mgSensor, INPUT);
  pinMode(mgChon, INPUT_PULLUP);
  pinMode(mgTang, INPUT_PULLUP);
  pinMode(mgGiam, INPUT_PULLUP);
  //////////////////
  lcd.begin(16, 2);
  LoadEEPROM();  
  LoadThoiGian();
}
///////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  KyTu    = NhanNut(); 
  DoAmDat = GiaTriDoAm(analogRead(mgSensor));
  TuoiCay();
  ///////////////
  switch (MenuChon) 
  {
    /////////////// Man hinh chinh
  case 'M':
    MainMenu(KyTu);
    break;
    /////////////// Chinh thoi gian
  case 'T': 
    TuyChinhThoiGian(KyTu);
    break;       
    /////////////// Chinh do am
  case 'A': 
    TuyChinhDoAm(KyTu);
    break;    
    /////////////// Chinh nhiet do
  case 'N': 
    TuyChinhNhietDo(KyTu);
    break;  
    /////////////// Chinh gio hen
  case 'G': 
    TuyChinhGioHen(KyTu);
    break;   
    /////////////// Chinh thoi gian tuoi
  case 'S': 
    TuyChinhThoiGianTuoi(KyTu);
    break;        
    /////////////// Chon kieu su dung
  case 'K': 
    TuyChinhKieuSuDung(KyTu);
    break;     
    ///////////////
  }
  ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void LoadEEPROM()
{
  KieuTuoi   = EEPROM_RN(1);
  DoAmMin    = EEPROM_RN(2);
  DoAmMax    = EEPROM_RN(3);
  NhietDoMin = EEPROM_RN(4);
  NhietDoMax = EEPROM_RN(5);
  GioHen     = EEPROM_RN(6);
  PhutHen    = EEPROM_RN(7);
  GioTuoi    = EEPROM_RN(8);
  PhutTuoi   = EEPROM_RN(9);
}
///////////////////////////////////////////////////////////////////////////////////////
int GiaTriDoAm(int GiaTri) { return  (250- map(GiaTri, 0, 1023, 0, 250)); }
///////////////////////////////////////////////////////////////////////////////////////
void ResetTrangThai(){ KieuChinh = 0; ChinhSua  = false; }
void ResetRelay() { TatRelay();  ChoPhepTuoiCay = false; }
void BatRelay(){ if(ChoPhepTuoiCay) digitalWrite(mgRelay, HIGH); }
///////////////////////////////////////////////////////////////////////////////////////
void TatRelay(){ digitalWrite(mgRelay, LOW);  }
///////////////////////////////////////////////////////////////////////////////////////
int CoiHuThoiGianThuc;
void CapNhatGioCoiHu(){ CoiHuThoiGianThuc = millis()/1000; }
void CoiHu()
{ 
  if(ChoPhepTuoiCay)
  {
    //if(((millis()/1000) - CoiHuThoiGianThuc)<= 1){ digitalWrite(mgBuzzer, HIGH);  }
    //else { digitalWrite(mgBuzzer, LOW); }
  }
}
///////////////////////////////////////////////////////////////////////////////////////
void KiemTraDoAmMin()
{ 
    if(ChoPhepTuoiCay == false)
    { 
        CapNhatGioCoiHu();    
        if(DoAmDat <= DoAmMin ) { ChoPhepTuoiCay = true;  } 
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void KiemTraDoAmMinUp(){ if(DoAmDat > DoAmMin ) { ResetRelay(); }}
///////////////////////////////////////////////////////////////////////////////////////
void KiemTraDoAmMax(){ if(DoAmMax <= DoAmDat){ ResetRelay(); }}
///////////////////////////////////////////////////////////////////////////////////////
void KiemTraNhietDo(){ if((NhietDo <= NhietDoMin) || (NhietDoMax <= NhietDo)) { ResetRelay(); } }
///////////////////////////////////////////////////////////////////////////////////////
void KiemTraDungGio()
{
   if(ChoPhepTuoiCay == false)
   {
      CapNhatGioCoiHu();
      DateTime ThoiGian = RTC.now();
      if((GioHen == ThoiGian.hour()) && (PhutHen == ThoiGian.minute())) { ChoPhepTuoiCay = true; }
   }
}
///////////////////////////////////////////////////////////////////////////////////////
void KiemTraGioHen()
{
  DateTime ThoiGian     = RTC.now();
  int tpGio  = (GioHen  + GioTuoi)%24;
  int tpPhut = (PhutHen + PhutTuoi)%60;
  if((ThoiGian.hour() == tpGio) && (ThoiGian.minute() == tpPhut)) { ResetRelay(); }
}
///////////////////////////////////////////////////////////////////////////////////////
void TuoiCay()
{
  if(KieuTuoi > 0)
  {
    //  "0.Stop", "1.Do am", "2.Do am-Hen gio", "3.Do am-Nhiet do", "4.Hen gio", "5.Hen gio-Nhiet", "6.[Gio]-Nhiet-Am", "7.Gio-Nhiet-[Am]"
    switch (KieuTuoi)
    {
      ////////////// 1.Do am
      case 1:
          KiemTraDoAmMin();
          if(ChoPhepTuoiCay) 
          { 
            KiemTraDoAmMax();
            
            CoiHu();
            BatRelay();
          } else {ResetRelay(); }
      break;
      //////////////  2.[Do am]-Hen gio
      case 2:
         KiemTraDungGio();
         if(ChoPhepTuoiCay) 
         { 
             KiemTraDoAmMax();
             
             CoiHu();
             BatRelay();
         } else {ResetRelay(); }
      break;
      //////////////  2.Do am-[Hen gio]
      case 3:
         KiemTraDungGio();
         if(ChoPhepTuoiCay) 
         { 
             KiemTraDoAmMax();
             KiemTraGioHen();
             
             CoiHu();
             BatRelay();
         } else {ResetRelay(); }
      break;      
      //////////////    4.[Do am]-Nhiet do    
      case 4:
          KiemTraDoAmMin();
          if(ChoPhepTuoiCay) 
          { 
             KiemTraDoAmMax(); 
             KiemTraNhietDo();
             
             CoiHu();
             BatRelay();
          } else {ResetRelay(); }
      break;
      //////////////    5.Hen gio
      case 5:
          KiemTraDungGio();
          if(ChoPhepTuoiCay) 
          { 
             KiemTraGioHen();
             
             CoiHu();
             BatRelay();
          } else {ResetRelay(); }
      break;
      ////////////// 5.[Hen gio]-Nhiet
      case 6: 
           KiemTraDungGio();
           if(ChoPhepTuoiCay) 
           { 
             KiemTraNhietDo(); 
             KiemTraGioHen();
             
             CoiHu();
             BatRelay();
           } else {ResetRelay(); }
      break;
      ////////////// 6.[Gio]-Nhiet-Am
      case 7:
           KiemTraDungGio();
           if(ChoPhepTuoiCay) 
           { 
             KiemTraDoAmMax(); 
             KiemTraNhietDo(); 
             KiemTraGioHen();
             
             CoiHu();
             BatRelay();
           } else {ResetRelay(); }
      break;
      ////////////// 7.Gio-Nhiet-[Am]
      case 8:
           KiemTraDungGio();
           if(ChoPhepTuoiCay) 
           { 
             KiemTraDoAmMax();
             KiemTraNhietDo();
             
             CoiHu();
             BatRelay();
           } else {ResetRelay(); }
      break;      
      //////////////    
    }
    ////////
  }
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void MainMenu(char Nut)
{
  Desktop(); 
  if(Nut == 'c') { XoaManHinh(); MenuChon = 'A'; }
  if(Nut == 'g') { XoaManHinh(); MenuChon = 'T'; }
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhThoiGian(char Nut)
{
  const int TGMax[7]  = { 3000 , 12, 31, 23, 59, 59  };
  const int TGMin[7]  = { 2015 ,  1,  1,  0,  0,  0  };
  const int nChuot[7] = { 3    ,  6,  9,  1,  4,  7  };
  ///////////////////////////
  lcd.setCursor(0, 0);
  lcd.print(ThoiGianTong[0]); 
  lcd.print("/"); 
  lcd.print(So2ChuoiZero(ThoiGianTong[1])); 
  lcd.print("/"); 
  lcd.print(So2ChuoiZero(ThoiGianTong[2])); 
  lcd.print("   ");
  if(KieuChinh < 3) NhayConTroDongTren(nChuot[KieuChinh]);
  ///////////////  
  lcd.setCursor(0, 1);
  lcd.print(So2ChuoiZero(ThoiGianTong[3])); 
  lcd.print(":"); 
  lcd.print(So2ChuoiZero(ThoiGianTong[4])); 
  lcd.print(":"); 
  lcd.print(So2ChuoiZero(ThoiGianTong[5])); 
  lcd.print("   ");
  if(KieuChinh >2 ) NhayConTroDongDuoi(nChuot[KieuChinh]);
  ///////////////
  ChinhTG(Nut, TGMin[KieuChinh], TGMax[KieuChinh]);
  if(KieuChinh > 5) { MenuChon = 'M'; ResetTrangThai(); LuuThoiGian(); }
  ///////////////
}
void ChinhTG(char cNut, int Min, int Max)
{
  if(cNut == 'c') { KieuChinh++; }
  if(cNut == 't') { ThoiGianTong[KieuChinh] = ThoiGianTong[KieuChinh]+1; if(ThoiGianTong[KieuChinh] > Max) ThoiGianTong[KieuChinh] = Min; }
  if(cNut == 'g') { ThoiGianTong[KieuChinh] = ThoiGianTong[KieuChinh]-1; if(ThoiGianTong[KieuChinh] < Min) ThoiGianTong[KieuChinh] = Max; }
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhDoAm(char Nut)
{
  InHangTren("I.Do am:");
  lcd.setCursor(0, 1);
  lcd.print("Min:");   
  lcd.print(So3Chu(DoAmMin)); 
  lcd.print("  Max:"); 
  lcd.print(So3Chu(DoAmMax)); 
  lcd.print("    ");
  HamTuyChinh2GiaTri(Nut, 'N', 'M', DoAmMin, 1, 250, 2, 6, DoAmMax, 1, 250, 3, 15);
  ///////////////
  //HamTuyChinh2GiaTri(char cNut, char MNext, char MBack, int &GT_1, int Min_1, int Max_1, int Luu_1, int Chuot_1, int &GT_2, int Min_2, int Max_2, int Luu_2, int Chuot_2)
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhNhietDo(char Nut)
{
  InHangTren("II.Nhiet do:");
  lcd.setCursor(0, 1);
  lcd.print("Min:");  
  lcd.print(So3Chu(NhietDoMin)); 
  lcd.print(" Max:"); 
  lcd.print(So3Chu(NhietDoMax)); 
  lcd.print("    ");
  HamTuyChinh2GiaTri(Nut, 'G', 'A', NhietDoMin, -100, 200, 4, 6, NhietDoMax, -100, 200, 5, 14);
  ///////////////   
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhGioHen(char Nut)
{
  InHangTren("III.Gio hen:   ");
  lcd.setCursor(0, 1);
  lcd.print("Gio:");     
  lcd.print(So2Chu(GioHen)) ; 
  lcd.print("   Phut:"); 
  lcd.print(So2Chu(PhutHen)); 
  lcd.print("    ");
  HamTuyChinh2GiaTri(Nut, 'S', 'N', GioHen, 0, 23, 6, 5, PhutHen, 0, 59, 7, 15);
  ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhThoiGianTuoi(char Nut)
{
  InHangTren("IV.T-Gian tuoi:    ");
  lcd.setCursor(0, 1);
  lcd.print("Gio:");     
  lcd.print(So2Chu(GioTuoi)) ; 
  lcd.print("   Phut:"); 
  lcd.print(So2Chu(PhutTuoi)); 
  lcd.print("    ");
  HamTuyChinh2GiaTri(Nut, 'K', 'G', GioTuoi, 0, 23, 8, 5, PhutTuoi, 0, 59, 9, 15);
  ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhKieuSuDung(char Nut)
{
  const char* KieuChon[10]  = { "0.Stop", "1.Do am", "2.[Am]-Hen gio", "3.Am-[Hen gio]", "4.[Am]-Nhiet", "5.Hen gio", "6.[Gio]-Nhiet", "7.[Gio]-Nhiet-Am", "8.Gio-Nhiet-[Am]"  };
  InHangTren("V.Tuy chon:  [8]");
  InHangDuoi(ChuoiBenPhai(KieuChon[KieuTuoi]));
  //lcd.setCursor(0, 1); lcd.print(KieuChinh); lcd.print("-"); lcd.print(Nut); lcd.print("-");  lcd.print(ChinhSua);
  ///////////////
  if(Nut == 'g' ) { XoaManHinh(); MenuChon = 'S'; ResetTrangThai(); }
  if(Nut == 'c' && KieuChinh == 0) { KieuChinh = 1  ; Nut == ' ';  }
  if(Nut == 'c' && ChinhSua == false) { MenuChon  = 'M'; ResetRelay(); ResetTrangThai(); }
  if(Nut == 't' ) { ChinhSua = true ; }
  if(ChinhSua)
  {
    if(Nut == 't') { KieuTuoi++; XoaManHinh(); if(KieuTuoi > 8) KieuTuoi = 0; }  
    if(Nut == 'c')
    { 
      ResetRelay();
      MenuChon = 'M'; XoaManHinh(); ResetTrangThai(); 
      LCD2Dong("-> Ban da chon:", KieuChon[KieuTuoi]); 
      EEPROM_WN(1, KieuTuoi); // Luu on/off vao vi tri o nho [1]
      delay(2000); 
    }
  }
  ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void HamTuyChinh2GiaTri(char cNut, char MNext, char MBack, int &GT_1, int Min_1, int Max_1, int Luu_1, int Chuot_1, int &GT_2, int Min_2, int Max_2, int Luu_2, int Chuot_2)
{
  if(cNut == 't') { if(ChinhSua == false ) { cNut = ' '; }; ChinhSua = true;  } 
  //lcd.setCursor(11, 0); lcd.print(cNut); lcd.print('-'); lcd.print(KieuChinh); lcd.print('-'); lcd.print(ChinhSua);
  ///////////////
  if(ChinhSua == false)
  {
    if(cNut == 'c') { XoaManHinh(); MenuChon = MNext;  }
    if(cNut == 'g') { XoaManHinh(); MenuChon = MBack; ResetTrangThai(); }
  }
  else
  {
    switch (KieuChinh) 
    {
      case 0:       
        if(cNut == 'c') { KieuChinh = 1; }
        if(cNut == 't') { GT_1++; if(GT_1 >Max_1) GT_1 = Min_1; }
        if(cNut == 'g') { GT_1--; if(GT_1 <Min_1) GT_1 = Max_1; }
        EEPROM_WN(Luu_1, GT_1);
        NhayConTroDongDuoi(Chuot_1);
      break;
      ///////////////
      case 1:       
        if(cNut == 'c') { XoaManHinh(); MenuChon = MNext; ResetTrangThai(); }
        if(cNut == 't') { GT_2++; if(GT_2 >Max_2)  GT_2 = Min_2; }
        if(cNut == 'g') { GT_2--; if(GT_2 <Min_2)  GT_2 = Max_2; }
        EEPROM_WN(Luu_2, GT_2);
        NhayConTroDongDuoi(Chuot_2);      
      break;
    }
    ///////////////
  }
  ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void Desktop()
{
  DateTime ThoiGian = RTC.now();
  TinhNhietDoThoiGianThuc();
  //////////////////    
  lcd.setCursor(0, 0); 
  lcd.print(So2ChuoiZero(ThoiGian.day()));
  lcd.print('/');      
  lcd.print(So2ChuoiZero(ThoiGian.month()));
  lcd.print('/');      
  lcd.print((String)(ThoiGian.year()));
  lcd.print(" -");     
  lcd.print(So3Chu(DoAmDat)); 
  lcd.print('%'); 
  //////////////////
  lcd.setCursor(0, 1); 
  lcd.print(So2ChuoiZero(ThoiGian.hour()));
  lcd.print(':');      
  lcd.print(So2ChuoiZero(ThoiGian.minute()));
  lcd.print(':');      
  lcd.print(So2ChuoiZero(ThoiGian.second()));
  lcd.print('-');      
  lcd.print(Thu_Vi[ThoiGian.dayOfWeek()]);
  lcd.print("- ");      
  lcd.print(NhietDo,DEC); 
  lcd.print((char)223); 
  lcd.print(' ');
  if(ChoPhepTuoiCay) { NhayConTroDongDuoi(11); }
  //////////////////
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  Tools     //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
char NhanNut()
{
  int buttonState = 1;  
  buttonState = digitalRead(mgGiam); if(buttonState == 0) { delay(200); return 'g'; }
  buttonState = digitalRead(mgChon); if(buttonState == 0) { delay(200); return 'c'; }
  buttonState = digitalRead(mgTang); if(buttonState == 0) { delay(200); return 't'; }
  return ' ';
}
///////////////////////////////////////////////////////////////////////////////////////
String Chuoi2So(int So){  return ThemChuoiDem("0", (String)So); }
///////////////////////////////////////////////////////////////////////////////////////
String Chuoi3So(int So){  return ThemChuoiDem("00", (String)So); }
///////////////////////////////////////////////////////////////////////////////////////
String ThemChuoiDem(String ChuoiDem, String Chuoi){ return (ChuoiDem + Chuoi); }
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  EEPROM    //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){ EEPROM.write(DiaChi, KyTu); delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){ EEPROM.write( DiaChi, SoLuu ); delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)EEPROM.read(DiaChi); }
///////////////////////////////////////////////////////////////////////////////////////
int EEPROM_RN(int DiaChi) { return  EEPROM.read(DiaChi);  }
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  Nhiet do  - Do am  /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void TinhNhietDoThoiGianThuc()
{
  int DaQua = millis()/1000;  
  if((DaQua - NhietDoThoiGianThuc) >= 2){ NhietDoThoiGianThuc = DaQua ; LayNhietDo(); }
}
///////////////////////////////////////////////////////////////////////////////////////
void LayNhietDo()
{
  sensors.requestTemperatures(); 
  NhietDo = sensors.getTempCByIndex(0);
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  RTC  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void LoadThoiGian()
{
  //int ThoiGianTong[7] = {2015, 1, 1, 12, 0, 0}; // -- [Y/M/D H:m:s]  
  DateTime ThoiGian = RTC.now();
  ThoiGianTong[0] = ThoiGian.year();
  ThoiGianTong[1] = ThoiGian.month();
  ThoiGianTong[2] = ThoiGian.day();
  ThoiGianTong[3] = ThoiGian.hour();
  ThoiGianTong[4] = ThoiGian.minute();
  ThoiGianTong[5] = ThoiGian.second();
}
///////////////////////////////////////////////////////////////////////////////////////
void LuuThoiGian()
{
  //int ThoiGianTong[7] = {2015, 1, 1, 12, 0, 0}; // -- [Y/M/D H:m:s]  
  RTC.adjust(DateTime(ThoiGianTong[0], ThoiGianTong[1], ThoiGianTong[2], ThoiGianTong[3], ThoiGianTong[4], ThoiGianTong[5]));
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  LCD  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void LCD2Dong(String Dong_1, String Dong_2)
{
  lcd.setCursor(0, 0); 
  lcd.print(Dong_1);
  lcd.setCursor(0, 1); 
  lcd.print(Dong_2);
}
///////////////////////////////////////////////////////////////////////////////////////
void InHangTren(String Chuoi){ 
  lcd.setCursor(0, 0); 
  lcd.print(Chuoi); 
}
///////////////////////////////////////////////////////////////////////////////////////
void InHangDuoi(String Chuoi){  
  lcd.setCursor(0, 1);  
  lcd.print(Chuoi); 
}
///////////////////////////////////////////////////////////////////////////////////////
void InKhoangTrang(){ 
  lcd.print("                "); 
}
///////////////////////////////////////////////////////////////////////////////////////
void XoaManHinh(){ 
  lcd.clear(); 
}
///////////////////////////////////////////////////////////////////////////////////////
void NhayConTroDongDuoi(int ViTri)
{
  lcd.setCursor(ViTri, 1);
  lcd.blink(); 
  delay(100); 
  lcd.noBlink();
}
///////////////////////////////////////////////////////////////////////////////////////
void NhayConTroDongTren(int ViTri)
{
  lcd.setCursor(ViTri, 0);
  lcd.blink(); 
  delay(100); 
  lcd.noBlink();
}
///////////////////////////////////////////////////////////////////////////////////////
String So3Chu(int So){  
  return ChuoiBenTraiPhaiChonSo((String)So, 1, 3, ' ') ;
}
///////////////////////////////////////////////////////////////////////////////////////
String So2Chu(int So){ 
  return ChuoiBenTraiPhaiChonSo((String)So, 1, 2, ' ') ;
}
String So2ChuoiZero(int So){ 
  return ChuoiBenTraiPhaiChonSo((String)So, 1, 2, '0'); 
}
///////////////////////////////////////////////////////////////////////////////////////
String So2ChuoiTrai(int So){ 
  return ChuoiBenTrai((String)So);
}
///////////////////////////////////////////////////////////////////////////////////////
String So2ChuoiPhai(int So){ 
  return ChuoiBenPhai((String)So);
}
///////////////////////////////////////////////////////////////////////////////////////
String ChuoiBenPhai(String Chuoi){ 
  return ChuoiBenTraiPhai(Chuoi, 1);
}
///////////////////////////////////////////////////////////////////////////////////////
String ChuoiBenTrai(String Chuoi){ 
  return ChuoiBenTraiPhai(Chuoi, 0);
}
///////////////////////////////////////////////////////////////////////////////////////
String ChuoiBenTraiPhai(String Chuoi, int ViTri) 
{
  return ChuoiBenTraiPhaiChonSo(Chuoi, ViTri, 16, ' ') ;
}
///////////////////////////////////////////////////////////////////////////////////////
String ChuoiBenTraiPhaiChonSo(String Chuoi, int ViTri, int DoDaiChuoi, char KyTuDem) 
{
  String tpChuoi = "";
  int SoKyTu = Chuoi.length() ;
  for(int i=0; i<DoDaiChuoi-SoKyTu; i++){ 
    tpChuoi = tpChuoi + KyTuDem; 
  }
  if(ViTri>0){ 
    Chuoi = tpChuoi + Chuoi; 
  }
  else { 
    Chuoi = Chuoi + tpChuoi; 
  }
  return Chuoi;
}

