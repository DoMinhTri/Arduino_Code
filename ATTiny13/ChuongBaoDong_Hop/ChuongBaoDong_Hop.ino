

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


#define CamBien       4//4//0
#define Relay         0//3//1
#define LedTH         2//1//2
#define AutoReset     3//0//4


int ThoiGianReset   = 0;
int ThoiDiemSauCung = 0;
int ThoiGianDelay   = 0;
boolean TrangThai   = 0;
boolean VoRaNhanh   = 1;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(Relay,      OUTPUT);          // Relay
  pinMode(LedTH,      OUTPUT);          // LedTH
  pinMode(AutoReset,  OUTPUT);          // AutoReset
  pinMode(CamBien, INPUT);              // Cam bien
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  ThoiGianReset = millis()/1000; 
  TrangThai     = digitalRead(CamBien); // Cam bien
  //////////////////////////////////////////////////
  if(TrangThai == 1) 
  {
      ThoiDiemSauCung  = millis()/1000;  
      if(ThoiGianDelay <= 30) ThoiGianDelay++; 
      digitalWrite(Relay, HIGH);
      NhayLedTinHieu();
  }
  else
  {
    if(ThoiGianDelay > 0) 
    {
        if(VoRaNhanh == 1){ if(ThoiGianDelay <= 25) { ThoiGianDelay = 5; VoRaNhanh = 0; } }
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
        if(ThoiGianReset > 3600){ digitalWrite(AutoReset, HIGH); delay(1000); } // Reset bộ nhớ
    }
  }
  ////////////////////
}
////////////////////
void NhayLedTinHieu(){ digitalWrite(LedTH, HIGH); delay(100); digitalWrite(LedTH, LOW); }

