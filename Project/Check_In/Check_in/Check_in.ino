
void setup() 
{

  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);  
}

// the loop routine runs over and over again forever:
void loop() 
{
  
  int A = digitalRead(2);
  int B = digitalRead(3);
  int C = digitalRead(4);
  int D = digitalRead(5);
  int E = digitalRead(6);
  int F = digitalRead(7);
  int G = digitalRead(8);
  int H = digitalRead(9);  
  
  Serial.println("---------------------------------");
  Serial.println(A);
  Serial.println(B);
  Serial.println(C);
  Serial.println(D);
  Serial.println(E);
  Serial.println(F);
  Serial.println(G);
  Serial.println(H);  
  delay(150);        
}



