char* Pin[18] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3, A4, A5 };

void setup() {
  
  for(uint8_t i=0; i< 18; i++){    pinMode(i, OUTPUT);  }
}


void loop() {
       
       for(uint8_t i=0; i< 18; i++){  Led(i);  }
}

void Led(uint8_t i)
{
  digitalWrite(i, HIGH); delay(300);             
  digitalWrite(i, LOW);  delay(300);   
}
