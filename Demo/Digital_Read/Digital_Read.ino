
#define  TinHieu 3
#define  Led     4

void(* RESET) (void) = 0;

void setup() {
  pinMode(TinHieu, INPUT);
  pinMode(Led,     OUTPUT);
}


void loop() {
   
  boolean TT = digitalRead(TinHieu);
  if(TT > 0)
  {
      digitalWrite(Led, HIGH);
  }
  else
  {
     digitalWrite(Led, LOW);
  }
  delay(100);
}



