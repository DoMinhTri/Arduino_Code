byte PWM_1 = A0;
byte PWM_2 = A1;
byte PWM_3 = A2;
byte PWM_4 = A3;
 
int GT1 , GT2, GT3, GT4 ;

 
void setup() {
  
  pinMode(PWM_1, INPUT);
  pinMode(PWM_2, INPUT);
  pinMode(PWM_3, INPUT);
  pinMode(PWM_4, INPUT);
  Serial.begin(115200);
  
}
 
void loop() {
  
  GT1 = pulseIn(PWM_1, HIGH);
  GT2 = pulseIn(PWM_2, HIGH);
  GT3 = pulseIn(PWM_3, HIGH);
  GT4 = pulseIn(PWM_4, HIGH);
  
  Serial.println(""); 
  Serial.print(GT1); Serial.print(" - "); Serial.print(GT2); 
  Serial.print(" - ");
  Serial.print(GT3); Serial.print(" - "); Serial.print(GT4); 
  delay(200);
}
