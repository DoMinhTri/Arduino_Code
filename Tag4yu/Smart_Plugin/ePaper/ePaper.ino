////////////////////////////
//  VCC : 3.3V
//  GND : Ground
//  CLK : D13 - Clock simulation
//  CS  : D10 - chip select signal
//  SDI : D11 - data input, similar to MOSI
//  D/C : D9  - command / data switch


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

void setup() {

  Serial.begin(9600);

  if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();

    if (epd.Init(lut_full_update) != 0) {
      Serial.print("e-Paper init failed");
      return;
  }


/*
  paint.SetWidth(30);
  paint.SetHeight(196);
  paint.SetRotate(ROTATE_270);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(40, 10, "Hello world!", &Font16, COLORED);
  epd.SetFrameMemory(paint.GetImage(),70, 5, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  delay(3000);
*/


  //epd.SetFrameMemory(gImage_images);
  //epd.DisplayFrame();
  //epd.SetFrameMemory(gImage_images);
  //epd.DisplayFrame();
  

  /*
  epd.SetFrameMemory(IMAGE_DATA2);
  epd.DisplayFrame();
  epd.SetFrameMemory(IMAGE_DATA2);
  epd.DisplayFrame();
  delay(2000);

  
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
  delay(2000);

  epd.SetFrameMemory(IMAGE_DATA1);
  epd.DisplayFrame();
  epd.SetFrameMemory(IMAGE_DATA1);
  epd.DisplayFrame();
  delay(2000);
  */



}

void loop()
{

  epd.SetFrameMemory(gImage_images);
  //epd.DisplayFrame();
  //paint.SetRotate(ROTATE_180); /*旋转文字的方向*/
 // delay(1000);
  /////////////
  paint.SetWidth(30);           /*设置区域的大小*/
  paint.SetHeight(200);
  paint.SetRotate(ROTATE_270);   /*旋转文字的方向*/
  paint.Clear(COLORED);          /*将区域显示为黑色*/
  //paint.DrawStringAt(20,8, "Smart Plugin", &Font20, UNCOLORED);
  paint.DrawStringAt(20,8, "Timer :", &Font20, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 160, 10, paint.GetWidth(), paint.GetHeight());     /*设置区域的位置*/

  //paint.SetWidth(160);           /*设置区域的大小*/
  //paint.SetHeight(200);
  //paint.SetRotate(ROTATE_180);   /*旋转文字的方向*/
 // paint.Clear(COLORED);          /*将区域显示为黑色*/
  //paint.DrawStringAt(20,8, "Timer :", &Font20, UNCOLORED);
  //epd.SetFrameMemory(paint.GetImage(), 160, 10, paint.GetWidth(), paint.GetHeight());     /*设置区域的位置*/
  
  
  epd.DisplayFrame();
  delay(1000);


}
