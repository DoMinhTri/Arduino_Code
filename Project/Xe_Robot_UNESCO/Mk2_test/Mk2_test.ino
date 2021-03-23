
#include <Servo.h>

Servo SV1; 
Servo SV2; 
Servo SV3; 


void setup() {
  
  Serial.begin(9600);
  SV3.attach(12);  
  SV2.attach(11); 
  SV1.attach(10);

  SV3.write(180);
  SV2.write(90);
  SV1.write(0);
}

void loop() 
{
//  uint8_t i=0;
//  for( i=0; i< 180; i+= 1){   myservo.write(i);  delay(30); Serial.println(i); }   
//  //myservo.write(90);  delay(5000);
//  for( i=180; i>0; i-= 1){  myservo.write(i);  delay(30);  Serial.println(i); } 
//  //myservo.write(150);  delay(5000);  
  
}

