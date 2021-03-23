#define Led 4

void setup() 
{
  pinMode(Led, OUTPUT);
}


void loop()
{
  digitalWrite(Led, HIGH); delay(1000);              
  digitalWrite(Led, LOW);  delay(1000); 
}
