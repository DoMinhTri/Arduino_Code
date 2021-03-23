

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

#define LedTH     0
#define LedSet    1
#define NutBam    2
#define Relay     3
#define CamBien   4

int ThoiGianDelay = 0;
int ThoiGianMax   = 30;
int intTrangThai  = 0, LucTruoc = 0, LucSau = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(Relay,  OUTPUT);          // Relay
  pinMode(LedTH,  OUTPUT);          // LedTH
  pinMode(LedSet, OUTPUT);          // LedSet
  
  pinMode(CamBien, INPUT);         // Cam bien
  pinMode(NutBam,  INPUT_PULLUP);  // Nut bam
  
  //ThoiGianMax   = EEPROM.read(1);
  //if(ThoiGianMax > 60) { ThoiGianMax = 30; }
  ThoiGianDelay = ThoiGianMax ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  intTrangThai = digitalRead(CamBien); // Cam bien
  //////////////////////////////////////////////////
  if(intTrangThai == 1) 
  {
      LucTruoc  = millis()/1000;  
      LucSau    = LucTruoc ;
      if(ThoiGianDelay < ThoiGianMax) ThoiGianDelay++; 
      digitalWrite(LedTH, HIGH);
      delay(100);
      digitalWrite(LedTH, LOW);
  }
  //////////////////////////////////////////////////
  if(ThoiGianDelay > 0)
  {
      digitalWrite(Relay, HIGH);  // Relay 
      if(intTrangThai == 0)
      {
          LucTruoc = millis()/1000;    
          if(( LucTruoc  - LucSau) == 1) 
          {
            ThoiGianDelay--;
            LucSau = LucTruoc;
            digitalWrite(LedTH, HIGH);
            delay(100);
            digitalWrite(LedTH, LOW);
          }
      }
      /////////      
  }
  else{ digitalWrite(Relay, LOW);  }// Relay 
  ////////////////////
  intTrangThai = digitalRead(NutBam); // Nut bam
  if(intTrangThai == 0)
  {
      ThoiGianDelay = ThoiGianDelay + 10;
      if(ThoiGianDelay > 60) ThoiGianDelay = 10;
      ThoiGianMax = ThoiGianDelay;
      //EEPROM.write(1,ThoiGianDelay);
      for(intTrangThai=1; intTrangThai< ThoiGianDelay/10; intTrangThai++)
      {
          digitalWrite(LedSet, HIGH);
          delay(100);
          digitalWrite(LedSet, LOW);
      }
  }
  ////////////////////
}

