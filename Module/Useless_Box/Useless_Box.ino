#include <Servo.h>                        

Servo handServo;                          
Servo boxServo;
#define HandMax 180
#define BoxMax   90

int switchStatus=0, action=1, vot, randNumber=0;                   
const int ledPin         = 13;                    
const int frontSwitchPin = 2;
const int handServoPin   = A2;
const int boxServoPin    = A3;

#include "function_1.h"
#include "function_2.h"
#include "function_3.h"

void setup()
{

  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin,HIGH);
  pinMode(frontSwitchPin, INPUT_PULLUP);       
  
  handServo.attach(handServoPin);              
  boxServo.attach(boxServoPin);
 
  HandBuongFast();                        
  BoxNamFast();
  //HandBamFast();
  //randomSeed(analogRead(0));
}

////////////////////////////////////////////////////
String UartData = "";
void loop()
{
  delay(500);
  switchStatus = digitalRead(frontSwitchPin); 
  //Serial.println(switchStatus);  
  action = random(1,16);

  ///////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ UartData+= KyTu; } delay(5);
  }
  ///////////////
  if(UartData != ""){ action = UartData.toInt();  UartData = "";  }
  ///////////////////
  if (switchStatus == LOW)
  {     
    switch (action)
    {
      case 1:  HanhDong_1();   action++; break;
      case 2:  HanhDong_2();   action++; break;
      case 3:  HanhDong_3();   action++; break;
      case 4:  HanhDong_4();   action++; break;
      case 5:  HanhDong_5();   action++; break;
      case 6:  HanhDong_6();   action++; break;
      case 7:  HanhDong_7();   action++; break;
      case 8:  HanhDong_8();   action++; break;
      case 9:  HanhDong_9();   action++; break;
      case 10: HanhDong_10();  action++; break;
      case 11: HanhDong_11();  action++; break;
      case 12: HanhDong_12();  action++; break;
      case 13: HanhDong_13();  action++; break;
      case 14: HanhDong_14();  action++; break;
      case 15: HanhDong_15();  action = 1; break;
    } 
  }             
}
