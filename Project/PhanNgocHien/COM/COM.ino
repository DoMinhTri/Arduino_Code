void setup() {
  Serial.begin(9600);
  pinMode(7,INPUT);
  pinMode(11,OUTPUT);
}

void loop() {
  
  int TinHieu = digitalRead(7);
  Serial.println(TinHieu);
  if(TinHieu > 0)
  {
    digitalWrite(11,0);
  }
  else
  {
    digitalWrite(11,1);
  }
  delay(500);
}
