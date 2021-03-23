
//////////////////////////////////////////////////////////////////////////
//                                 MCU ---- ATMEGA8 
//  clockPin : SRCLK_Pin : SH_CP    11        4  
//  latchPin : RCLK_Pin  : ST_CP    12        2
//  dataPin  : SER_Pin   : DS       14        1 
//
//////////////////////////////////////////////////////////////////////////
//int latchPin = 11 // chân ST_CP của 74HC595
//int clockPin = 12 // chân SH_CP của 74HC595
//int dataPin  = 10 // Chân DS    của 74HC595

int latchPin = 6;
int clockPin = 5;
int dataPin  = 7;

// digits from the right
byte colDig[4] = 
{
    B00001000, // digit 1
    B00000100, // digit 2
    B00000010, // digit 3
    B00000001, // digit 4
};

const byte digit[10] =      //seven segment digits in bits
{
    B11000000, // 0
    B11111001, // 1
    B10100100, // 2
    B10110000, // 3
    B10011001, // 4
    B10010010, // 5
    B10000010, // 6
    B11111000, // 7
    B10000000, // 8
    B10011000, // 9
};

void CapNhatSo(int col, int num)
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, colDig[col]);
    shiftOut(dataPin, clockPin, MSBFIRST, digit[num]);
    digitalWrite(latchPin, HIGH);
}

///////////////////////////////////////////////////////////////////////////
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
///////////////////////////////////////////////////////////////////////////
void setup()
{
    //Serial.begin(9600);
    //////////////////////////////     
    Wire.begin();
    RTC.begin();
    
   if (! RTC.isrunning()) 
   {
      RTC.adjust(DateTime(__DATE__, __TIME__));
   }
   ////////////////////////////// 
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

void loop()
{
    DateTime now = RTC.now();
    
    CapNhatSo(3, 5);
    CapNhatSo(2, 6);
    CapNhatSo(1, 7);
    CapNhatSo(0, 8);  
  
    //DateTime now = RTC.now();
    /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    */
}



