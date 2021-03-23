#include <STM32RTC.h>
STM32RTC& rtc = STM32RTC::getInstance();

const byte seconds = 39;
const byte minutes = 32;
const byte hours   = 11;
const byte weekDay = 1;
const byte day     = 28;
const byte month   = 2;
const byte year    = 19;
///////////////////////////
#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <avr/pgmspace.h>
//////////////////////////
void setup()
{
  Serial.begin(9600);
  rtc.begin(); 
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(weekDay, day, month, year);
  ///////////////////
  Wire.begin();
  SeeedGrayOled.init(SH1107G);      // initialize SEEED OLED display
  SeeedGrayOled.clearDisplay();     // Clear Display.
  SeeedGrayOled.setNormalDisplay(); // Set Normal Display Mode
  SeeedGrayOled.setVerticalMode();  // Set to vertical mode for displaying text
  ///////////////////
}

void loop()
{
  // Print date...

  String ngay =  "20" + String(rtc.getYear())      + "/" + SoHangChuc(rtc.getMonth())   + "/" + SoHangChuc(rtc.getDay()) ;
  String gio  =         SoHangChuc(rtc.getHours()) + ":" + SoHangChuc(rtc.getMinutes()) + ":" + SoHangChuc(rtc.getSeconds());
  
  SeeedGrayOled.setGrayLevel(15); //Set Grayscale level. Any number between 0 - 15.
  
  SeeedGrayOled.setTextXY(2,0);  //set Cursor to ith line, 0th column
  SeeedGrayOled.putString(String2Char(ngay)); //Print Hello World

  SeeedGrayOled.setTextXY(4,0);  //set Cursor to ith line, 0th column
  SeeedGrayOled.putString(String2Char(gio)); //Print Hello World
  
  Serial.print(ngay + " " + gio);
  Serial.println();

  delay(1000);
}

///////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
/////////////////////////
String SoHangChuc(uint8_t So)
{
    if(So >9)
        return String(So); 
    else
        return ("0" + String(So));
}
///////////////////////
void print2digits(int number) {
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}
/////////////////////
