#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 13, 11, 7, 10, 9);
RTC_DS1307 RTC;
 
void setup () {
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    lcd.begin(20, 4);
    
   pinMode(8,OUTPUT);
 
  if (! RTC.isrunning()) {
   Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
   RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}
 
void loop () {
    DateTime now = RTC.now();
    lcd.setCursor(0, 0);
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
    lcd.print('-');
    //////////////////
    int dayofweek = now.dayOfWeek();
    switch(dayofweek)
    {
     case 1:
     lcd.print("Mon");
     break;
     case 2:
     lcd.print("Tues");
     break;
     case 3:
     lcd.print("Wed");
     break;
     case 4:
     lcd.print("Thur");
     break;
     case 5:
     lcd.print("Fri");
     break;
     case 6:
     lcd.print("Sat");
     break;
     case 0:
     lcd.print("Sun");
     break;
    delay(1000);
    }
    //////////////////
    lcd.setCursor(0, 1);
     if (now.hour()<10)
    lcd.print('0');
    lcd.print(now.hour(), DEC);
    lcd.print(':');
     if (now.minute()<10)
    lcd.print('0');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second()<10)
    lcd.print('0');
    lcd.print(now.second(), DEC);
    //////////////////
}
    
