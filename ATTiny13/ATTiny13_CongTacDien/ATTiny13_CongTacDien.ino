/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//   Port ---  Tiny13 Pin
//     0       5
//     1       6
//     2       7
//     3       2
//     4       3
//     5       1 ( Reset)
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////




int _TinHieu = 8;   // Chan tin hieu phat hien can
int _Relay   = 13 ;   // Chan out dieu khien Relay
int i        = 0 ;   //////////////////////////////
int SoLan    = 0 ;   //////////////////////////////
int Batden   = 0;
 
void setup()
{
  pinMode(_Relay, OUTPUT);      
  pinMode(_TinHieu, INPUT);
  digitalWrite(_Relay, LOW);  
}
 
void loop()
{
    /////////////////////////////////
  for( i=1; i<=5 ; i++)
  {
     int BamNut = digitalRead(_TinHieu);
    if( BamNut == HIGH ) SoLan ++;
    if (SoLan >= 3) goto LamLai;
    delay(200);
  }

  /////////////////////////////////
  LamLai:
  if (SoLan >= 3 ) 
  {                             
    if(Batden == 0)
    {
      Batden = 1;
      digitalWrite(_Relay, HIGH);  
      delay(1500);
    }
    else
    {
      Batden = 0;
      digitalWrite(_Relay, LOW);  
      delay(1500);
    }
    SoLan = 0;
  }
}
