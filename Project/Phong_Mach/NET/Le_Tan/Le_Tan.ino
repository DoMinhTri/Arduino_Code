#include <EEPROM.h>
/////////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(A0, 13, 12, 11, 10, 9);

#define Led     A2
#define LCDLed  6
#define Chuong  7

String Chuoi            = "";
int ThoiDiemBatChuong   = 0;
int TGBatChuong         = 9;
boolean ChoBatChuong    = false;
boolean ChoPhapChopDen  = false;
String ChuoiSoBenhNhan  = "";
String ChuoiTenPhong    = "";
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void setup(void) 
{
    Serial.begin(9600);
    ////////////////////  IN - OUT
    pinMode(Led,     OUTPUT);
    pinMode(Chuong,  OUTPUT);
    pinMode(LCDLed,  OUTPUT);
    digitalWrite(LCDLed,HIGH);
    //////////////////////////   
    TGBatChuong = EEPROM.read(1);
    if(TGBatChuong > 10) TGBatChuong = 10;
    //////////////////////////   
    ManHinhCho();
}
/////////////////////////////////////////////////////////////////////////////////////////


void loop(void) 
{
  boolean KiemTraChuoi = true;
  ////////////////////////////////
  while (Serial.available() > 0) 
  {
      char kk = Serial.read();
      Chuoi = Chuoi + kk;
      KiemTraChuoi = false;
   }  
  ////////////////////////////////   
  if(KiemTraChuoi)
  {
    if(Chuoi != "")
    { 
      String KyTu = "";
      int VT1 = 0,VT2 = 0;
      for(int i=0; i <= Chuoi.length();i++)
      {
          KyTu = Chuoi.substring(i,i + 1);
          if(KyTu == "-" && VT1 == 0) VT1 = i;
          if(KyTu == "-" && VT1 >  0) VT2 = i;
      }
      ////////////////////  In thong tin ra LCD 
      String BenhNhanMoi = Chuoi.substring(0, VT1);
      String SoBenhNhan  = Chuoi.substring(VT1 + 1, VT2);
      String TenPhong    = Chuoi.substring(VT2 + 1, Chuoi.length());
      Chuoi = "";
      //////////////////// 
      if(BenhNhanMoi == "chuong")
      {
        TGBatChuong = SoBenhNhan.toInt();
        EEPROM.write(1,TGBatChuong);
        SetThoiDiemBatChuong();
        ManHinhKhach();
        if(ChuoiSoBenhNhan  == "0") {  ManHinhCho(); ChoPhapChopDen = false; } else {  ChoPhapChopDen = true;}
      }
      else
      {
        ChuoiSoBenhNhan = SoBenhNhan;
        ChuoiTenPhong   = TenPhong;
        ManHinhKhach();
        //////////////////// Kiem tra che do bat den
        if(ChuoiSoBenhNhan  == "0") {  ManHinhCho(); ChoPhapChopDen = false; } else {  ChoPhapChopDen = true;}
        //////////////////// Kiem tra che do bat chuong
        if(BenhNhanMoi == "yes"){ SetThoiDiemBatChuong(); }
        ////////////////////   
      }
    }
  }
  ////////// 
  SetThoiGianTatChuong();
  if(ChoPhapChopDen) { ChopDen(); }
  delay(100);
  ////////// 
}
////////////////////////////////  
void SetThoiDiemBatChuong()
{
    ThoiDiemBatChuong = millis()/1000;
    ChoBatChuong   = true;
    BatChuong();
}
////////////////////////////////  
void SetThoiGianTatChuong()
{
  if(ChoBatChuong)
  {  
    if((millis()/1000 - ThoiDiemBatChuong) > TGBatChuong) 
    { 
      TatChuong(); 
      ChoBatChuong = false; 
    } 
  }
}
////////////////////////////////  
void BatChuong(){ digitalWrite(Chuong,HIGH); }
////////////////////////////////  
void TatChuong(){ digitalWrite(Chuong,LOW); }
////////////////////////////////  
void ChopDen(){ digitalWrite(Led,HIGH); delay(200); digitalWrite(Led,LOW); }
////////////////////////////////  
void ManHinhCho()
{
    lcd.begin(16, 2);
    lcd.print("  BV QT EXSON");
    lcd.setCursor(0, 1);
    lcd.print(" CHAO QUI KHACH");
}
////////////////////////////////  
void ManHinhKhach()
{
    lcd.begin(16, 2);  
    lcd.print("Yeu cau: "); 
    lcd.print(ChuoiSoBenhNhan);
    lcd.setCursor(0,1); 
    lcd.print(ChuoiTenPhong);
}
