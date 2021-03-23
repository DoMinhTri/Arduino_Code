
#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 6, 8, 7, 4, 3);
//////////////////////////////////////
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
String ChuoiThoiGian = "";
//////////////////////////////////////
#define NutChon A3
#define NutOk   A2
#define TinHieu A1
#define Chuong  2
//////////////////////////////////////
void setup() 
{
    Serial.begin(9600);
    ////////////////////  
    pinMode(NutChon, INPUT_PULLUP);
    pinMode(NutOk, INPUT_PULLUP);
    pinMode(Chuong, OUTPUT);
    pinMode(TinHieu, OUTPUT);
    ////////////////////
    Wire.begin();
    RTC.begin();

    if (! RTC.isrunning()) 
    {
      Serial.println("RTC is NOT running!");
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    //////////////////////////

}
/////////////////////////////////////////////////////////////////////////////////////  
char KyTu = 'b'; 
String Chuoi_1,Chuoi_2;
void loop() 
{
     NhapNhay(3,1);
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
