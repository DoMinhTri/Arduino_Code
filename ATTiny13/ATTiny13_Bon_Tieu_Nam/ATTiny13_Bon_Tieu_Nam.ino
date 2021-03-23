

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


#define CamBien       4
#define Relay         1
#define AutoReset     0


long ThoiGianTong   = 0;
int ThoiDiemSauCung = 0;
boolean TinHieuSS   = 0;
boolean XaVanNuoc   = false;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(Relay,      OUTPUT);          // Relay
  pinMode(AutoReset,  OUTPUT);          // AutoReset
  pinMode(CamBien,    INPUT_PULLUP);    // Cam bien
  delay(2000);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  ThoiGianTong = millis()/1000; 
  TinHieuSS    = digitalRead(CamBien); // Cam bien
  //////////////////////////////////////////////////
  if(TinHieuSS == 0) 
  {
      ThoiDiemSauCung = millis()/1000;  
      XaVanNuoc       = true;
  }
  else
  {
    if(XaVanNuoc) 
    {
        if(( millis()/1000  - ThoiDiemSauCung) >= 3) 
        {
              XaVanNuoc = false;
              digitalWrite(Relay, HIGH); delay(5000); digitalWrite(Relay, LOW);
        }
    }
    else
    { 
        if(ThoiGianTong > 18000000){ digitalWrite(AutoReset, HIGH); } // Reset bộ nhớ
    }
  }
  ////////////////////
}


