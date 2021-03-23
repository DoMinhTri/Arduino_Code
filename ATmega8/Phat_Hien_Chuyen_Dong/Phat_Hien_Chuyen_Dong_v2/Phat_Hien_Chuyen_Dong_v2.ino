
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// Nguyên lý hoạt động :
// khi phát hiện có sự chuyển trạng thái thì tăng giá trị thời gian
// mỗi giá trị được tính là 1 giây.

#include <EEPROM.h>

#define Relay       13
#define NutBam      2
#define CamBien     11
#define Led         1


int ThoiGianDelay   = 0;
int ThoiGianMax     = 10;
const int intEEPROM = 1;
int BoTayRa = 0, TGDaBam = 0, TGLucDau = 0, BamNut = 0;

boolean HoatDong = false;
int TTCamBien = 0, LucTruoc = 0, LucSau = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(Relay, OUTPUT);          // Relay
  pinMode(CamBien, INPUT);         // Cam bien
  pinMode(NutBam,  INPUT_PULLUP);  // Nut bam
  
  if(EEPROM.read(intEEPROM) <= 60) { ThoiGianMax = EEPROM.read(intEEPROM) ; }
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
      Serial.print("TG Tang: "); Serial.println(ThoiGianDelay);
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
            Serial.print("TG giam: "); Serial.println(ThoiGianDelay);
          }
      }
      /////////      
  }
  else{ digitalWrite(Relay, LOW);  }// Relay 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void NhanNutChinhThoiGian()
{
  BamNut = digitalRead(NutBam); // Nut bam
  if(BamNut == 0)
  {
      ThoiGianDelay = ThoiGianDelay + 10;
      if(ThoiGianDelay > 60) ThoiGianDelay = 10;
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
    EEPROM.write(intEEPROM, ThoiGianDelay); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NhayDen()
{

}
