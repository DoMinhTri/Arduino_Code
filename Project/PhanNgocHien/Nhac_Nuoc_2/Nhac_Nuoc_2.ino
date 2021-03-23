

#define LedTH     9
#define Relay     10
#define CamBien   A2

uint16_t ThoiGianDelay   = 0;
uint16_t ThoiDiemSauCung = 0;

boolean TrangThai = 0;
boolean VoRaNhanh = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(Relay,  OUTPUT);          // Relay
  pinMode(LedTH,  OUTPUT);          // LedTH
  
  pinMode(CamBien, INPUT);          // Cam bien
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  TrangThai = analogRead(CamBien); // Cam bien
  //////////////////////////////////////////////////
  if(TrangThai >0 ) 
  {
      ThoiDiemSauCung = millis()/1000;  
      ThoiGianDelay   = 2; 
      digitalWrite(Relay, HIGH);
      digitalWrite(LedTH, HIGH);
  }
  else
  {
    if(ThoiGianDelay > 0) 
    {

        if(( millis()/1000  - ThoiDiemSauCung) == 1) 
        {
              ThoiGianDelay--;

        }
        /////////      
    }

  }
  ////////////////////
}


