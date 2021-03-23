
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
int MangTinHieu[5] = {0,0,0,0,0};
//////////////////////////////////////
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
String ChuoiThoiGian = "";
/////////////////////////////////////////////////
#define NutChon A3
#define NutOk   A2
#define TinHieu A1
#define Chuong  2
/////////////////////////////////////////////////////////////////////////////////////////
void setup(void) 
{
    Serial.begin(9600);
    ////////////////////  IN - OUT
    pinMode(NutChon, INPUT_PULLUP);
    pinMode(NutOk,   INPUT_PULLUP);
    pinMode(Chuong,  OUTPUT);
    pinMode(TinHieu, OUTPUT);
    //////////////////////////    RTC
    Wire.begin();
    RTC.begin();

    if (! RTC.isrunning()) 
    {
      Serial.println("RTC is NOT running!");
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
    serialEvent() ;
    //////////////////////////    
}
/////////////////////////////////////////////////////////////////////////////////////////
char KyTu = 'b'; 
String Chuoi_1,Chuoi_2;

void loop(void) 
{
    NhanTinHieu();
    GuiTinHieu();
    
    ////////////////////////    
    int  BamCHON = digitalRead(NutChon);
    int  BamOK   = digitalRead(NutOk);
    ////////////////////////  
    switch (KyTu) 
    {
        case '1': 
        digitalWrite(Chuong, HIGH);
        RTC.adjust(DateTime("FEB 27 2015","17:13:45"));
        break;
        /////////////////////////////////      
        case '2': 
        digitalWrite(TinHieu, HIGH);
        RTC.adjust(DateTime("FEB 27 2015","18:13:45"));
        break;
        /////////////////////////////////                
        case 'b': // 
        if(BamCHON == LOW) KyTu = '1';
        if(BamOK   == LOW) KyTu = '2';
        digitalWrite(TinHieu, LOW);    
        digitalWrite(Chuong, LOW);
        DongHo();
        break;
    }
    //////////////////
}
/////////////////////////////////////////////////////////////////////////////////////////
void SuLyTinHieu(String Chuoi)
{
    //////////////////
    if(Chuoi == "Phong_1M") SendGiaTri("Phong_1K");
    if(Chuoi == "Phong_1N") SendGiaTri("Phong_1C");
    //////////////////
    if(Chuoi == "Phong_2M") SendGiaTri("Phong_2K");
    if(Chuoi == "Phong_2N") SendGiaTri("Phong_2C");
    //////////////////
    if(Chuoi == "Phong_3M") SendGiaTri("Phong_3K");
    if(Chuoi == "Phong_3N") SendGiaTri("Phong_3C");
    //////////////////
    if(Chuoi == "Phong_4M") SendGiaTri("Phong_4K");
    if(Chuoi == "Phong_4N") SendGiaTri("Phong_4C");
    //////////////////
    if(Chuoi == "Phong_5M") SendGiaTri("Phong_5K");
    if(Chuoi == "Phong_5N") SendGiaTri("Phong_5C");
    //////////////////
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
      SuLyTinHieu(MangLoad);
      Serial.print("R:");
      Serial.print(MangLoad);
      Serial.println();
      MangLoad[0] = 0;  // Clear the buffers
      NhayDen();
  }  
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
        Serial.print("S:");
        Serial.print(MangGui);          
        Serial.println();
        MangGui[0] = 0;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void NhayDen()
{
    digitalWrite(DenLed, LOW);
    delay(500);
    digitalWrite(DenLed, HIGH);
}
/////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////    RTC     /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////  
void DongHo()
{
    ////////////////////////  
    DateTime ThoiGian = RTC.now();
    int ngay  = ThoiGian.year();
    int thang = ThoiGian.month();
    int nam   = ThoiGian.day();
    int gio   = ThoiGian.hour();
    int phut  = ThoiGian.minute();
    int giay  = ThoiGian.second();
    
    Chuoi_1 = (String)nam + ":" + (String)thang + ":" + (String)ngay ;
    Chuoi_2 = (String)gio + ":" + (String)phut  + ":" + (String)giay ;
    
    LCD2Dong(Chuoi_1, Chuoi_2);
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
