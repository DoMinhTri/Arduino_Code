/*
https://github.com/MCUdude/MicroCore
https://mcudude.github.io/MicroCore/package_MCUdude_MicroCore_index.json

*/

#define  PutPin  2


void setup() {
  Serial.begin(57600);
  pinMode(PutPin, INPUT);
}


void loop() {

  boolean stas = digitalRead(PutPin);
  if(stas > 0)
      Serial.println("On");
   else
      Serial.println("Off");
  delay(500); 
}
