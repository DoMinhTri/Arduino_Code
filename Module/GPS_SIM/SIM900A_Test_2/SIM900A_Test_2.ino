

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

#include <LiquidCrystal.h>
LiquidCrystal lcd(A3, A2, A1, A0, 8, 12);

#define CamBien       5
#define Relay         7
#define LedTH         13
#define AutoReset     2
#define LCDLed       11

int ThoiGianReset   = 0;
int ThoiDiemSauCung = 0;
int ThoiGianDelay   = 0;
boolean TrangThai   = 0;
boolean VoRaNhanh   = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(Relay,      OUTPUT);          // Relay
  pinMode(LedTH,      OUTPUT);          // LedTH
  pinMode(AutoReset,  OUTPUT);          // AutoReset
  pinMode(LCDLed,     OUTPUT);
  
  pinMode(CamBien, INPUT);              // Cam bien
  
  lcd.begin(16, 2);
  digitalWrite(LCDLed, HIGH);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  ThoiGianReset = millis()/1000; 
  TrangThai     = digitalRead(CamBien); // Cam bien
  lcd.setCursor(0,1);   lcd.print("                    "); lcd.setCursor(0,1); lcd.print(ThoiGianDelay);   delay(300); 
  //////////////////////////////////////////////////
  if(TrangThai == 1) 
  {
      ThoiDiemSauCung  = millis()/1000;  
      if(ThoiGianDelay <= 30) ThoiGianDelay++; 
      NhayLedTinHieu();
      VoRaNhanh = 1;
  }
  else
  {
    if(ThoiGianDelay > 0) 
    {
        if(VoRaNhanh == 1){ if(ThoiGianDelay <= 25) ThoiGianDelay = 4; VoRaNhanh = 0; }
        if(( millis()/1000  - ThoiDiemSauCung) == 1) 
        {
              ThoiGianDelay--;
              ThoiDiemSauCung  = millis()/1000;  
              NhayLedTinHieu();
        }
    }
    else
    { 
        digitalWrite(Relay, LOW); 
        if(ThoiGianReset > 300){ digitalWrite(AutoReset, HIGH); delay(1000); } // Reset bộ nhớ
    }
  }
  ////////////////////
}
////////////////////
void NhayLedTinHieu(){ digitalWrite(LedTH, HIGH); delay(100); digitalWrite(LedTH, LOW); }

