

int _TinHieu = 12;   // Chan tin hieu phat hien can
int _Relay   = 2 ;   // Chan out dieu khien Relay
int i        = 0 ;   //////////////////////////////
int SoLan    = 0 ;   //////////////////////////////


void setup()
{
   Serial.begin(9600);
  pinMode(_Relay, OUTPUT);      
  pinMode(_TinHieu, INPUT);     
}
 
void loop()
{
  /////////////////////////////////
  for(i=1; i<=5; i++)
  {
     if( digitalRead(_TinHieu) == LOW ) SoLan ++;
     Serial.println(SoLan);
     delay(200);
  }
  /////////////////////////////////
  LamLai:
  if (SoLan >= 2) 
  {                             
    Serial.println("Open");
    digitalWrite(_Relay, HIGH);  
    for( i=1; i<=7 ; i++)
    {
      delay(300);
      Serial.println("Kiem tra");
      if (digitalRead(_TinHieu) == LOW) goto LamLai;
    }
  }
  /////////////////////////////////  
  digitalWrite(_Relay, LOW); 
  Serial.println("Close");
  delay(500);
  SoLan = 0;
  /////////////////////////////////  
}
