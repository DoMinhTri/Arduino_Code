#include<Servo.h>
Servo ESC; 
Servo SERVO; 

#include<SoftwareSerial.h>
SoftwareSerial HC05(13, 12); 

#define SerVoPin      8
#define QuatHut       9
#define DenLed        11
#define Pin_Vol       A2


#define Kenh_A        6
#define Kenh_B        5
#define Chieu_Kenh_A  7
#define Chieu_Kenh_B  4

uint8_t TocDo    = 90;
uint8_t GocSerVo = 90;
boolean ChinhSerVo = false;
/////////////////////////////////////////////////////////////
float vout = 0.0;
float vin  = 0.0;
float R1   = 100000.0; // resistance of R1 (100K) 
float R2   = 10000.0;  // resistance of R2 (10K) 

void KiemTraVol()
{
     int value = analogRead(Pin_Vol);
     vout      = (value * 5.0) / 1024.0; 
     vin       = vout / (R2/(R1 + R2)); 
     if (vin <= 10.0){ analogWrite(DenLed, 50); Serial.println("Yeu pin"); } else { analogWrite(DenLed, 0); }
     Serial.println(vin);
}
/////////////////////////////////////////////////////////////
void setup()
{
  
  HC05.begin(9600);
  Serial.begin(9600);
  ESC.attach(QuatHut);
  SERVO.attach(SerVoPin);

  pinMode(Kenh_A, OUTPUT);
  pinMode(Kenh_B, OUTPUT);
  pinMode(Chieu_Kenh_A, OUTPUT);
  pinMode(Chieu_Kenh_B, OUTPUT);

  pinMode(DenLed, OUTPUT);
  pinMode(Pin_Vol, INPUT);
  
  SERVO.write(90);
  ESC.writeMicroseconds(1000);
  delay(3000);
}


void loop()
{
  
  KiemTraVol();
  
  while (HC05.available() > 0 ) 
  {
      char KyTu = HC05.read();  
      
      if(KyTu == 'B')  
          XeLui();
      else if(KyTu == 'F')  
          XeToi();
      else if(KyTu == 'L')
      {  
          if(ChinhSerVo)
          {
              GocSerVo--;
              if(GocSerVo <10 ) {GocSerVo = 10;}
              SERVO.write(GocSerVo);
          }
          else
          {
              XeQuayTrai();
          } 
      }     
      else if(KyTu == 'R') 
      {  
          if(ChinhSerVo)
          {
              GocSerVo++;
              if(GocSerVo >179 ) {GocSerVo = 179;}
              SERVO.write(GocSerVo);
          }
          else
          {
              XeQuayPhai();
          } 
      }       
      else if(KyTu == 'S')  
          XeNgung();
      else if(KyTu == 'W')  
      {
          ESC.writeMicroseconds(1400);
      }
      else if(KyTu == 'w')  
      {
          ESC.writeMicroseconds(1500);
      }
      else if(KyTu == 'U')  
      {
          ESC.writeMicroseconds(1600);
      }
      else if(KyTu == 'u')  
      {
          ESC.writeMicroseconds(1700);
      }      
      else if(KyTu == 'V')  // Loa
      {
          ESC.writeMicroseconds(1800);
      }
      else if(KyTu == 'v')  
      {
          ESC.writeMicroseconds(1000);
      }
      else if(KyTu == 'X')  // Tam giac
      {
          ChinhSerVo = true;
      }
      else if(KyTu == 'x')  
      {
          ChinhSerVo = false;
      }
      else
      {
          uint8_t MucTocDo = KyTu - 48;
          if(MucTocDo > 9){  MucTocDo = 9;   }
          TocDo = map(MucTocDo, 0, 9, 0, 255);
          Serial.println(KyTu);
      }
      /////////////////
  }
  /////////////////
}
//////////////////////////////////////////////////////////
void XeNgung()
{
    BanhTraiNgung();
    BanhPhaiNgung();
}
void XeToi()
{
    BanhPhaiToi();
    BanhTraiToi(); 
}
void XeLui()
{
    BanhPhaiLui();
    BanhTraiLui();
}
void XeQuayTrai()
{
    BanhPhaiToi();
    BanhTraiLui();
}
void XeQuayPhai()
{
    BanhPhaiLui();
    BanhTraiToi();
}
//////////////////////////////////////////////////////////
void BanhTraiNgung()
{ 
    digitalWrite(Chieu_Kenh_A, LOW); 
    digitalWrite(Kenh_A, LOW); 
}
void BanhTraiToi()
{
    digitalWrite(Chieu_Kenh_A, HIGH);
    analogWrite(Kenh_A, TocDo);
}
void BanhTraiLui()
{
    digitalWrite(Chieu_Kenh_A, LOW);
    analogWrite(Kenh_A, TocDo);
}
//////////////////////////////////////////////////////////
void BanhPhaiNgung()
{ 
    digitalWrite(Chieu_Kenh_B, LOW); 
    digitalWrite(Kenh_B, LOW); 
}
void BanhPhaiToi()
{
    digitalWrite(Chieu_Kenh_B, LOW);
    analogWrite(Kenh_B, TocDo);
}
void BanhPhaiLui()
{
    digitalWrite(Chieu_Kenh_B, HIGH);
    analogWrite(Kenh_B, TocDo);
}
/////////////////////////////////////////////////

