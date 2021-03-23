/// -- LCD 5110 -- //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//      http://www.arduino-projekte.de/index.php?n=23
//      SCK  - PIN_SCLK  - Pin 2
//      MOSI - PIN_SDIN  - Pin 8
//      DC   - PIN_DC    - Pin 7
//      RST  - PIN_RESET - Pin 3
//      CS   - PIN_SCE   - Pin 6
/////////////////////////////////////////////////////////////////////////////
#include <LCD5110_Graph.h>
LCD5110 myGLCD(2,8,7,3,6);
extern uint8_t SmallFont[];
extern unsigned char TinyFont[];
extern uint8_t MediumNumbers[];
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/********************************************************
**  Download from:                                     **
**  http://www.arduino-projekte.de                     **
**                                                     **
**  Based on Code from:				       **
**  http://arduino.cc/playground/                      **
**                                                     **
**  Released into the public domain.                   **
********************************************************/

#include <AH_24Cxx.h>
#include <Wire.h> 

#define AT24C01  0
#define AT24C02  1
#define AT24C04  2
#define AT24C08  3
#define AT24C16  4
#define AT24C32  5
#define AT24C64  6
#define AT24C128 8
#define AT24C256 9
//Initialisation

#define BUSADDRESS  0x00      
#define EEPROMSIZE  4096     //AT24C32 4096byte

AH_24Cxx ic_eeprom = AH_24Cxx(AT24C32, BUSADDRESS);

void setup(){

  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  ////////////////////////////
  Wire.begin();
  //Serial.println("Write data");
  myGLCD.clrScr();
  myGLCD.print("Write data", LEFT, 8);
  myGLCD.update();
  for(int i = 0; i < 100; i++) 
  {  
      ic_eeprom.write_byte(i,99);  delay(100); 
    myGLCD.printNumI(i, CENTER, 24, 1);
    myGLCD.update();
  }
}

void loop(){
  //Serial.println("Read data");
  myGLCD.clrScr();
  myGLCD.print("Read data", LEFT, 8);
  myGLCD.update();
  for (int i=0;i<100;i++)
  {
    //Serial.print("pos. ");  Serial.print(i);  Serial.print(": ");   Serial.println(ic_eeprom.read_byte(i));
    
    myGLCD.print("pos. ", LEFT, 16);
    myGLCD.printNumI(i, LEFT, 24, 4);
    myGLCD.print(": ", LEFT, 32);
    myGLCD.printNumI(ic_eeprom.read_byte(i), CENTER, 40);
    myGLCD.update();
  
    delay(200);
  }
}

