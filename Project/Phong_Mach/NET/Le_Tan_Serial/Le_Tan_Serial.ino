


#define Led     A2
#define LCDLed  13
#define Chuong  3

String Chuoi    = "";
int TGBatChuong = 0;
boolean ChoBatChuong = false;
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void setup(void) 
{
    Serial.begin(9600);
    ////////////////////  IN - OUT
    pinMode(Led,     OUTPUT);
    pinMode(Chuong,  OUTPUT);
    pinMode(LCDLed,  OUTPUT);
}
/////////////////////////////////////////////////////////////////////////////////////////

void loop(void) 
{
  boolean KiemTraChuoi = true;
  ////////////////////////////////
  while (Serial.available() > 0) 
  {
      char kk = Serial.read();s
      Chuoi = Chuoi + kk;
      KiemTraChuoi = false;
   }  
  ////////////////////////////////   
  if(KiemTraChuoi)
  {
    if(Chuoi != "")
    { 
      String KyTu = "";
      int VT1 = 0,VT2 = 0;
      for(int i=0; i <= Chuoi.length();i++)
      {
          KyTu = Chuoi.substring(i,i + 1);
          if(KyTu == "-" && VT1 == 0) VT1 = i;
          if(KyTu == "-" && VT1 >  0) VT2 = i;
      }
      String BenhNhanMoi = Chuoi.substring(0, VT1);
      String SoBenhNhan  = Chuoi.substring(VT1 + 1, VT2);
      String TenPhong    = Chuoi.substring(VT2 + 1, Chuoi.length());
      Serial.print("Yeu cau: ");  Serial.println(SoBenhNhan);
      Serial.println(TenPhong);
      ////////////////////
      if(SoBenhNhan  == "no") { TatDen(); } else { BatDen(); }
      ////////////////////
      if(BenhNhanMoi == "yes") 
      {
          TGBatChuong = millis()/1000;
          ChoBatChuong = true;
          BatChuong();
      }
      ////////////////////      
      Chuoi = "";
    }
  }
  ////////////////////////////////  
  if(ChoBatChuong) { KiemTraBatChuong(); }
  delay(100);
}

////////////////////////////////  
void KiemTraBatChuong()
{
    if((millis()/1000 - TGBatChuong) > 3) { TatChuong(); ChoBatChuong = false;}
}
////////////////////////////////  
void BatChuong(){ digitalWrite(LCDLed,HIGH); }
////////////////////////////////  
void TatChuong(){ digitalWrite(LCDLed,LOW); }
////////////////////////////////  
void BatDen(){ digitalWrite(LCDLed,HIGH); }
////////////////////////////////  
void TatDen(){ digitalWrite(LCDLed,LOW); }
