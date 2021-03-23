#include <SPI.h>
#include "e-ink-1.54.h"
#include "e-ink-display.h"
#include "images.c"
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1

unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;

/////////////////////////////////////////////////////////////////////////////////

#include <STM32RTC.h>
STM32RTC& rtc = STM32RTC::getInstance();

const byte seconds = 0;
const byte minutes = 41;
const byte hours   = 18;
const byte weekDay = 1;
const byte day     = 21;
const byte month   = 2;
const byte year    = 19;

void setup()
{
  Serial.begin(9600);
  rtc.begin(); // initialize RTC 24H format

  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);
  //rtc.setWeekDay(weekDay);
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

  // you can use also
  //rtc.setTime(hours, minutes, seconds);
  //rtc.setDate(weekDay, day, month, year);

  //////////////////////////////////////////////////////////////
  if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  //epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  //epd.DisplayFrame();
  //epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  //epd.DisplayFrame();
  //////////////////
}

void loop()
{
  // Print date...
  print2digits(rtc.getDay());
  Serial.print("/");
  print2digits(rtc.getMonth());
  Serial.print("/");
  print2digits(rtc.getYear());
  Serial.print(" ");

  // ...and time
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());

  String mDate = String(rtc.getYear()) + "/" +  String(rtc.getMonth()) + "/" + String(rtc.getDay()) + " " + String(rtc.getHours()) + ":" + String(rtc.getMinutes()) + ":" + String(rtc.getSeconds());
  /////////////////////////////////////////////////////////////////
  epd.SetFrameMemory(gImage_images);
  /////////////
  paint.Clear(COLORED);       
  paint.SetWidth(30);           
  paint.SetHeight(200);
  paint.SetRotate(ROTATE_270);    
  paint.DrawStringAt(20,8, String2Char(mDate), &Font16, UNCOLORED);

  paint.DrawCircle(20,20,360,COLORED);
  epd.SetFrameMemory(paint.GetImage(), 160, 10, paint.GetWidth(), paint.GetHeight());    
  /////////////////////////////////////////////////////////////////
  epd.DisplayFrame();
  delay(1000);
}



void print2digits(int number) {
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}

char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
