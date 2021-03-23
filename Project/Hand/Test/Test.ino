/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 28 May 2015
 by Michael C. Miller
 modified 8 Nov 2013
 by Scott Fitzgerald

 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo SV1;
Servo SV2;
Servo SV3;
Servo SV4;
Servo SV5;// create servo object to control a servo 
                // twelve servo objects can be created on most boards
 

void setup() 
{ 
  SV1.attach(13); 
  SV2.attach(11);
  SV3.attach(10);
  SV4.attach(9);
  SV5.attach(6);
} 
 
void loop() 
{ 
  SV1.write(0);
  SV2.write(0);  
  SV3.write(0);  
  SV4.write(0);  
  SV5.write(0);  
} 

