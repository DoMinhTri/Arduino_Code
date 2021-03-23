#include<Servo.h>
Servo SERVO; 

#include<SoftwareSerial.h>
SoftwareSerial HC05(3, 2); 

#define VatCanPin     4
#define SerVoPin      5

#define Chieu_Kenh_A  12    // IN1
#define Kenh_A        6     // IN2
#define Kenh_B        11    // IN3
#define Chieu_Kenh_B  10    // IN4

uint8_t TocDo       = 90;
#define MIN_SPEED 0
#define MAX_SPEED 255 //từ 0-255



uint8_t PhatHienCan = 0;
boolean DutTayVao   = false;
boolean MoThungRac  = false;
/////////////////////////////////////////////////////////////
void setup()
{
  
  HC05.begin(9600);
  Serial.begin(9600);
  SERVO.attach(SerVoPin);
  pinMode(VatCanPin, INPUT);

  pinMode(Kenh_A, OUTPUT);
  pinMode(Kenh_B, OUTPUT);
  pinMode(Chieu_Kenh_A, OUTPUT);
  pinMode(Chieu_Kenh_B, OUTPUT);

  SERVO.write(0);
  delay(3000);
}

//////////////////////////////////////////////////////////
void loop()
{  
 
  PhatHienCan = digitalRead(VatCanPin);
  
  if (PhatHienCan == LOW) 
  { 
      //if(DutTayVao == false) DutTayVao = true; 
  }
  else
  {
      if(DutTayVao) 
      {
        MoThungRac = !MoThungRac;
        //DutTayVao = false;
      }
  }
  //////////////////////////////
  while (HC05.available() > 0 ) 
  {
      char KyTu = HC05.read();  
      //Serial.println(KyTu);
      
      if(KyTu == 'B')  
      {
          XeLui();
          //Serial.println("Xe lui");
           MoThungRac = false;
      }
      else if(KyTu == 'F')  
      {
          XeToi();
          //Serial.println("Xe toi");
           MoThungRac = true;
      }
      else if(KyTu == 'L')
      {  
          XeQuayTrai();
          //Serial.println("Quay trai");
      }     
      else if(KyTu == 'R') 
      {  
          XeQuayPhai();
          //Serial.println("Quay phai");
      }       
      else if(KyTu == 'S')  
      {
           XeNgung();
           //Serial.println("Ngung");
      }
      else if(KyTu == 'X')  // Tam giac
      {
          MoThungRac = true;
          Serial.println("Bat nap");
      }
      else if(KyTu == 'x')  
      {
          MoThungRac = false;
          Serial.println("Dong nap");
      }
      else
      {
          uint8_t MucTocDo = KyTu - 48;
          if(MucTocDo > 9){  MucTocDo = 9;   }
          TocDo = map(MucTocDo, 0, 9, 0, 255);
          
          //Serial.println(KyTu);
      }
      /////////////////
  }
  //////////////////////////////////
  if(MoThungRac)
      BatNapThungRac();
  else
      DongNapThungRac();
  //////////////////////////////////
  delay(500);
}
//////////////////////////////////////////////////////////
void BatNapThungRac()
{
  
  SERVO.write(0);
}
void DongNapThungRac()
{
  
  SERVO.write(180);
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
void BanhPhaiNgung()
{ 
    digitalWrite(Chieu_Kenh_A, LOW); 
    digitalWrite(Kenh_A, LOW); 
}
void BanhPhaiToi()
{
    TocDo = constrain(TocDo, MIN_SPEED, MAX_SPEED);
    digitalWrite(Chieu_Kenh_A, LOW);
    analogWrite(Kenh_A, TocDo);
}
void BanhPhaiLui()
{
    TocDo = constrain(TocDo, MIN_SPEED, MAX_SPEED);
    digitalWrite(Chieu_Kenh_A, HIGH);
    analogWrite(Kenh_A, 255 - TocDo);
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
void BanhTraiNgung()
{ 
    digitalWrite(Chieu_Kenh_B, LOW); 
    digitalWrite(Kenh_B, LOW); 
}
void BanhTraiToi()
{
    TocDo = constrain(TocDo, MIN_SPEED, MAX_SPEED);
    digitalWrite(Chieu_Kenh_B, LOW);
    analogWrite(Kenh_B, TocDo);
}
void BanhTraiLui()
{
    TocDo = constrain(TocDo, MIN_SPEED, MAX_SPEED);
    digitalWrite(Chieu_Kenh_B, HIGH);
    analogWrite(Kenh_B, 255 - TocDo);
}
/////////////////////////////////////////////////

