

///////////////////////////////////////////
///////////////////////////////////////////
//   Port  --- Tiny13 Pin   ---  Arduino UN
//     2       7            -->   13
//     1       6            -->   12
//     0       5            -->   11
//     3       2            -->   
//     4       3            -->   
//             1 ( Reset)   -->   10    ---
//             4 ( GND )    -->   GND   ---
//             8 ( VCC )    -->   VCC
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
// Nguyên lý hoạt động :
// khi phát hiện có sự chuyển trạng thái th
// mỗi giá trị được tính là 1 giây.

#define CamBien   0
#define Relay     3
void(* RESET) (void)  = 0;

boolean BatDauTinhGio = true;
uint8_t  SoGiayCho    = 0;
uint16_t TGBatDau     = 0;
///////////////////////////////////////////
void setup() 
{
  pinMode(Relay,  OUTPUT);          // Rela
  pinMode(CamBien, INPUT);          // Cam 
}
///////////////////////////////////////////
void loop()
{
  boolean TrangThai = digitalRead(CamBien); // Cam 
  /////////////////////////////////////////
  if(TrangThai <= 0 ) 
  {
    SoGiayCho = 3;
    if(BatDauTinhGio)  
    {
      TGBatDau      = millis()/1000; 
      BatDauTinhGio = false;
    }
  }
  else
  {
    if(SoGiayCho > 0) 
    {
      delay(1000);
      SoGiayCho--;
    }
    else
    {
      if(millis()/1000 - TGBatDau > 9)
      {
        delay(2000);
        digitalWrite(Relay, HIGH);
        delay(6000);
        RESET();
      }
    }
    ////////////////////
  }
  ////////////////////
}


