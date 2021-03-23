


int Pin[6] = {9, 10, 11, 12, 13};


void setup() {
  
  Serial.begin(9600);
  for(uint8_t i=0; i<5; i++)
  {
     pinMode(Pin[i], OUTPUT);
  }
}
 
void loop() {

  for(uint8_t i=0; i<5; i++)
  {
     Serial.println(Pin[i]);
     analogWrite(Pin[i], 1000);
     delay(1000);
     analogWrite(Pin[i], 0);
     
  }

}

