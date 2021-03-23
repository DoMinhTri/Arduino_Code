#include<Servo.h>
Servo TayKep; 

#define TayKep_Pin 11
uint8_t nTocDo = 10, nTayKep = 0;

#include<SoftwareSerial.h>
SoftwareSerial HC05(7, 6); 


/////////////////////////////////////////////////////////////
void setup()
{
  
  HC05.begin(9600);
  Serial.begin(9600);
  TayKep.attach(TayKep_Pin);
}


void loop()
{
  
  while (HC05.available() > 0 ) 
  {
      char KyTu = HC05.read();  
      Serial.println(KyTu);
      if(KyTu == 'C')  
      {
          nTayKep+= nTocDo; if(nTayKep > 180)  nTayKep = 180;
          TayKep.write(nTayKep); delay(100);
          Serial.println(nTayKep);
      }
      if(KyTu == 'D')  
      {
          nTayKep-= nTocDo; 
          if(nTayKep < 0)    nTayKep = 0;
          if(nTayKep > 200)  nTayKep = 0;
          TayKep.write(nTayKep); delay(100);
          Serial.println(nTayKep);
      }
      else
      {

      }
      /////////////////
  }
  /////////////////
}
//////////////////////////////////////////////////////////

