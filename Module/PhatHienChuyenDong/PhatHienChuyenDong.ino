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

#define CamBien   7 
#define Nutbam    5 
#define ledPin    13

int ThoiGianDelay = 0;
int ThoiGianMax   = 180;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      
  digitalWrite(ledPin, HIGH);  
  
  pinMode(Nutbam,  INPUT_PULLUP); 
  pinMode(CamBien, INPUT); 
  ThoiGianDelay = EEPROM.read(0);
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
  // Thời gian delay min 
  if(ThoiGianDelay <=0 ) ThoiGianDelay = 20; 
  /////////////////////////////////////////////////
  static boolean HoatDong           = false;
  static int TGHienTai              = 0;
  static int TGDaQua                = 0;
  static int TrangThaiCamBien       = 0;
  static int TrangThaiCamBienLucSau = 0;
  
  TrangThaiCamBien = digitalRead(CamBien); 
  //////////////////////////////////////////////////
  if(TrangThaiCamBien != TrangThaiCamBienLucSau)
  {
    if(TrangThaiCamBien > 0) 
    {
      HoatDong = false;
      if(ThoiGianDelay <= ThoiGianMax) ThoiGianDelay++; // khi có chuyển động thời gian tăng lên đến ngưỡng Max
    }
    else
    {
      HoatDong  = true;
      TGDaQua   = TGHienTai ;
    }
  }

  //////////////////////////////////////////////////
  if(HoatDong)
  {
      TGHienTai = millis()/1000;    
      if((TGHienTai - TGDaQua) == 1) 
      {
        ThoiGianDelay--;
        TGDaQua = TGHienTai;
      }
      ////////
      if(ThoiGianDelay > 0)
          digitalWrite(ledPin, LOW);  
      else
          digitalWrite(ledPin, HIGH);  
  }
  ////////

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NhanNutChinhThoiGian()
{
  static int BamNut    = 0;
  static int SauKhiBam = 0;
  static int ThoiGianBatDauBam = 0;
  static int ThoiGianDaBam     = 0;
  static int ThoiGianMax       = 0;
  
  BamNut = digitalRead(Nutbam);
  if(BamNut != SauKhiBam)
  {
    if(BamNut == 0)  
    {
      ThoiGianBatDauBam = millis()/1000;
      ThoiGianMax++;
      delay(200);
      EEPROM.write(0,ThoiGianMax);
    }
    else
    {
      ThoiGianBatDauBam = ThoiGianDaBam;
    }
  }
  else
  {
      if(BamNut == 0) 
      {
        ThoiGianDaBam = millis()/1000;
      }
       if((ThoiGianDaBam - ThoiGianBatDauBam) >= 3) 
       {
         EEPROM.write(0,0);
         ThoiGianDelay = 0;
       }
      //Serial.print(ThoiGianDaBam - ThoiGianBatDauBam);
  }
  SauKhiBam = BamNut;
}

