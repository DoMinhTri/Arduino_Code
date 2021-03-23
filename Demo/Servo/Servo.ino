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

Servo Chan2;
Servo Chan3;
Servo Chan4; 


.attach( );
void setup() 
{
  Chan2.attach(16); 
  Chan3.attach(5); 
  Chan4.attach(4); 
  ///////////////
  Chan2.write(0); 
  Chan3.write(90); 
  Chan4.write(180);  
}

void loop() {

}
