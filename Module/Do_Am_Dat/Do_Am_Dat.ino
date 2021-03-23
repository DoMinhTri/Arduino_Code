int sensorPin   = A0;   
int ledPin      = 13;      
int sensorValue = 0; 

void setup() 
{
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);    
}

void loop() 
{
  sensorValue = analogRead(sensorPin);    
  delay(1000);          
  Serial.print("sensor = " );                       
  Serial.println(sensorValue);                   
}
