#include <EEPROM.h>
////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(A2, 13, 12, 11, 10, 9);

////////////////////////////////////
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
const char* Thu_Vi[8]  = {"CN" , "T2" , "T3"  , "T4" , "T5"  , "T6" , "T7"};
const char* Thu_En[8]  = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};
const char* Thang[13]  = {"January", "February", "March", "April", "May", "June", "July", "August","September", "October", "November", "December"};
////////////////////////////////////
#define  mgBuzzer  3
#define  mgRelay   4

#define  mgGiam   A2
#define  mgChon   A1
#define  mgTang   A0

#define  mgSensor A3
////////////////////////////////////
char KyTu;
char MenuChon    = 'M';
int KieuChinh    = 0;
boolean ChinhSua = false;
////////////////////////////////////
int NhietDo ;
int DoAmDat ;
int DoAmMin, DoAmMax;
int NhietDoMin, NhietDoMax;
int GioTuoi, PhutTuoi, ThoiGianTuoi;
int ThoiGianTong[7] = { 2015, 2, 1, 12, 1, 0 }; // -- [Y/M/D H:m:s]  
////////////////////////////////////
boolean DanhDauBanDau = false;
int BanDau  = millis()/1000, DaQua  = BanDau  ;
int TBanDau = millis()/1000, TDaQua = TBanDau ;
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Wire.begin();
  RTC.begin();
  //////////////////
  pinMode(mgRelay,  OUTPUT);
  pinMode(mgBuzzer, OUTPUT);
  pinMode(mgSensor, INPUT);
  pinMode(mgChon, INPUT_PULLUP);
  pinMode(mgTang, INPUT_PULLUP);
  pinMode(mgGiam, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  ThoiGianTuoi = EEPROM_RN(2);
  DoAmMin      = EEPROM_RN(3);
  DoAmMax      = EEPROM_RN(4);
  NhietDoMin   = EEPROM_RN(5);
  NhietDoMax   = EEPROM_RN(6);
  GioTuoi      = EEPROM_RN(7);
  PhutTuoi     = EEPROM_RN(8);;
  LoadThoiGian();
}
///////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  KyTu    = NhanNut(); 
  DoAmDat = GiaTriDoAm(analogRead(mgSensor));
  //BatCongTac();
  ///////////////
  switch (MenuChon) 
  {
      /////////////// Mà hình chính
      case 'C':
           ChonMenu(KyTu);
      break;
      /////////////// Chọn menu
      case 'M':
           Menu(KyTu);
      break;      
      /////////////// Chỉnh thời gian
      case 'T': 
           TuyChinhThoiGian(KyTu);
      break;       
      /////////////// Chỉnh các giá trị
      case 'G': 
           TuyChinhGiaTri(KyTu);
      break;    
   }
  ///////////////
}
void BatCongTac()
{
  DateTime ThoiGian = RTC.now();
  TDaQua = millis()/1000;
  if((ThoiGian.hour() == GioTuoi))
  {
    if((ThoiGian.minute() == PhutTuoi)) 
    {
        digitalWrite(mgRelay, HIGH); 
        if(DanhDauBanDau == false)
        {
          TBanDau = millis()/1000;
          DanhDauBanDau = true;
        }
    }
  }
  if(DanhDauBanDau)
  {
    if((ThoiGianTuoi*60 - (TDaQua - TBanDau)) <=0) digitalWrite(mgRelay, LOW); 
  }

}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  View      //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void ChonMenu(char Nut)
{
    Desktop(); 
    if(Nut == 'g') { XoaManHinh(); MenuChon = 'G';  }
    if(Nut == 'c') { XoaManHinh(); MenuChon = 'M';  }
}
///////////////////////////////////////////////////////////////////////////////////////
void Menu(char Nut)
{
    InDongTren("      Menu");
    const char* Ten[8] = {"1.Date time","2.Do am","3.Nhiet do:","4.Nhiet do: Max","5.Gio tuoi:","6.Phut tuoi","7.Thoi gian tuoi"};  
    if(Nut == 'g') { XoaManHinh(); MenuChon = 'T';  }
    if(Nut == 'c') { XoaManHinh(); MenuChon = 'G';  }
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhThoiGian(char Nut)
{
  const int TGMax[7]  = {3000 , 12, 31, 23, 59, 59};
  const int TGMin[7]  = {2015 ,  1,  1,  0,  0,  0};
  const int nChuot[7] = {3    ,  6,  9,  1,  4,  7};
  ///////////////////////////
  lcd.setCursor(0, 0);
  lcd.print(ThoiGianTong[0]); lcd.print("/"); 
  lcd.print(ThemSoKhong(ThoiGianTong[1])); lcd.print("/"); 
  lcd.print(ThemSoKhong(ThoiGianTong[2])); lcd.print("   ");
  if(KieuChinh < 3) NhayConTroDongTren(nChuot[KieuChinh]);
  ///////////////  
  lcd.setCursor(0, 1);
  lcd.print(ThemSoKhong(ThoiGianTong[3])); lcd.print(":"); 
  lcd.print(ThemSoKhong(ThoiGianTong[4])); lcd.print(":"); 
  lcd.print(ThemSoKhong(ThoiGianTong[5])); lcd.print("   ");
  if(KieuChinh >2 ) NhayConTroDongDuoi(nChuot[KieuChinh]);
  ///////////////
  ChinhTG(Nut, TGMin[KieuChinh], TGMax[KieuChinh]);
  if(KieuChinh > 5) { MenuChon = 'M'; ResetTrangThai(); LuuThoiGian(); }
  ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
const char* Ten[8] = {"1.Do am: Min","2.Do am: Max","3.Nhiet do: Min","4.Nhiet do: Max","5.Gio tuoi:","6.Phut tuoi","7.Thoi gian tuoi"};  
const int Max[8]   = {250           ,           250,              200,              200,           23,           59,             99999};
const int Min[8]   = {1             ,             1,             -100,             -100,            0,            0,                 1};
int GiaTriLuu[8]   = {0             ,             0,                0,                0,            0,            0,                 0};
void TuyChinhGiaTri(char Nut)
{
  LCD2Dong(Ten[KieuChinh], So2ChuoiTrai(GiaTriLuu[KieuChinh]));  
  if(ChinhSua) NhayConTroDongDuoi(15);
  ChinhGiaTri(Nut, Min[KieuChinh], Max[KieuChinh]);
  if(KieuChinh > 6) { MenuChon = 'M'; ResetTrangThai(); }
  ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void ChinhGiaTri(char cNut, int cMin, int cMax)
{
    if(cNut == 't' && (ChinhSua== false)) { ChinhSua = true; }
    if(ChinhSua)
    {
      if(cNut == 'c') { XoaManHinh(); KieuChinh++; }
      if(cNut == 't') { GiaTriLuu[KieuChinh] = GiaTriLuu[KieuChinh]+1; if(GiaTriLuu[KieuChinh] > cMax) GiaTriLuu[KieuChinh] = cMin; }
      if(cNut == 'g') { GiaTriLuu[KieuChinh] = GiaTriLuu[KieuChinh]-1; if(GiaTriLuu[KieuChinh] < cMin) GiaTriLuu[KieuChinh] = cMax; }
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void ChinhTG(char cNut, int Min, int Max)
{
    if(cNut == 't' && (ChinhSua== false)) { ChinhSua = true; }
    if(ChinhSua)
    {
      if(cNut == 'c') { XoaManHinh(); KieuChinh++; }
      if(cNut == 't') { ThoiGianTong[KieuChinh] = ThoiGianTong[KieuChinh]+1; if(ThoiGianTong[KieuChinh] > Max) ThoiGianTong[KieuChinh] = Min; }
      if(cNut == 'g') { ThoiGianTong[KieuChinh] = ThoiGianTong[KieuChinh]-1; if(ThoiGianTong[KieuChinh] < Min) ThoiGianTong[KieuChinh] = Max; }
    }   
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void Desktop()
{
    DateTime ThoiGian = RTC.now();
    TinhNhietDoThoiGianThuc();
    //////////////////    
    lcd.setCursor(0, 0); lcd.print(ThemSoKhong(ThoiGian.day()));
    lcd.print('/');      lcd.print(ThemSoKhong(ThoiGian.month()));
    lcd.print('/');      lcd.print((String)(ThoiGian.year()));
    lcd.print(" -");     lcd.print(DoAmDat,DEC); lcd.print("% "); 
    //////////////////
    lcd.setCursor(0, 1); lcd.print(ThemSoKhong(ThoiGian.hour()));
    lcd.print(':');      lcd.print(ThemSoKhong(ThoiGian.minute()));
    lcd.print(':');      lcd.print(ThemSoKhong(ThoiGian.second()));
    lcd.print('-');      lcd.print(Thu_Vi[ThoiGian.dayOfWeek()]);
    lcd.print("-");      lcd.print(NhietDo,DEC); lcd.print((char)223); lcd.print(' ');
    //////////////////
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  Tools     //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
int GiaTriDoAm(int GiaTri)
{
  return  map(GiaTri, 0, 1023, 0, 250);
}
///////////////////////////////////////////////////////////////////////////////////////
void ResetTrangThai()
{
  KieuChinh = 0;  
  ChinhSua  = false;
}
///////////////////////////////////////////////////////////////////////////////////////
char NhanNut()
{
  int buttonState = 1;
  buttonState = digitalRead(mgGiam);  if(buttonState == 0) { delay(150); return 'g'; }
  buttonState = digitalRead(mgChon);  if(buttonState == 0) { delay(150); return 'c'; }
  buttonState = digitalRead(mgTang);  if(buttonState == 0) { delay(150); return 't'; }
  return ' ';
}
///////////////////////////////////////////////////////////////////////////////////////
String ThemSoKhong(int So)
{
  if(So > 9) 
    return (String)So;
  else
    return "0"+(String)So;
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  EEPROM    //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){  EEPROM.write(DiaChi, KyTu);     delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){  EEPROM.write( DiaChi, SoLuu );  delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)EEPROM.read(DiaChi); }
///////////////////////////////////////////////////////////////////////////////////////
int EEPROM_RN(int DiaChi) { return       EEPROM.read(DiaChi); }
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  Nhiet do  //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void TinhNhietDoThoiGianThuc()
{
  DaQua = millis()/1000;  
  if((DaQua - BanDau) >= 6){ BanDau = DaQua ;  LayNhietDo();  }
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
/////////////////////////  LCD  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void LCD2Dong(String Dong_1, String Dong_2)
{
   lcd.setCursor(0, 0); lcd.print(Dong_1);
   lcd.setCursor(0, 1); lcd.print(Dong_2);
}
///////////////////////////////////////////////////////////////////////////////////////
String So2ChuoiTrai(int So){ return ChoiBenTrai((String)So);}
///////////////////////////////////////////////////////////////////////////////////////
String So2ChuoiPhai(int So){ return ChoiBenPhai((String)So);}
///////////////////////////////////////////////////////////////////////////////////////
String ChoiBenPhai(String Chuoi){ return ChoiBenTraiPhai(Chuoi, 0);}
String ChoiBenTrai(String Chuoi){ return ChoiBenTraiPhai(Chuoi, 1);}
String ChoiBenTraiPhai(String Chuoi, int ViTri) 
{
    String tpChuoi = "";
    int SoKyTu = Chuoi.length() ;
    for(int i=0; i<16-SoKyTu; i++){ tpChuoi = tpChuoi + " "; }
    if(ViTri>0){ Chuoi = tpChuoi + Chuoi; }
    else { Chuoi = Chuoi + tpChuoi; }
    return Chuoi;
}
///////////////////////////////////////////////////////////////////////////////////////
void InHangTren(String Chuoi){ lcd.setCursor(0, 0); lcd.print(Chuoi); }
///////////////////////////////////////////////////////////////////////////////////////
void InHangDuoi(String Chuoi){  lcd.setCursor(0, 1);  lcd.print(Chuoi); }
///////////////////////////////////////////////////////////////////////////////////////
void InKhoangTrang(){ lcd.print("                "); }
///////////////////////////////////////////////////////////////////////////////////////
void XoaManHinh(){ lcd.clear(); }
///////////////////////////////////////////////////////////////////////////////////////
void NhayConTroDongDuoi(int ViTri)
{
    lcd.setCursor(ViTri, 1);
    lcd.blink(); delay(100); lcd.noBlink();
}
///////////////////////////////////////////////////////////////////////////////////////
void NhayConTroDongTren(int ViTri)
{
    lcd.setCursor(ViTri, 0);
    lcd.blink(); delay(100); lcd.noBlink();
}

