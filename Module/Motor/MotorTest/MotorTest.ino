// Adafruit Motor shield library
// copyright Adafruit Industries LLC, 2009
// this code is public domain, enjoy!

#include <AFMotor.h>

AF_DCMotor motor(4);
int TocDo  = 100;
void setup() {
  Serial.begin(9600);  
  motor.setSpeed(200);
  motor.run(RELEASE);
}

void loop() {
 
  String ChuoiDuLieu = "";
  motor.run(FORWARD);
  
   ///////////////////////////
   while (Serial.available() > 0 ) 
   {
         char KyTu = Serial.read();
         if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu+= KyTu; }
         delay(5);
   }
   if(ChuoiDuLieu !="") 
   {
      Serial.println(ChuoiDuLieu);
      TocDo = ChuoiDuLieu.toInt();
   }
   motor.setSpeed(TocDo);  
 // motor.run(BACKWARD);
  //motor.run(RELEASE);
  delay(1000);
}
