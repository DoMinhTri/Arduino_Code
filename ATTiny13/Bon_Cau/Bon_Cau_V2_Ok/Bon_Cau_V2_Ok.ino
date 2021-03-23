

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

boolean PhatHien  = false;
uint8_t SoGiayCho = 0;
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
    SoGiayCho = 50;
    PhatHien  = true;
  }
  else
  {
    if(SoGiayCho > 0) 
    {
      SoGiayCho--;
      delay(100);
    }
    else
    {
       if(PhatHien)
       {
          digitalWrite(Relay, HIGH);
          delay(6000);
          digitalWrite(Relay, LOW);
          PhatHien = false;
       }
    }
    ////////////////////
  }
  ////////////////////
}


