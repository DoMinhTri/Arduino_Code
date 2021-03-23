

#include <Servo.h> 
Servo Hand; 


////////////////////////////////////////////
void setup() 
{
  ////////////////////
  Hand.attach(A3);
  Hand.write(0);
  delay(3000); 
  Hand.write(30);
  delay(1000); 
  Hand.write(0);
  ////////////////////
}
////////////////////////////////////////////
void loop() { delay(1000); }
