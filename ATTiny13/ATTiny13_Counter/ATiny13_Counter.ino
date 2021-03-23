//////////////////////////////////////////////////////////////////////////
//                                 MCU ---- ATTINY 13 
//  clockPin : SRCLK_Pin : SH_CP    11        4  
//  latchPin : RCLK_Pin  : ST_CP    12        2
//  dataPin  : SER_Pin   : DS       14        1 
//
//////////////////////////////////////////////////////////////////////////
//int latchPin = 2 // chân ST_CP của 74HC595
//int clockPin = 2 // chân SH_CP của 74HC595
//int dataPin  = 1 // Chân DS    của 74HC595

const int Seg[10] = 
{
  0b01000000,//0
  0b01111001,//1
  0b10100100,//2
  0b10110000,//3
  0b00011001,//4
  0b00010010,//5
  0b10000011,//6
  0b11111000,//7
  0b00000000,//8
  0b00010000,//9
};

#include <EEPROM.h>
int So = 0, Timer = 0; 

void HienThi(int nPin) 
{
  digitalWrite(4, LOW);                // chân ST_CP của 74HC595
  shiftOut(1, 3, MSBFIRST, Seg[nPin]); // Chân ( DS - SH_CP )của 74HC595
  digitalWrite(4, HIGH);               // chân ST_CP của 74HC595
}

/////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(2, INPUT);  // Button
  
  pinMode(0, OUTPUT); // Relay
   
  pinMode(1, OUTPUT); // Chân DS    của 74HC595
  pinMode(4, OUTPUT); // chân ST_CP của 74HC595
  pinMode(3, OUTPUT); // chân SH_CP của 74HC595
  
  So = EEPROM.read(0);
  Timer = So*10*60;
  BatRelay();
}


void loop() 
{ 
  if(digitalRead(2) ==  HIGH) // Button click
  { 
     So++;
     So=So%10; 
     EEPROM.write(0, So);
     Timer = So*10*60;
   }
   
   if(Timer > 0)
   {
     Timer--; 
     digitalWrite(0, HIGH); // Relay on
     delay(1000);
   }
   else
   {
      digitalWrite(0, LOW); // Relay off
   }
   
   HienThi(Timer/600);
}

void BatRelay()
{

  digitalWrite(0, HIGH);
  delay(5000);          
  digitalWrite(0, LOW); 
  delay(5000);         
}



