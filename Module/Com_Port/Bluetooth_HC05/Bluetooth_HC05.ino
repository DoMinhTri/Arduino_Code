
#include <SoftwareSerial.h>
SoftwareSerial HC05(7, 6); // RX | TX

String DataUART  = ""; 
const int ledPin = 13; 

char ABC[8] = {'A','B','C','D','E','F','G','H'};
char abc[8] = {'a','b','c','d','e','f','g','h'};

void setup() 
{
  Serial.begin(9600);
  HC05.begin(38400);
  pinMode(ledPin, OUTPUT);
}
 
void loop() {

  if (HC05.available() > 0 ) 
  {
    char kk = HC05.read();
    Serial.println(kk);
    
    for(int i=0;i<8;i++)
    {
      if(kk == ABC[i]) digitalWrite(ledPin, HIGH); 
      if(kk == abc[i]) digitalWrite(ledPin, LOW); 
      delay(50);
    }
  }
  /////////////
  delay(100);
}

