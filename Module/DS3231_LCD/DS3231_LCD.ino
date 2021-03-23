////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
///////////////////////////////////////////
#include "Sodaq_DS3231.h"
#include <Wire.h>
DateTime dt(2011, 11, 10, 15, 18, 0, 5);
void setup() 
{
   Serial.begin(9600);
   lcd.begin(16, 2); 
   //////////////////
   Wire.begin();
   rtc.begin();
   //ChinhGio(2016,4,24,15,21,15);
   rtc.setDateTime(dt);
}
void ReadDS3231()
{
  DateTime Clock = rtc.now(); //get the current date-time
  int second,minute,hour,date,month,year,temperature; 
  second = Clock.second();
  minute = Clock.minute();
  hour   = Clock.hour();
  date   = Clock.date();
  month  = Clock.month();
  year   = Clock.year();
  
//  temperature=Clock.getTemperature();
  
  lcd.setCursor(0,0);
  //lcd.print(20); 
  lcd.print(year,DEC);    lcd.print('/');
  lcd.print(month,DEC);   lcd.print('/');
  lcd.print(date,DEC);    lcd.print('[');
 // lcd.print(temperature); lcd.print(']');
  
  lcd.setCursor(0,1);
  lcd.print(hour,DEC);    lcd.print(':');
  lcd.print(minute,DEC);  lcd.print(':');
  lcd.print(second,DEC);  

}
void loop() 
{
  ReadDS3231();
  delay(1000);
}

void ChinhGio(int Nam, int Thang, int Ngay, int Gio, int Phut, int Giay)
{
    DateTime dt(Nam, Thang, Ngay, Gio, Phut, Giay, 5);
    //DateTime dt(2011, 11, 10, 15, 18, 0, 5);
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
