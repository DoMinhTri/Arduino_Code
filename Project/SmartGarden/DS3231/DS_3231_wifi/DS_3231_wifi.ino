#include <EEPROM.h>
////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature CamBienNhietDo(&oneWire);
////////////////////////////////////
const char* Thu_Vi[8]  = {  "CN" , "T2" , "T3"  , "T4" , "T5"  , "T6" , "T7"};
const char* Thu_En[8]  = {  "Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};
const char* Thang[13]  = {  "January", "February", "March", "April", "May", "June", "July", "August","September", "October", "November", "December"};
////////////////////////////////////
#include <DS3231.h>
#include <Wire.h>
DS3231 DongHoThoiGian;
bool Century=false;
bool h12;
bool PM;
////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Wire.begin();
  ///////////////////////////
        DongHoThoiGian.setSecond(50);//Set the second 
        DongHoThoiGian.setMinute(52);//Set the minute 
        DongHoThoiGian.setHour(5);  //Set the hour 
        DongHoThoiGian.setDoW(5);    //Set the day of the week
        DongHoThoiGian.setDate(28);  //Set the date of the month
        DongHoThoiGian.setMonth(3);  //Set the month of the year
        DongHoThoiGian.setYear(14);  //Set the year (Last two digits of the year)
  ///////////////////////////
  Serial.begin(9600);
}
///////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  int second,minute,hour,date,month,year,temperature; 
  second = DongHoThoiGian.getSecond();
  minute = DongHoThoiGian.getMinute();
  hour   = DongHoThoiGian.getHour(h12, PM);
  date   = DongHoThoiGian.getDate();
  month  = DongHoThoiGian.getMonth(Century);
  year   = DongHoThoiGian.getYear();
  
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  
  delay(1000);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
