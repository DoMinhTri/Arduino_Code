

//////////////////////////////
void(* Soft_Reset_2)(void) = 0; 
void (*Soft_Reset_3)(void) = 0x0000;
//////////////////////////////

int led = 13;

void setup()
{                
  pinMode(led, OUTPUT);     
  Serial.begin(9600);
}
///////////////////
void loop() 
{
  Serial.println("//////////////");  
  Serial.println("AA"); digitalWrite(led, HIGH); delay(1000);              
  Serial.println("BB"); digitalWrite(led, LOW);  delay(1000);    
  Serial.println("END"); delay(500);    
  Soft_Reset_2();  //call reset
  ////////////////////////////////////////////
  delay(100);
  Serial.println("never happens");
}
