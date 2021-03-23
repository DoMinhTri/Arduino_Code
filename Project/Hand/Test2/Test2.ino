
#include <Servo.h>
Servo SV1, SV2;  

void setup() {
  SV1.attach(11);  
  SV2.attach(10);
}

void loop() {

  SV1.write(0);
  SV2.write(0);
  delay(3000);
  SV1.write(180);
  delay(3000);
}

