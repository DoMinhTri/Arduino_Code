////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
///////////////////////////////////////////
#include <Sodaq_DS3231.h>
#include <Wire.h>

uint32_t old_ts;

String weekDay[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
///////////////////////////////////////////

#define RELAY   4
void setup() 
{
   Wire.begin();
   pinMode(RELAY, OUTPUT);
   ////////////////
   Serial.begin(38400);
   lcd.begin(16, 2); 
   ////////////////
   rtc.begin();
}

//////////////////////////////////////////////////////////////////////////////
void ReadDS3231()
{
    DateTime now = rtc.now(); 
    uint32_t ts  = now.getEpoch();
    String dayofweek;
    int second,minute,hour,date,month,year,Temperature; 
    
    if (old_ts == 0 || old_ts != ts) 
    {
        old_ts = ts;
        second = now.second();
        minute = now.minute();
        hour   = now.hour();
        date   = now.date();
        month  = now.month();
        year   = now.year();
        dayofweek   = weekDay[now.dayOfWeek()];
        
        rtc.convertTemperature();
        Temperature = rtc.getTemperature();
    }
    
  lcd.setCursor(0,0);
  lcd.print(year,DEC);    lcd.print('/');
  lcd.print(month,DEC);   lcd.print('/');
  lcd.print(date,DEC);    lcd.print('[');
  lcd.print(dayofweek);   lcd.print(']');
  
  lcd.setCursor(0,1);
  lcd.print(hour,DEC);    lcd.print(':');
  lcd.print(minute,DEC);  lcd.print(':');
  lcd.print(second,DEC);  lcd.print('[');
  lcd.print(Temperature); lcd.print(']');
  delay(1000);
}
//////////////////////////////////////////////////////////////////////////////
String ChuoiTinNhan = "";
void loop() 
{
  ReadDS3231();
  BlinkRelay();
  /////////////
   while (Serial.available() > 0 ) 
  {
       char KyTu = Serial.read();
       if(KyTu != '\n' && KyTu != ' '){ ChuoiTinNhan+= KyTu; }
       delay(5);
   }
   if( ChuoiTinNhan != "") 
   { 
     SuLyGiaTriChuoiNhan(ChuoiTinNhan);
     ChuoiTinNhan = ""; 
   }
   /////////////
}
//////////////////////////////////////////////////////////////////////////////
void BlinkRelay()
{
  digitalWrite(RELAY, !digitalRead(RELAY));  
}

void SuLyGiaTriChuoiNhan(String ChuoiNhan)
{
  
}
void PhanTichChuoiGioVaCaiDat(String ChuoiGio)
{
     
     DateTime dt(2015, 12, 28, 0, 7, 30, 1);
     rtc.setDateTime(dt);
}
//////////////////////////////////////////////////////////////////////////////
void ChinhGio()
{
    int Nam, Thang,  Ngay,  Gio,  Phut,  Giay, Thu;
    DateTime dt(Nam, Thang, Ngay, Gio, Phut, Giay, Thu);
    rtc.setDateTime(dt);
    //Clock.setSecond(Giay);    // Set the second 
    //Clock.setMinute(Phut);    // Set the minute 
    //Clock.setHour(Gio);       // Set the hour 
    //Clock.setDoW(5);          // Set the day of the week
    //Clock.setDate(Ngay);      // Set the date of the month
    //Clock.setMonth(Thang);    // Set the month of the year
    // Lấy 2 chữ số cuối vd:15 = 2015
    //Clock.setYear(Nam);       //Set the year (Last two digits of the year)
}
