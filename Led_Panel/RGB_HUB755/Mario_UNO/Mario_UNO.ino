// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);
//RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64); // 32x64

const unsigned short CongChua[512]={
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD384, 0xFFDF, 0xD384, 0xFFDF, 0xD384, 0xF646, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0010 (16) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD384, 0xF646, 0xFFDF, 0xD384, 0xFFDF, 0xD384, 0xF646, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0020 (32) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x9281, 0x9281, 0xD384, 0xF646, 0xFFDF, 0xFFDF, 0xF646, 0xCC41, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0030 (48) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0x9281, 0x9281, 0xCC41, 0xCC41, 0xCC41, 0x9281, 0x9281, 0x9281, 0xCC41, 0xF646, 0x9281, 0x9281, 0xFFFF,   // 0x0040 (64) pixels
0xFFFF, 0xFFFF, 0x9281, 0xCC41, 0xF646, 0xF646, 0xF646, 0xF646, 0xF646, 0xCC41, 0xCC41, 0xF646, 0xF646, 0xF646, 0xCC41, 0x9281,   // 0x0050 (80) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0x9281, 0xCC41, 0xF646, 0xCC41, 0xF646, 0xCC41, 0x9281, 0x9281, 0xCC41, 0x9281, 0xCC41, 0x9281, 0xFFFF,   // 0x0060 (96) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0x9281, 0xCC41, 0xCC41, 0x9281, 0xCC41, 0x9281, 0xE30B, 0xE30B, 0x9281, 0xE30B, 0x89C0, 0xFFFF, 0xFFFF,   // 0x0070 (112) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0x9281, 0xCC41, 0x9281, 0xFD4E, 0x9281, 0xE30B, 0xFD4E, 0x18C3, 0xFD4E, 0x18C3, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0080 (128) pixels
0xFFFF, 0xFFFF, 0x9281, 0xCC41, 0xCC41, 0x9281, 0xFD4E, 0x9281, 0xE30B, 0xFD4E, 0x89C0, 0xFD4E, 0x89C0, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0090 (144) pixels
0xFFFF, 0x9281, 0xCC41, 0xF646, 0xCC41, 0x9281, 0xE30B, 0xFD4E, 0xFD4E, 0xFD4E, 0xFD4E, 0xFD4E, 0xFD4E, 0x89C0, 0xFFFF, 0xFFFF,   // 0x00A0 (160) pixels
0x9281, 0xCC41, 0xF646, 0xF646, 0xCC41, 0xCC41, 0x9281, 0xE30B, 0xE30B, 0xFD4E, 0xFD4E, 0xFD4E, 0xE30B, 0x89C0, 0xFFFF, 0xFFFF,   // 0x00B0 (176) pixels
0x9281, 0xCC41, 0xF646, 0xCC41, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0x89C0, 0xE30B, 0xE30B, 0xE30B, 0x89C0, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x00C0 (192) pixels
0x9281, 0xCC41, 0xCC41, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xB10A, 0x89C0, 0x89C0, 0x89C0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x00D0 (208) pixels
0x9281, 0xCC41, 0xB10A, 0xFA92, 0xFD1B, 0xFD1B, 0xFD1B, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x00E0 (224) pixels
0xFFFF, 0x9281, 0xB10A, 0xFA92, 0xFD1B, 0xFD1B, 0xFD1B, 0xFA92, 0xFA92, 0xFD1B, 0xFD1B, 0xFA92, 0xB10A, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x00F0 (240) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFA92, 0xFD1B, 0xFD1B, 0xFD1B, 0xB10A, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0100 (256) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xB10A, 0xE30B, 0xE30B, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xB10A, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0110 (272) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x89C0, 0xFD4E, 0xFD4E, 0x89C0, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0120 (288) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x89C0, 0xF646, 0xFFDF, 0xFFDF, 0x89C0, 0xFFDF, 0xFA92, 0xB10A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0130 (304) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xB10A, 0x89C0, 0xF646, 0xFFDF, 0xF646, 0x89C0, 0xB10A, 0xB10A, 0xB10A, 0xFFFF, 0xFFFF, 0xFFFF,   // 0x0140 (320) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xB10A, 0xFA92, 0x89C0, 0x89C0, 0xF646, 0xFFDF, 0x89C0, 0xFD1B, 0xFA92, 0xB10A, 0xFFFF, 0xFFFF,   // 0x0150 (336) pixels
0xFFFF, 0xFFFF, 0xFFFF, 0xB10A, 0xFA92, 0xFD1B, 0xFA92, 0x89C0, 0xCC41, 0xFFDF, 0x89C0, 0xFD1B, 0xFA92, 0xB10A, 0xFFFF, 0xFFFF,   // 0x0160 (352) pixels
0xFFFF, 0xFFFF, 0xB10A, 0xFA92, 0xFD1B, 0xFD1B, 0xFD1B, 0xFA92, 0x89C0, 0xF646, 0x89C0, 0xFA92, 0xFD1B, 0xFA92, 0xB10A, 0xFFFF,   // 0x0170 (368) pixels
0xFFFF, 0xFFFF, 0xB10A, 0xFD1B, 0xFD1B, 0xFA92, 0xFD1B, 0xFD1B, 0xFA92, 0x89C0, 0x89C0, 0xFA92, 0xFD1B, 0xFA92, 0xB10A, 0xFFFF,   // 0x0180 (384) pixels
0xFFFF, 0xB10A, 0xFA92, 0xFD1B, 0xFD1B, 0xFA92, 0xFD1B, 0xFD1B, 0xFA92, 0xFA92, 0xFA92, 0xFA92, 0xFD1B, 0xFD1B, 0xFA92, 0xB10A,   // 0x0190 (400) pixels
0xFFFF, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFD1B, 0xFD1B, 0xFA92, 0xFD1B, 0xFA92, 0xFA92, 0xB10A, 0xFD1B, 0xFA92, 0xB10A,   // 0x01A0 (416) pixels
0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFD1B, 0xFD1B, 0xFA92, 0xB10A, 0xFA92, 0xFA92, 0xB10A, 0xFD1B, 0xFA92, 0xB10A,   // 0x01B0 (432) pixels
0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFA92, 0xFD1B, 0xFD1B, 0xFA92, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFA92, 0xB10A,   // 0x01C0 (448) pixels
0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xFA92, 0xB10A,   // 0x01D0 (464) pixels
0xB10A, 0xB10A, 0xFA92, 0xFA92, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xB10A, 0xB10A,   // 0x01E0 (480) pixels
0xFFFF, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xB10A, 0xFA92, 0xFA92, 0xFA92, 0xB10A, 0xB10A, 0xB10A,   // 0x01F0 (496) pixels
0xFFFF, 0xFFFF, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xB10A, 0xFFFF, 0xFFFF,   // 0x0200 (512) pixels
};

void setup() {

  matrix.begin();

  //drawImage();
  VeCongChua();
  //matrix.drawPixel(5 , 10, CongChua[1]);
  delay(500);

}

void drawImage()
{
  
  int imageWidth  = 16;
  int imageHeight = 32;
  
  int counter     = 0;
  for (int yy = 0; yy < imageHeight; yy++)
  {
    for (int xx = 0; xx < imageWidth; xx++)
    {
      matrix.drawPixel(xx , yy , CongChua[counter]);
      counter++;
    }
  }
}


void VeCongChua()
{
  
 
  int counter     = 0;
    for (int xx = 0; xx < 16; xx++)
    {
      matrix.drawPixel(xx , 0 , CongChua[xx]);
      counter++;
    }
}


void loop() {

}