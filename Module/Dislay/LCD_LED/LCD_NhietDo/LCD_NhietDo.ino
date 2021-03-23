#include <OneWire.h>
#include <DallasTemperature.h>
#define Pin_NhietDo 2
OneWire DocNhietDo(Pin_NhietDo);
DallasTemperature CamBienNhietDo(&DocNhietDo);

///////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
///////////////////////////////////////

#include <Wire.h>
#include "Sodaq_DS3231.h"
String weekDay[8] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

///////////////////////////////////////
void setup() {
  Wire.begin();
  rtc.begin();
  lcd.begin(16, 2);
  ////////////////
  CamBienNhietDo.begin();
  
}

void loop() 
{
  CamBienNhietDo.requestTemperatures(); 
  int NhietDo = CamBienNhietDo.getTempCByIndex(0);
  ///////////////////////////////////////
  DateTime now = rtc.now();
  ///////////////////////////////////////
  lcd.setCursor(0, 1);
  lcd.print(NhietDo);
  lcd.setCursor(0, 0);
  lcd.print(weekDay[now.dayOfWeek()]);
}

