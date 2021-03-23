

const int ledPin = 3; 
char ABC[8] = {'A','B','C','D','E','F','G','H'};
char abc[8] = {'a','b','c','d','e','f','g','h'};

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}
 
void loop() {

  if (Serial.available() > 0) 
  {
    char kk = Serial.read();
    for(int i=0;i<8;i++)
    {
      if(kk == ABC[i]) digitalWrite(ledPin, LOW); 
      if(kk == abc[i]) digitalWrite(ledPin, HIGH); 
      delay(100);
    }
  }    

}

