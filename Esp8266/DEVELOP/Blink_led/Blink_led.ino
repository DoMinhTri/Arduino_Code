
int ledPin_2 = 2;

void setup() {

  pinMode(ledPin_2, OUTPUT);
}
 
void loop() {

  digitalWrite(ledPin_2, HIGH); 
  delay(1000);                 
  digitalWrite(ledPin_2, LOW);  
  delay(1000);                  
}
