

#define Relay   13
#define CamBien 11

int ThoiGianDelay = 0;
int TTCamBien = 0, LucTruoc = 0, LucSau = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(Relay,   OUTPUT);        // Relay
  pinMode(CamBien, INPUT);         // Cam bien

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  TinhThoiGianThuc();
  delay(50);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TinhThoiGianThuc()
{
  TTCamBien = digitalRead(CamBien); // Cam bien
  if(ThoiGianDelay > 60) 
      { digitalWrite(Relay, HIGH); }
  else
      { digitalWrite(Relay, LOW);  }
  //////////////////////////////////////////////////
  if(TTCamBien == 1) 
  {
      LucTruoc  = millis()/1000;  
      LucSau    = LucTruoc ;
      ThoiGianDelay++; 
      delay(50);
      Serial.print("# Thoi gian tang: ");  Serial.println(ThoiGianDelay);  
  }
  //////////////////////////////////////////////////
  if(ThoiGianDelay > 0)
  {
      LucTruoc = millis()/1000;    
      if((LucTruoc - LucSau) == 1) 
      {
         ThoiGianDelay--;
         LucSau = LucTruoc;
         Serial.print(" @@@ Thoi gian giam: ");  Serial.println(ThoiGianDelay);  
      }
      /////////      
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


