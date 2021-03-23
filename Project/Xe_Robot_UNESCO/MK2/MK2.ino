// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 

Servo claw;
Servo twist;
Servo wrist;
Servo elbow;
Servo shoulder;
//Servo turret;

int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
   Serial.begin(9600);
  claw.attach(8);   // claw
  twist.attach(9);  // twist 
  wrist.attach(10);  // wrist 
  elbow.attach(11); //elbow
  //shoulder.attach(5);
  //turret.attach(6);

} 
 
 
void loop() 
{ 
elbow.write(180); delay(100);
shoulder.write(105); delay(100);
elbow.write(100);
wrist.write(0);
//delay(1000);

clawcheck();
delay(2000);
//shouldercheck();
//elbowcheck();
delay(3000);
} 

void clawcheck(){
    for(pos = 50; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    claw.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    Serial.print("Claw: ");
    Serial.println(pos);
  } 
  for(pos = 180; pos>=50; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    claw.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    Serial.print("Claw: ");
    Serial.println(pos);
  }
}

void twistcheck(){
    for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    twist.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    Serial.print("Twist: ");
    Serial.println(pos);
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    claw.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    Serial.print("Twist: ");
    Serial.println(pos);
  }
}

void wristcheck(){
    for(pos = 0; pos < 100; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    wrist.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    Serial.print("Wrist: ");
    Serial.println(pos);
  } 
  for(pos = 100; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    wrist.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    Serial.print("Wrist: ");
    Serial.println(pos);
  }
}



void elbowcheck(){
    for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    elbow.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    Serial.print("Elbow: ");
    Serial.println(pos);
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    elbow.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    Serial.print("Elbow: ");
    Serial.println(pos);
  }
}
