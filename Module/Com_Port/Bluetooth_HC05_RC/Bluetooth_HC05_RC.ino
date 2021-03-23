
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

  while (HC05.available() > 0 ) 
  {
      char KyTu = HC05.read();  
      DataUART+= KyTu;
      if(KyTu == 'w')  Serial.println("1  .w");
      if(KyTu == 'W')  Serial.println("2 ..W");
      if(KyTu == 'u')  Serial.println("3  .u");
      if(KyTu == 'U')  Serial.println("4 ..U");
      if(KyTu == 'v')  Serial.println("5 ..v");
      if(KyTu == 'V')  Serial.println("6 ..V");
      if(KyTu == 'x')  Serial.println("7 ..x");
      if(KyTu == 'X')  Serial.println("8 ..X");
      delay(3);
  }
  /////////////
  if( DataUART != "")
  { 
      Serial.println(DataUART);
      DataUART = ""; 
  }
  /////////////
  delay(100);
}

