#include <Sodaq_DS3231.h>




#include <Wire.h>

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
///////////////////////////////////////////
byte year, month, date, DoW, hour, minute, second;

void setup() 
{
   Wire.begin();
   Serial.begin(9600);
}
void ReadDS3231()
{
  int second,minute,hour,date,month,year,temperature; 
  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour   = Clock.getHour(h12, PM);
  date   = Clock.getDate();
  month  = Clock.getMonth(Century);
  year   = Clock.getYear();
  
  temperature=Clock.getTemperature();
  
  Serial.print("20");
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
  Serial.print('\n');
  Serial.print("Temperature=");
  Serial.print(temperature); 
  Serial.print('\n');
}
void loop() 
{
  ReadDS3231();
  delay(1000);
}

void ChinhGio(int Nam, int Thang, int Ngay, int Gio, int Phut, int Giay)
{
    Clock.setSecond(Giay);    // Set the second 
    Clock.setMinute(Phut);    // Set the minute 
    Clock.setHour(Gio);       // Set the hour 
    Clock.setDoW(5);          // Set the day of the week
    Clock.setDate(Ngay);      // Set the date of the month
    Clock.setMonth(Thang);    // Set the month of the year
    // 2 Chũ số cuối vd:15 = 2015
    Clock.setYear(Nam);       //Set the year (Last two digits of the year)
}
