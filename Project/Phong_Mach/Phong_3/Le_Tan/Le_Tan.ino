
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9,10);
/////////////////////////////////////////////////
const uint64_t pipes[2] = { 0xDEDEDEDEE7LL, 0xDEDEDEDEE9LL };
#define MaxChar 7
static int ThuTu = 0;
boolean GuiDi = false; 
char MangGui[MaxChar]  = "";
char MangLoad[MaxChar] = "";
char MangTam[MaxChar]  = "";
/////////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 6, 8, 7, 4, 3);
int MangTinHieu[6] = {0,0,0,0,0,0};
//////////////////////////////////////
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
DateTime ThoiGian;

int KieuChinh = 1;
int ngay, thang, nam, gio, phut, giay;
String MangThang[12]   = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
int NgayTrongThang[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/////////////////////////////////////////////////
#define NutChon A3
#define NutOk   A2
#define LedLCD  A0
#define Chuong  2
char Tang   = '1';
char Phong  = '0';
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void setup(void) 
{
    //Serial.begin(9600);
    ////////////////////  IN - OUT
    pinMode(NutChon, INPUT_PULLUP);
    pinMode(NutOk,   INPUT_PULLUP);
    pinMode(Chuong,  OUTPUT);
    pinMode(LedLCD,  OUTPUT);
    //////////////////////////    RTC
    Wire.begin();
    RTC.begin();

    if (! RTC.isrunning()) 
    {
      //Serial.println("RTC is NOT running!");
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    //////////////////////////    nRF 2.4
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(70);
  
    radio.enableDynamicPayloads();
    radio.setRetries(15,15);
    radio.setCRCLength(RF24_CRC_16);

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);  
  
    radio.startListening();
    radio.printDetails();

    delay(200);
    //serialEvent() ;
    //////////////////////////   
    lcd.begin(16, 2);
    LayThoiGian();
}
/////////////////////////////////////////////////////////////////////////////////////////
char KyTu ;
char Menu = 'B'; // mặt bàn
String Chuoi_1, Chuoi_2;

void loop(void) 
{
    KyTu = SuLyNutBam(); 
    ThoiGian = RTC.now();
    NhanTinHieu();
    ////////////////////////  
    switch (Menu) 
    {
        /////////////////////////////////
        case 'B': // 
              //GuiTinHieu();
              //NhanTinHieu();
              MenuChinh(KyTu);
        break;
        /////////////////////////////////
        case 'G': 
              ChinhGio(KyTu);
        break;         
        /////////////////////////////////
    }
    //////////////////

}
/////////////////////////////////////////////////////////////////////////////////////////
void MenuChinh(char BNut)
{
  if(BNut == 'S') { Menu = 'G'; } else {  }
}
/////////////////////////////////////////////////////////////////////////////////////////
char SuLyNutBam()
{
    int DaBamNut = 1;
    DaBamNut = digitalRead(NutChon); if(DaBamNut == 0){ delay(200); return 'S'; }
    DaBamNut = digitalRead(NutOk);   if(DaBamNut == 0){ delay(200); return 'K'; }
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void SuLyTinHieu(String Chuoi)
{

}
/////////////////////////////////////////////////////////////////////////////////////////
void KiemTraTinHieuNhan()
{
  int TangNhan   = (int)MangLoad[5];
  int PhongNhan  = (int)MangLoad[6];
  ///////////
  if( TangNhan == (int)Tang)
  {
      if( PhongNhan == (int)Phong)
      { 
          ChuongReo();  
          lcd.print("Ok");
      }
  }
  ///////////
}
/////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////    nRF 2.4       ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////  
void serialEvent() 
{
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();
      MangTam[ThuTu] = KyTu;
      ThuTu++;
      if (ThuTu > MaxChar) { GuiDi = true; break; }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void SendGiaTri(String Chuoi)
{
  Chuoi.toCharArray(MangTam, MaxChar+2);
  GuiDi = true;
  ThuTu = 8;
}
/////////////////////////////////////////////////////////////////////////////////////////
void NhanTinHieu(void)
{
    int DoDai = 0;
    if ( radio.available() ) 
    {
        boolean KetThuc = false;
        while ( !KetThuc ) 
        {
          DoDai = radio.getDynamicPayloadSize();
          KetThuc = radio.read(&MangLoad,DoDai);
          delay(5);
        }
        MangLoad[DoDai] = 0; // null terminate string
        ///////////////////
        //Serial.print("R:");  Serial.print(MangLoad);  Serial.println();
        KiemTraTinHieuNhan();
        MangLoad[0] = 0;  // Clear the buffers
    }  
    //else { DongHo(); }
}
/////////////////////////////////////////////////////////////////////////////////////////
void GuiTinHieu(void)
{
  if (GuiDi) 
  { 
      strcat(MangGui,MangTam);      
      // swap TX & Rx addr for writing
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(0,pipes[0]);  
      radio.stopListening();
      boolean ok = radio.write(&MangGui,strlen(MangGui));
      ThuTu = 0;
      GuiDi = false;
      // restore TX & Rx addr for reading       
      radio.openWritingPipe(pipes[0]);
      radio.openReadingPipe(1,pipes[1]); 
      radio.startListening();  
      ///////////////////////
      //Serial.print("S:");  Serial.print(MangGui);  Serial.println();
      MangGui[0] = 0;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void ChuongReo()
{
    digitalWrite(LedLCD, HIGH);
    digitalWrite(Chuong, HIGH);
    delay(1500);
    digitalWrite(Chuong, LOW);
}
/////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////    RTC     /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////  
void ChinhGio(char BNut)
{
    int nMax = NgayTrongThang[thang - 1];    
    if(BNut == 'S') KieuChinh++; 
    //NhapNhay(5,2);
    
    switch (KieuChinh) 
    {
        case 1:
            if(BNut == 'K') { ngay++; if(ngay > nMax) ngay = 1; }
        break;
        /////////////////////////////////
        case 2:
            if(BNut == 'K') { thang++; if(thang > 12) thang = 1; }
        break;
        /////////////////////////////////
        case 3:
            if(BNut == 'K') { nam++; if(nam > 2060) nam = 2015; }
        break;
        /////////////////////////////////
        case 4:
            if(BNut == 'K') { gio++; if(gio > 23) gio = 1; }
        break;
        /////////////////////////////////
        case 5:
            if(BNut == 'K') { phut++; if(phut > 59) phut = 1; }
        break;
        /////////////////////////////////
        case 6:
            if(BNut == 'K') { giay++; if(giay > 59) giay = 1; }
        break;
        /////////////////////////////////
        case 7:
            LayChuoiThoiGian();
            RTC.adjust(DateTime(String2Char(Chuoi_1), String2Char(Chuoi_1)));
            Menu = 'B';
            KieuChinh = 0;
        break;        
        /////////////////////////////////        
    }
    LayChuoiThoiGian();
    lcd.setCursor(15, 1);
    lcd.print(KieuChinh);
    // RTC.adjust(DateTime("FEB 27 2015","17:13:45"));
}
////////////////////////////////////////////////////////////////////////////////////// 
void LayThoiGian()
{
    nam   = ThoiGian.year();
    thang = ThoiGian.month();
    ngay  = ThoiGian.day();
    gio   = ThoiGian.hour();
    phut  = ThoiGian.minute();
    giay  = ThoiGian.second();
}
////////////////////////////////////////////////////////////////////////////////////// 
void LayChuoiThoiGian()
{
    Chuoi_1 = (String)ngay + ":" + (String)thang + ":" + (String)nam ;
    Chuoi_2 = (String)gio  + ":" + (String)phut  + ":" + (String)giay ;
    LCD2Dong(Chuoi_1, Chuoi_2);
}
////////////////////////////////////////////////////////////////////////////////////// 
void DongHo()
{
    LayThoiGian();
    LayChuoiThoiGian();
 }
/////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////    LCD     /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////   
void XoaDong_1(){ lcd.setCursor(0, 2); lcd.print("                "); }
//////////////////////////////////////////////////////////////////////////////////////  
void XoaDong_2(){ lcd.setCursor(0, 1); lcd.print("                "); }
//////////////////////////////////////////////////////////////////////////////////////  
void LCD2Dong(String Dong_1, String Dong_2)
{
   //lcd.clear();
   lcd.begin(16, 2);
   lcd.print(Dong_1);
   lcd.setCursor(0, 1);
   lcd.print(Dong_2);
}
//////////////////////////////////////////////////////////////////////////////////////  
void NhapNhay(int ViTri, int Dong)
{
    lcd.setCursor(ViTri - 1, Dong - 1);
    lcd.cursor();
}

/////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
