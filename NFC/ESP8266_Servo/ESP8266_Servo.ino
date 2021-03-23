                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
int TinHieu  = 2; // 1 TX - 3 RX
int ServoPin = 1;

#include <Servo.h>

Servo myservo;  // create servo object to control a servo


void setup() 
{
  myservo.attach(ServoPin); 
  pinMode(TinHieu, INPUT);
}

void loop() {
  
    int TrangThai = digitalRead(TinHieu);
   
    if(TrangThai == 0)
    {
        myservo.write(180); 
        delay(1000);
        myservo.write(0); 
        delay(1000);
    }
    
    delay(100);
}

