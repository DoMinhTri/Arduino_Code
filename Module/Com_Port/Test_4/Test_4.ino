
int incomingByte;      // a variable to read incoming serial data into
int ledPin = 13;

void setup() {
  Serial.begin(9600);
  digitalWrite(ledPin, HIGH);
}
 
void loop() {

  if (Serial.available() > 0) 
  {
    incomingByte = Serial.read();
    Serial.println(incomingByte);
  }    

}


