//////////////////////////////////////////////////////////////////////////////////
#define LedConn  2
#define Relay1   34
#define Relay2   35
#define Relay3   33
#define Relay4   32
uint8_t MangTTDen[4] = {0,0,0,0};
/////////////////////////////////////////////////////////////////////////////
String TTCongTac()
{
  String TTDen =  String(MangTTDen[0]) +  String(MangTTDen[1]) +  String(MangTTDen[2]) +  String(MangTTDen[3]) ;
  return TTDen;
}
/////////////////////////////////////////////////////////////////////////////
void LedOn(){  digitalWrite(LedConn, HIGH); }
void LedOff(){ digitalWrite(LedConn, LOW);  }
/////////////////////////////////////////////////////////////////////////////
void BatDen()
{
    digitalWrite(LedConn,(MangTTDen[0] + 1)%2); 
    digitalWrite(Relay1, (MangTTDen[0] + 1)%2); 
    digitalWrite(Relay2, (MangTTDen[1] + 1)%2); 
    digitalWrite(Relay3, (MangTTDen[2] + 1)%2); 
    digitalWrite(Relay4, (MangTTDen[3] + 1)%2); 
}
/////////////////////////////////////////////////////////////////////////////
void IOSetup()
{
  pinMode(LedConn, OUTPUT); 
  pinMode(Relay1 , OUTPUT); 
  pinMode(Relay2 , OUTPUT); 
  pinMode(Relay3 , OUTPUT); 
  pinMode(Relay4 , OUTPUT); 
  ////////////////////////
  delay(2000);
  BatDen();
}
/////////////////////////////////////////////////////////////////////////////
