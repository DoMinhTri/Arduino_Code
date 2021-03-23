

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

#define LedTH     1
#define Relay     3
#define CamBien   4

int ThoiGianDelay = 0;
boolean TrangThai = 0;
int LucTruoc = 0, LucSau = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(Relay,  OUTPUT);          // Relay
  pinMode(LedTH,  OUTPUT);          // LedTH
  pinMode(0,  OUTPUT);          
  
  pinMode(CamBien, INPUT);          // Cam bien
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  TrangThai = digitalRead(CamBien); // Cam bien
  //////////////////////////////////////////////////
  if(TrangThai == 1) 
  {
      LucTruoc  = millis()/1000;  
      LucSau    = LucTruoc ;
      if(ThoiGianDelay <= 40) ThoiGianDelay++; 
      NhayLedTinHieu();
  }
  //////////////////////////////////////////////////
  if(ThoiGianDelay > 0)
  {
      BatRelay();  // Relay 
      if(TrangThai == 0)
      {
          LucTruoc = millis()/1000;    
          if(( LucTruoc  - LucSau) == 1) 
          {
            ThoiGianDelay--;
            LucSau = LucTruoc;
            NhayLedTinHieu();
          }
      }
      /////////      
  }
  else{ TatRelay();  }// Relay 
  ////////////////////
}

////////////////////
void NhayLedTinHieu(){ digitalWrite(LedTH, HIGH); delay(100); digitalWrite(LedTH, LOW); }
////////////////////
void BatRelay(){ digitalWrite(Relay, HIGH); }
////////////////////
void TatRelay(){digitalWrite(Relay, LOW); }
