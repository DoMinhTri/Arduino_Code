
const int ledPin    = 1;
const int buttonPin = 2;     
const int Relay     = 3;    
const int Sensor    = 4;    

int buttonState     = 0;        

void setup() {
  
  pinMode(ledPin, OUTPUT);      
  pinMode(Relay , OUTPUT); 
  pinMode(Sensor, OUTPUT); 
  pinMode(buttonPin, INPUT_PULLUP);    
 

}

void loop(){

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) 
  {     
    digitalWrite(ledPin  , HIGH);  
    digitalWrite(Relay   , HIGH); 
    digitalWrite(Sensor  , HIGH); 
  } 
  else 
  {
    digitalWrite(ledPin  , LOW); 
    digitalWrite(Relay   , LOW); 
    digitalWrite(Sensor  , LOW); 
  }
  
    digitalWrite(ledPin  , HIGH);  
    digitalWrite(Relay   , HIGH); 
    digitalWrite(Sensor  , HIGH);  
    delay(1000);
    digitalWrite(ledPin  , LOW); 
    digitalWrite(Relay   , LOW); 
    digitalWrite(Sensor  , LOW); 
}
