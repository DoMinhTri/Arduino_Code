#include<Servo.h>
 
Servo ESC; 
 
 
void setup()
{
  ESC.attach(10);
  
}

boolean KK = true;

void loop()
{
  if(KK)
  {
    ESC.writeMicroseconds(1000);
    delay(1000);
    KK = false ;
  }
  ESC.writeMicroseconds(1100);
}
