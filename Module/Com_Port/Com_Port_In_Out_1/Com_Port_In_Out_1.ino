#include <Servo.h> 
Servo myservo;
int pos = 0; 

const int ledPin = 8; // the pin that the LED is attached to - change this if you have a separate LED connected to another pin
int incomingByte;      // a variable to read incoming serial data into


int SoChon = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
   myservo.attach(9);
   
  digitalWrite(ledPin, HIGH);
}
 
void loop() {

  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();
    if (incomingByte == 'H') SoChon =1;
    if (incomingByte == 'L') SoChon =2;
  }    
  if(SoChon==1) digitalWrite(ledPin, LOW); 
  if(SoChon==2) ServoQuay() ; 
}



void ServoQuay() 
{ 
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
} 
