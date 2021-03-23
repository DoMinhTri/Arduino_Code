#include <Wire.h>
#include "Sodaq_DS3231.h"
char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
DateTime dt(2016, 4, 24, 15, 58, 15, 0);
///////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);


void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2); 
    ////////////
    
    //rtc.setDateTime(dt);
 }

int NgayTrongTuan(int d, int m, int y){
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

void loop() {

    ReadDS3231();
    delay(1000);
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

  rtc.convertTemperature();
  //temperature = (int)Clock.getTemperature();
  
  lcd.setCursor(0,0);
  lcd.print(year,DEC);    lcd.print('/');
  lcd.print(month,DEC);   lcd.print('/');
  lcd.print(date,DEC);    lcd.print('[');
  lcd.print(rtc.getTemperature()); lcd.print(']');
  
  lcd.setCursor(0,1);
  lcd.print(hour,DEC);    lcd.print(':');
  lcd.print(minute,DEC);  lcd.print(':');
  lcd.print(second,DEC);  
  lcd.print("-"); 
  lcd.print(weekDay[Clock.dayOfWeek()]);
  lcd.print("-"); 
  lcd.print(NgayTrongTuan(date, month, year));
}
