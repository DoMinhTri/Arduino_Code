#include<Servo.h>
Servo SERVO; 

#include<SoftwareSerial.h>
SoftwareSerial ESPData(7, 6); 

#define SerVoPin      9
//////////////////////////////////////////////////////////
#define Kenh_A        13
#define Chieu_Kenh_A  12

#define Kenh_B        11
#define Chieu_Kenh_B  10
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
const int trigPin = 3;
const int echoPin = 2;

int KhoangCach()
{
  long duration, inches, cm;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  cm       = microsecondsToCentimeters(duration);
  inches   = microsecondsToInches(duration);
  return cm;
}
//////////////////////////////////////////////////////////
long microsecondsToInches(long microseconds){  return microseconds / 74 / 2; }
long microsecondsToCentimeters(long microseconds){  return microseconds / 29 / 2; }
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
void BanhPhaiNgung(){ digitalWrite(Chieu_Kenh_A, LOW);  digitalWrite(Kenh_A, LOW);  }
void BanhPhaiToi()  { digitalWrite(Chieu_Kenh_A, LOW);  digitalWrite(Kenh_A, HIGH); }
void BanhPhaiLui()  { digitalWrite(Chieu_Kenh_A, HIGH); digitalWrite(Kenh_A, LOW);  }
//////////////////////////////////////////////////////////
void BanhTraiNgung(){ digitalWrite(Chieu_Kenh_B, LOW);  digitalWrite(Kenh_B, LOW);  }
void BanhTraiToi()  { digitalWrite(Chieu_Kenh_B, LOW);  digitalWrite(Kenh_B, HIGH); }
void BanhTraiLui()  { digitalWrite(Chieu_Kenh_B, HIGH); digitalWrite(Kenh_B, LOW);  }
//////////////////////////////////////////////////////////
uint8_t GTri1 = 0, GTri2 = 0, GTri3 = 0;
uint8_t SLan1 = 0, SLan2 = 0, Slan3 = 0;

void XeDoiHuong(){ XeLui(); delay(1500); XeQuayPhai(); delay(1500); }
void ResetSoLan(){ SLan1 = 0; SLan2 = 0; Slan3 = 0; }
void PhatHienKetXe(uint8_t tpGTri)
{
    if(tpGTri == GTri1)
    {
        SLan1++;
    }
    else if(tpGTri == GTri2)
    {
        SLan2++;
    }
    else if(tpGTri == GTri3)
    {
         Slan3++;
    }
    else
    {
        GTri3 = GTri2;
        GTri2 = GTri1;
        GTri1 = tpGTri;
        /////////////
        Slan3 = SLan2;
        SLan2 = SLan1;
        SLan1 = 1;
    }
    /////
    if(SLan1 >= 10 ){ ResetSoLan(); XeDoiHuong(); }
    if(SLan2 >= 10 ){ ResetSoLan(); XeDoiHuong(); }
    if(Slan3 >= 10 ){ ResetSoLan(); XeDoiHuong(); }
}
//////////////////////////////////////////////////////////
void XeNgung()   { BanhTraiNgung(); BanhPhaiNgung(); }
void XeToi()     { BanhPhaiToi();   BanhTraiToi();   }
void XeLui()     { BanhPhaiLui();   BanhTraiLui();   }
void XeQuayTrai(){ BanhPhaiToi();   BanhTraiLui();   }
void XeQuayPhai(){ BanhPhaiLui();   BanhTraiToi();   }
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void ThungMo(){   SERVO.write(180);}
void ThungDong(){ SERVO.write(0);  }
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////

boolean XeTuLai = false;
void setup()
{
  
  ESPData.begin(9600);
  Serial.begin(9600);
  SERVO.attach(SerVoPin);

  pinMode(Kenh_A, OUTPUT);
  pinMode(Kenh_B, OUTPUT);
  pinMode(Chieu_Kenh_A, OUTPUT);
  pinMode(Chieu_Kenh_B, OUTPUT);
  ///////////////////////////
  XeNgung(); delay(1000);  XeNgung();  delay(2000);
  ///////////////////////////
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
///////////////////////////
void loop()
{
  int cm = KhoangCach();
  ////////////////
  if((cm <= 3) && (1 <= cm))
  {
      XeNgung();
      ThungMo();
      delay(5000);
      ThungDong();
  }
  ////////////////
  if(XeTuLai)
  {
      if(cm <= 10){  XeDoiHuong(); } else { XeToi(); }
      PhatHienKetXe(cm);
  }
  ////////////////
  
  ////////////////
  Serial.print(cm); Serial.print(" cm");Serial.println();
  ////////////////
  while (ESPData.available() > 0 ) 
  {
      char KyTu = ESPData.read(); 
      Serial.println(KyTu);
      if(KyTu      == 'U'){  XeToi();        }
      else if(KyTu == 'L'){  XeLui();        }
      else if(KyTu == 'T'){  XeQuayTrai();   }
      else if(KyTu == 'P'){  XeQuayPhai();   }
      else if(KyTu == 'A'){  XeTuLai = true; }
      ///////////////////
      else if(KyTu == 'N')  
      {
         XeNgung();
         XeTuLai = false;
      }
      /////////////////////////
      else if(KyTu == 'M'){ ThungMo();   }
      else if(KyTu == 'D'){ ThungDong(); }
      else{ XeNgung(); }
      /////////////////
  }
  /////////////////
  delay(100); 
  /////////////////
}
