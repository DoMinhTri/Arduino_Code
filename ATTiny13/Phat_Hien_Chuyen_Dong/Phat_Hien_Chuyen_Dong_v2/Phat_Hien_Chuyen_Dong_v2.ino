

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//   Port  --- Tiny13 Pin   ---  Arduino UNO ISP
//     0       5            -->   13
//     1       6            -->   12
//     2       7            -->   11
//     3       2            -->   
//     4       3            -->   
//             1 ( Reset)   -->   10    ---|
//             4 ( GND )    -->   GND   ---|--> ( >= 10uF )
//             8 ( VCC )    -->   VCC
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// Nguyên lý hoạt động :
// khi phát hiện có sự chuyển trạng thái thì tăng giá trị thời gian
// mỗi giá trị được tính là 1 giây.

#include <EEPROM.h>

#define Relay   13
#define NutBam  1
#define CamBien 7

int ThoiGianDelay = 0;
int ThoiGianMax   = 30;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(Relay, OUTPUT);          // Relay
  digitalWrite(Relay, HIGH);       // Relay
  
  pinMode(NutBam,  INPUT_PULLUP);  // Nut bam
  pinMode(CamBien, INPUT);         // Cam bien
  
  ThoiGianMax   = EEPROM.read(0);
  ThoiGianDelay = ThoiGianMax ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  NhanNutChinhThoiGian();                          
  TinhThoiGianThuc();
  delay(50);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
boolean HoatDong = false;
int TTCamBien = 0, LucTruoc = 0, LucSau = 0;

void TinhThoiGianThuc()
{
  TTCamBien = digitalRead(CamBien); // Cam bien
  //////////////////////////////////////////////////
  if(TTCamBien == 1) 
  {
      HoatDong  = false;
      LucTruoc  = millis()/1000;  
      LucSau    = LucTruoc ;
      if(ThoiGianDelay < ThoiGianMax) ThoiGianDelay++; 
      delay(50);
  }
  else{ HoatDong  = true; }
  //////////////////////////////////////////////////
  if(ThoiGianDelay > 0)
  {
      digitalWrite(Relay, HIGH);  // Relay 
      if(HoatDong)
      {
          LucTruoc = millis()/1000;    
          if((LucTruoc - LucSau) == 1) 
          {
            ThoiGianDelay--;
            LucSau = LucTruoc;
          }
      }
      /////////      
  }
  else{ digitalWrite(Relay, LOW);  }// Relay 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int BoTayRa = 0, TGDaBam = 0, TGLucDau = 0, BamNut = 0;

void NhanNutChinhThoiGian()
{
  BamNut = digitalRead(NutBam); // Nut bam
  if(BamNut == 0)
  {
      ThoiGianDelay = ThoiGianDelay + 30;
      if(ThoiGianDelay > 180) ThoiGianDelay = 180;
      LuuEEPROM();
      delay(200);
  }
  //////////
  if(BamNut != BoTayRa)
  {
    if(BamNut == 0) 
    {
        TGLucDau = millis()/1000;  
        TGDaBam  = TGLucDau;
    }
    else
    {
        TGDaBam  = (millis()/1000) - TGLucDau; 
        if(TGDaBam >= 2)  
        {
            ThoiGianDelay = 30;
            LuuEEPROM();
        }
    }  
  }
  //////////  
  BoTayRa = BamNut;
  //////////  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LuuEEPROM()
{  
    ThoiGianMax = ThoiGianDelay;
    EEPROM.write(0,ThoiGianDelay); 
}
