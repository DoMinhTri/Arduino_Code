/*
  Kết nối:
          TB6600                  Arduino
           ENA+                      8
           DIR+                      2
           PUL+                      5 
           ENA-                     GND
           DIR-                     GND
           PUL-                     GND

  A+ A- B+ B- kết nối với động cơ

  Nguồn đầu vào là 9V - 42V.
 */


const int stepPin = 7; 
const int dirPin  = 6; 
const int enPin   = 5;
void setup() {
  
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,LOW);
  
}
void loop() {
  
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 30000; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(20); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(20); 
  }
  delay(1000); // One second delay

  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 30000; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(20);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(20);
  }
  delay(1000);
  
}
