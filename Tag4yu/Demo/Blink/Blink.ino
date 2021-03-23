


#define Den1 1
#define Den2 2

void setup() {
  pinMode(Den1, OUTPUT);
  pinMode(Den2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(Den1, HIGH);  
  digitalWrite(Den2, HIGH);  
  delay(2000);               
  digitalWrite(Den1, LOW);   
  digitalWrite(Den2, LOW);   
  delay(2000);               
}
