/*
  The new OLED module drive IC has two version,
  1. SH1107G
  2. SSD1327
  
  Change the paramater of the drive IC name to init the module.
  **
  ** SeeedGrayOled.init(SSD1327);  // SSD1327 or SH1107G
  **

  Copyright - Seeedstudio
  Author - lambor
  Date - 4/24/2017
*/

#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <avr/pgmspace.h>


void setup()
{
  Wire.begin();
  SeeedGrayOled.init(SH1107G);             //initialize SEEED OLED display
  SeeedGrayOled.clearDisplay();     //Clear Display.
  SeeedGrayOled.setNormalDisplay(); //Set Normal Display Mode
  SeeedGrayOled.setVerticalMode();  // Set to vertical mode for displaying text
  
  for(char i=0; i < 12 ; i++)
  {
  SeeedGrayOled.setTextXY(i,0);  //set Cursor to ith line, 0th column
  SeeedGrayOled.setGrayLevel(i); //Set Grayscale level. Any number between 0 - 15.
  SeeedGrayOled.putString("Hello World"); //Print Hello World
  }
}

void loop()
{
  
}


String SoHangChuc(uint8_t So)
{
    if(So >9)
        return String(So); 
    else
       return ("0" + String(So));
}