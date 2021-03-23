
#define DenLed   3
#define CamBien  4
#define XaNuoc   1

void(* RESET)(void) = 0;
boolean PhatHien = false;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(XaNuoc, OUTPUT);  // Relay
  pinMode(DenLed, OUTPUT);  // Relay
  pinMode(CamBien, INPUT);  // Cam bien
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  
  boolean TinHieu = digitalRead(CamBien); 
  
  if(TinHieu > 0)
  {
     PhatHien = true;
     digitalWrite(DenLed, HIGH);
  }
  else
  {
     digitalWrite(DenLed, LOW);
  }
  
  if(PhatHien == true)
  {
    if(TinHieu == 1)
    {
      delay(5000); 
      digitalWrite(XaNuoc, HIGH);
      delay(6000); 
      digitalWrite(XaNuoc, LOW);
      PhatHien = false;
    }
  }
  
}
////////////////////


