void setup() {
  
 Serial.begin(9600);
  pinMode(11,OUTPUT);
}

void loop() {
  
digitalWrite(11,HIGH); 
Serial.println("DEN TAT");
delay(3000);

digitalWrite(11,LOW); 
Serial.println("DEN MO");
delay(3000);

}
