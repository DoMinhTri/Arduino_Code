

#define Relay1  13   
#define Relay2  A2   
#define Relay3  A3  
#define Relay4  A5

#define LedS1  9   
#define LedS2  6   
#define LedS3  5  
#define LedS4  A4

#define Nut1    8  
#define Nut2    7  
#define Nut3    3 
#define Nut4    4  

int TTG1     = 0;    
int TTG2     = 0; 
int TTG3     = 0; 
int TTG4     = 0;    


/////////////////////////
void setup() 
{ 

  Serial.begin(9600);
  pinMode(Nut1, INPUT);
  pinMode(Nut2, INPUT);
  pinMode(Nut3, INPUT);
  pinMode(Nut4, INPUT);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  
}

void loop() {

//  
//  digitalWrite(Relay1, HIGH);  
//  digitalWrite(Relay2, HIGH); 
//  digitalWrite(Relay3, HIGH); 
//  digitalWrite(Relay4, HIGH); 
//  delay(10000);                       
//  digitalWrite(Relay1, LOW);    
//  digitalWrite(Relay2, LOW);    
//  digitalWrite(Relay3, LOW);    
//  digitalWrite(Relay4, LOW);    
//  delay(1000);

    
 //RelayNN(Relay1);
 //RelayNN(Relay2);
 //RelayNN(Relay3);
 //RelayNN(Relay4);
    TTG1 = digitalRead(Nut1);    BatRelayNutBam(TTG1, Relay1);
    TTG2 = digitalRead(Nut2);    BatRelayNutBam(TTG2, Relay2);
    TTG3 = digitalRead(Nut3);    BatRelayNutBam(TTG3, Relay3);
    TTG4 = digitalRead(Nut4);    BatRelayNutBam(TTG1, Relay4);

  
  delay(500);
}

void BatRelayNutBam(uint8_t Nut, uint8_t Relay)
{
     if (Nut == HIGH) 
     { 
        digitalWrite(Relay, HIGH);  
        Serial.println(Relay);
        //delay(3000);  
      }
      else {    digitalWrite(Relay, LOW);  }
     
}
//////////////////////////////
void RelayNN(uint8_t Pin)
{
    digitalWrite(Pin, HIGH);   
    delay(3000);                       
    digitalWrite(Pin, LOW);    
    delay(3000);
    Serial.println(Pin);
 }
