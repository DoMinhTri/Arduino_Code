


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
const char* Thu_Vi[]  = {"CN" , "T2" , "T3"  , "T4" , "T5"  , "T6" , "T7"};
const char* Thu_En[]  = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};
const char* Thang[]   = {"January", "February", "March", "April", "May", "June", "July", "August","September", "October", "November", "December"};
////////////////////////////////////
#define  mgBuzzer  3
#define  mgRelay   4

#define  mgGiam   A2
#define  mgChon   A1
#define  mgTang   A0

#define  mgSensor  A3
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
int ThoiGianTong[7] = { 2015, 2, 1, 12, 1, 0 }; // -- [Y/M/D H:m:s]  
////////////////////////////////////
int BanDau = millis()/1000, DaQua = BanDau;
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Wire.begin();
  RTC.begin();
  sensors.begin();
  LayNhietDo();
  //////////////////
  pinMode(mgRelay,  OUTPUT);
  pinMode(mgBuzzer, OUTPUT);
  pinMode(mgSensor, INPUT);
  pinMode(mgChon, INPUT_PULLUP);
  pinMode(mgTang, INPUT_PULLUP);
  pinMode(mgGiam, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  DoAmMin    = LoadDoAmMin();
  DoAmMax    = LoadDoAmMax();
  NhietDoMin = LoadNhietDoMin();
  NhietDoMax = LoadNhietDoMax();
  //LoadThoiGian();
  LuuThoiGian();
}
///////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  KyTu    = NhanNut(); 
  DoAmDat = GiaTriDoAm(analogRead(mgSensor));
  ///////////////
  switch (MenuChon) 
  {
      case 'M':
           MainMenu(KyTu);
      break;
      /////////////// Chinh do am
      case 'A': 
           TuyChinhDoAm(KyTu);
      break;    
      /////////////// Chinh nhiet do
      case 'N': 
           TuyChinhNhietDo(KyTu);
      break;  
      /////////////// Chinh thoi gian
      case 'T': 
           TuyChinhThoiGian(KyTu);
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
void MainMenu(char Nut)
{
    ThoiGian(); 
    if(Nut == 'c') { XoaManHinh(); MenuChon = 'A';  }
}///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhDoAm(char Nut)
{
   if(Nut == 't') { ChinhSua = true;  } 
   InHangTren("I.Do am:");
   lcd.setCursor(0, 1);
   lcd.print("Min:");    lcd.print(DoAmMin); 
   lcd.print("   Max:"); lcd.print(DoAmMax); lcd.print("    ");
   ///////////////   
   if(ChinhSua == false)
   {
       if(Nut == 'c') { XoaManHinh(); MenuChon = 'N';  }
       if(Nut == 'g') { XoaManHinh(); MenuChon = 'M'; ResetTrangThai(); }
   }
   else
   {
       switch (KieuChinh) 
       {
          case 0:       
                if(Nut == 'c') { KieuChinh = 1; }
                if(Nut == 't') { DoAmMin++; if(DoAmMin >250)  DoAmMin = 1;   }
                if(Nut == 'g') { DoAmMin--; if(DoAmMin <1)    DoAmMin = 250; }
                LuuDoAmMin();
                NhayConTroDongDuoi(6);
          break;
          ///////////////
          case 1:       
                if(Nut == 'c') { XoaManHinh(); MenuChon = 'N'; ResetTrangThai(); }
                if(Nut == 't') { DoAmMax++; if(DoAmMax >250)  DoAmMax = 1;   }
                if(Nut == 'g') { DoAmMax--; if(DoAmMax <1)    DoAmMax = 250; }
                LuuDoAmMax();
                NhayConTroDongDuoi(15);      
          break;
       }
       ///////////////
   }
   ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhNhietDo(char Nut)
{
   if(Nut == 't') { ChinhSua = true;  }
   InHangTren("II.Nhiet do:");
   lcd.setCursor(0, 1);
   lcd.print("Min:");    lcd.print(NhietDoMin); 
   lcd.print(" Max:"); lcd.print(NhietDoMax); lcd.print("    ");
   ///////////////   
   if(ChinhSua == false)
   {
       if(Nut == 'c') { XoaManHinh(); MenuChon = 'T';  }
       if(Nut == 'g') { XoaManHinh(); MenuChon = 'A'; ResetTrangThai(); }
   }
   else
   {
       switch (KieuChinh) 
       {
          case 0:       
                if(Nut == 'c') { KieuChinh = 1; }
                if(Nut == 't') { NhietDoMin++; if(NhietDoMin >200)  NhietDoMin = -100;  }
                if(Nut == 'g') { NhietDoMin--; if(NhietDoMin <-100) NhietDoMin = 200;   }
                LuuNhietDoMin();
                NhayConTroDongDuoi(6);
          break;
          ///////////////
          case 1:       
                if(Nut == 'c') { XoaManHinh(); MenuChon = 'T'; ResetTrangThai(); }
                if(Nut == 't') { NhietDoMax++; if(NhietDoMax >200)  NhietDoMax = -100;  }
                if(Nut == 'g') { NhietDoMax--; if(NhietDoMax <-100) NhietDoMax = 200;   }
                LuuNhietDoMax();
                NhayConTroDongDuoi(15);      
          break;
       }
       ///////////////
   }
   ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhThoiGian(char Nut)
{
   if(Nut == 't') { ChinhSua = true;  }
   InHangTren("III.Thoi gian:");
   lcd.setCursor(0, 1);
   lcd.print("Min:");    lcd.print(NhietDoMin); 
   ///////////////   
   if(ChinhSua == false)
   {
       if(Nut == 'c') { XoaManHinh(); MenuChon = 'T';  }
       if(Nut == 'g') { XoaManHinh(); MenuChon = 'A'; ResetTrangThai(); }
   }
   else
   {
       switch (KieuChinh) 
       {
          case 0:       
                if(Nut == 'c') { KieuChinh = 1; }
                if(Nut == 't') { NhietDoMin++; if(NhietDoMin >200)  NhietDoMin = -100;  }
                if(Nut == 'g') { NhietDoMin--; if(NhietDoMin <-100) NhietDoMin = 200;   }
                LuuNhietDoMin();
                NhayConTroDongDuoi(6);
          break;
          ///////////////
          case 1:       
                if(Nut == 'c') { XoaManHinh(); MenuChon = 'T'; ResetTrangThai(); }
                if(Nut == 't') { NhietDoMax++; if(NhietDoMax >200)  NhietDoMax = -100;  }
                if(Nut == 'g') { NhietDoMax--; if(NhietDoMax <-100) NhietDoMax = 200;   }
                LuuNhietDoMax();
                NhayConTroDongDuoi(15);      
          break;
       }
       ///////////////
   }
   ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void TuyChinhKieuSuDung(char Nut)
{
  InHangTren("IV.Kieu dung:   ");
  if(Nut == 'c') { XoaManHinh();  MenuChon = 'M'; }
  if(Nut == 'g') { XoaManHinh();  MenuChon = 'T'; }
  ///////////////
}

void HamTuyChinh2GiaTri(char cNut, boolean cCSua, char MNext, char MBack, int &GT_1, int Min_1, int Max_1, int Luu_1, int Chuot_1, int &GT_2, int Min_2, int Max_2, int Luu_2, int Chuot_2)
{
   ///////////////   
   if(cChinhSua == false)
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
                if(Nut == 'c') { XoaManHinh(); MenuChon = MNext; ResetTrangThai(); }
                if(Nut == 't') { GT_2++; if(GT_2 >Max_2)  GT_2 = Min_2; }
                if(Nut == 'g') { GT_2--; if(GT_2 <Min_2)  GT_2 = Max_2; }
                EEPROM_WN(Luu_2, GT_2);
                NhayConTroDongDuoi(Chuot_2);      
          break;
       }
       ///////////////
   }
   ///////////////
}
///////////////////////////////////////////////////////////////////////////////////////
void ThoiGian()
{
    DateTime ThoiGian = RTC.now();
    TinhNhietDoThoiGianThuc();
    //////////////////    
    lcd.setCursor(0, 0); lcd.print(ThemSoKhong(ThoiGian.day()));
    lcd.print('/');      lcd.print(ThemSoKhong(ThoiGian.month()));
    lcd.print('/');      lcd.print((String)(ThoiGian.year()));
    lcd.print(" -");      lcd.print(DoAmDat,DEC); lcd.print("% "); 
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
void ResetTrangThai()
{
  KieuChinh = 0;  
  ChinhSua  = false;
}
///////////////////////////////////////////////////////////////////////////////////////
void PlayBuzzer()
{
  digitalWrite(mgBuzzer, HIGH);  delay(1000); 
  digitalWrite(mgBuzzer, LOW);   delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////////
void PlayRelay()
{
  digitalWrite(mgRelay, HIGH);  delay(1000); 
  digitalWrite(mgRelay, LOW);   delay(1000);
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
void InHangTren(String Chuoi)
{
  lcd.setCursor(0, 0);
  lcd.print(Chuoi);
}
///////////////////////////////////////////////////////////////////////////////////////
void InHangDuoi(String Chuoi)
{
  lcd.setCursor(0, 1);
  lcd.print(Chuoi);
}
///////////////////////////////////////////////////////////////////////////////////////
void InKhoangTrang()
{
  lcd.print("                ");
}
///////////////////////////////////////////////////////////////////////////////////////
void XoaManHinh()
{
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
void LuuDoAmMin(){    EEPROM_WN(1, DoAmMin);} int LoadDoAmMin(){    return EEPROM_RN(1); }
void LuuDoAmMax(){    EEPROM_WN(2, DoAmMax);} int LoadDoAmMax(){    return EEPROM_RN(2); }
void LuuNhietDoMin(){ EEPROM_WN(3, DoAmMin);} int LoadNhietDoMin(){ return EEPROM_RN(3); }
void LuuNhietDoMax(){ EEPROM_WN(4, DoAmMax);} int LoadNhietDoMax(){ return EEPROM_RN(4); }
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
String ThemSoKhong(int So)
{
  if(So > 9) 
    return (String)So;
  else
    return "0"+(String)So;
}
///////////////////////////////////////////////////////////////////////////////////////
int GiaTriDoAm(int GiaTri)
{
  return  map(GiaTri, 0, 1023, 0, 250);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TinhNhietDoThoiGianThuc()
{
  DaQua = millis()/1000;  
  if((DaQua - BanDau) >= 6) 
  {
      BanDau = DaQua ;
      LayNhietDo();
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LayNhietDo()
{
    sensors.requestTemperatures(); 
    NhietDo = sensors.getTempCByIndex(0);
}
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
