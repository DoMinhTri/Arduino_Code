


void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A3, OUTPUT);

  //pinMode(13, OUTPUT);
}

////////////////////////////////////
void loop() {
  BlikLed(3) ;
  BlikLed(4) ;
  BlikLed(5) ;
  BlikLed(A3) ;

  //BlikLed(13) ;
  
}

void BlikLed(uint8_t nLed)
{
  digitalWrite(nLed, HIGH);  delay(1000);  
  digitalWrite(nLed, LOW);   delay(1000); 
}

