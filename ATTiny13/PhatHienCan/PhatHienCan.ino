
const int buttonPin = 8 ; 
const int ledPin    = 13;
int TrangThai       = 0 ; 

void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      
}

void loop()
{

  TrangThai = digitalRead(buttonPin);
  
  Serial.print(TrangThai);  Serial.print(" - ");
  
  if (TrangThai == HIGH) 
  {     
      digitalWrite(ledPin, HIGH);  
      Serial.println(1);
  } 
  else 
  {
      digitalWrite(ledPin, LOW); 
      Serial.println(0);
  }
  
  delay(100);
}
