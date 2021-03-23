//////////////////////////////////
int NutA = 10;
int NutB = 9;
int NutC = 8;
//////////////////////////////////
int Den1 = 13;
int Den2 = 12;
int Den3 = 11;
//////////////////////////////////
int BatDen   = 0;
int DenChon  = 0;
//////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(NutA, INPUT);
  pinMode(NutB, INPUT);
  pinMode(NutC, INPUT);
  
  pinMode(Den1, OUTPUT);
  pinMode(Den2, OUTPUT);
  pinMode(Den3, OUTPUT);  
}


void loop() 
{
  
  if( digitalRead(NutA) == HIGH ) { DenChon = Den1; BatDen = 1;  Serial.println("A");}
  if( digitalRead(NutB) == HIGH ) { DenChon = Den2; BatDen = 1;  Serial.println("B");}
  if( digitalRead(NutC) == HIGH ) { DenChon = Den3; BatDen = 1;  Serial.println("C");}
 
  if(BatDen > 0 )
  {
      digitalWrite(Den1, LOW);  
      digitalWrite(Den2, LOW);  
      digitalWrite(Den3, LOW);  
      BatDen = 0;
      digitalWrite(DenChon, HIGH);  
  }

  delay(500);        
  
  Serial.print("Light: ");
  Serial.println(DenChon);
  
}


