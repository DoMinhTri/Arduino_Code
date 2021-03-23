
void setup() {
  pinMode(9,  OUTPUT);
  pinMode(10, OUTPUT);
}


void loop() {
    BlinkLed(9) ;       
    BlinkLed(10) ;          
}

void BlinkLed(uint8_t Pin)
{
    digitalWrite(Pin, HIGH); //delay(1000); 
   // digitalWrite(Pin, LOW);  //delay(1000);  
}

