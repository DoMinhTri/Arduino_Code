// Date and time functions using RX8025 RTC connected via I2C and Wire lib
#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(10);


#include <Wire.h>
#include "Sodaq_DS3231.h"

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
DateTime dt(2016, 6, 9, 18, 51, 30, 5);

void setup () 
{
    Serial.begin(57600);
    Wire.begin();
    rtc.begin();
    rtc.setDateTime(dt);
    /////////
    lcd.begin(16, 2);
    lcd.print("Xin chao!");

    //////////
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
}

uint32_t old_ts;

void loop () 
{
  int s = millis()/1000;
  lcd.setCursor(0,1);
  lcd.print(s);
  //////////////
  BlinkPin(4, 500);  
  BlinkPin(5, 2000);  
  BlinkPin(6, 2000);  
  BlinkPin(7, 2000);  
  BlinkPin(8, 2000);           
  //////////////
  DateTime now = rtc.now(); //get the current date-time
  uint32_t ts = now.getEpoch();
  if (old_ts == 0 || old_ts != ts) 
  {
	      old_ts = ts;
	      Serial.print(now.year(), DEC);
	      Serial.print('/');
	      Serial.print(now.month(), DEC);
	      Serial.print('/');
	      Serial.print(now.date(), DEC);
	      Serial.print(' ');
	      Serial.print(now.hour(), DEC);
	      Serial.print(':');
	      Serial.print(now.minute(), DEC);
	      Serial.print(':');
	      Serial.print(now.second(), DEC);
	      Serial.print(' ');
	      Serial.print(weekDay[now.dayOfWeek()]);
	      Serial.println();
	      Serial.print("Seconds since Unix Epoch: "); 
	      Serial.print(ts, DEC);
	      Serial.println();
    }
    delay(1000);
}


void BlinkPin(uint8_t _pin, int _Timer)
{
    digitalWrite(_pin, HIGH); delay(_Timer);              
    digitalWrite(_pin, LOW);  delay(_Timer); 
}
