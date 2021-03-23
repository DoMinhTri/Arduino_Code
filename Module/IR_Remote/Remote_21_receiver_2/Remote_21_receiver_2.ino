#include "IRremote.h"
int receiver = 8; 
IRrecv irrecv(receiver);        
decode_results results;    

 int nData = 13;
/////////////////////////////////////
void setup()  
{
  Serial.begin(9600);
  pinMode(nData, OUTPUT);
  
  
  Serial.println("-------------------");
  Serial.println("| B1    B2    B3  |");
  Serial.println("| B4    B5    B6  |");
  Serial.println("| B7    B8    B9  |");
  Serial.println("| B10   B11   B12 |");
  Serial.println("| N_1   N_2   N_3 |");
  Serial.println("| N_4   N_5   N_6 |");
  Serial.println("| N_7   N_8   N_9 |");
  Serial.println("-------------------");
  irrecv.enableIRIn(); 
}
/////////////////////////////////////
void loop()  
{
  if (irrecv.decode(&results))
  {
    // Serial.println(results.value, HEX);  UN Comment to see raw values
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}
/////////////////////////////////////
void translateIR() 
{
  switch(results.value)
  {
  case 0xFFA25D:  
    Serial.println(" B1"); 
    digitalWrite(nData, HIGH);
    break;
  case 0xFF629D:  
    Serial.println(" B2"); 
    digitalWrite(nData, LOW);
    break;
  case 0xFFE21D:  
    Serial.println(" B3"); 
    break;
  case 0xFF22DD:  
    Serial.println(" B4"); 
    break;
  case 0xFF02FD:  
    Serial.println(" B5"); 
    break;
  case 0xFFC23D:  
    Serial.println(" B6"); 
    break;
  case 0xFFE01F:  
    Serial.println(" B7"); 
    break;
  case 0xFFA857:  
    Serial.println(" B8"); 
    break;
  case 0xFF906F:  
    Serial.println(" B9"); 
    break;
  case 0xFF6897:  
    Serial.println(" B10"); 
    break;
  case 0xFF9867:  
    Serial.println(" B11"); 
    break;
  case 0xFFB04F:  
    Serial.println(" B12"); 
    break;
  case 0xFF30CF:  
    Serial.println(" 1              "); 
    break;
  case 0xFF18E7:  
    Serial.println(" 2              "); 
    break;
  case 0xFF7A85:  
    Serial.println(" 3              "); 
    break;
  case 0xFF10EF:  
    Serial.println(" 4              "); 
    break;
  case 0xFF38C7:  
    Serial.println(" 5              "); 
    break;
  case 0xFF5AA5:  
    Serial.println(" 6              "); 
    break;
  case 0xFF42BD:  
    Serial.println(" 7              "); 
    break;
  case 0xFF4AB5:  
    Serial.println(" 8              "); 
    break;
  case 0xFF52AD:  
    Serial.println(" 9              "); 
    break;
  default: 
    Serial.println(" other button   ");
  }
  delay(500);
}

