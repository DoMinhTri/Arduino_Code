#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0
uint8_t TocDo = 230;

#include<SoftwareSerial.h>
SoftwareSerial HC05(13, 12); 

void setup()
{
  HC05.begin(9600);
  Serial.begin(9600);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}
 
void loop()
{
  while (HC05.available() > 0 ) 
  {
      char KyTu = HC05.read();  
      Serial.println(KyTu);
      if(KyTu == 'B')      XeLui();
      else if(KyTu == 'F') XeToi();
      else if(KyTu == 'S') XeNgung();
      else if(KyTu == 'L') XeQuayTrai();
      else if(KyTu == 'R') XeQuayPhai();

      else if(KyTu == 'I') XeToiPhai();
      else if(KyTu == 'J') XeLuiPhai();
      else if(KyTu == 'H') XeLuiTrai();
      else if(KyTu == 'G') XeToiTrai();
      
      else
      {
          uint8_t MucTocDo = KyTu - 48;
          if(MucTocDo > 9){  MucTocDo = 9;   }
          TocDo = map(MucTocDo, 0, 9, 0, 255);
          Serial.println(KyTu);
      }
      /////////////////
  }
}
/////////////////////////////////////
void XeNgung()
{
  BanhTrai_Dung();
  BanhPhai_Dung();
}
/////////////////
void XeToi()
{
   BanhPhai_Toi(TocDo);
   BanhTrai_Toi(TocDo);
}
/////////////////
void XeToiPhai()
{
   BanhPhai_Dung();
   BanhTrai_Toi(TocDo);
}
/////////////////
void XeLuiPhai()
{
   BanhPhai_Dung();
   BanhTrai_Lui(TocDo);
}
/////////////////
void XeToiTrai()
{
   BanhPhai_Toi(TocDo);
   BanhTrai_Dung();
}
/////////////////
void XeLuiTrai()
{
   BanhPhai_Lui(TocDo);
   BanhTrai_Dung();
}
/////////////////
void XeLui()
{
   BanhPhai_Lui(TocDo);
   BanhTrai_Lui(TocDo);
}
/////////////////
void XeQuayTrai()
{
   BanhPhai_Toi(TocDo);
   BanhTrai_Lui(TocDo);
}
/////////////////
void XeQuayPhai()
{
   BanhPhai_Lui(TocDo);
   BanhTrai_Toi(TocDo);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void BanhPhai_Dung() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
 
void BanhTrai_Dung() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
 
void BanhPhai_Toi(int speed) 
{ 
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  digitalWrite(IN1, HIGH);
  analogWrite(IN2, 255 - speed);
}
 
void BanhPhai_Lui(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  digitalWrite(IN1, LOW);
  analogWrite(IN2, speed);
}
 
void BanhTrai_Toi(int speed) 
{ 
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  analogWrite(IN3, speed);
  digitalWrite(IN4, LOW);
}
 
void BanhTrai_Lui(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);
  analogWrite(IN3, 255 - speed);
  digitalWrite(IN4, HIGH);
}
