
#define Kenh1 2
#define Kenh2 3
#define Kenh3 4
#define Kenh4 5
#define Kenh5 6
#define Kenh6 7

#define PWMPin 10
#define SPPin  A5

#define FR_1  A1 
#define EN_1  A0
#define FR_2  A3
#define EN_2  A2

bool VKenh1 = 0, VKenh2 = 0;
uint16_t GTTam, GtKenh1, GtKenh2, GtKenh3, GtKenh4, GtKenh5, GtKenh6;


#include<Servo.h>
Servo ESC;

void setup() {
  
  Serial.begin(9600);
  
  pinMode(Kenh1, INPUT);
  pinMode(Kenh2, INPUT);
  pinMode(Kenh3, INPUT);
  pinMode(Kenh4, INPUT);
  pinMode(Kenh5, INPUT);
  pinMode(Kenh6, INPUT);
  
  pinMode(FR_1, OUTPUT);
  pinMode(EN_1, OUTPUT);
  pinMode(FR_2, OUTPUT);
  pinMode(EN_2, OUTPUT);
  pinMode(SPPin, OUTPUT);
  
  ESC.attach(PWMPin);
  ESC.writeMicroseconds(1000);
  delay(1000);
  
}

String UartData = "a";

void loop() {
  
  ESC.writeMicroseconds(2000);
  analogWrite(SPPin,255);
  ////////////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ UartData += KyTu; } delay(5);
  }
  
  Serial.println("");
   GTTam = pulseIn(Kenh1, HIGH); GtKenh1 = GTTam;  if(GTTam <=1400){ BanhPhaiLui(); VKenh1 = 0; } else if(GTTam>1700){ BanhPhaiToi(); VKenh1 = 1; } else{ BanhPhaiNgung(); VKenh1 = 2; }
   GTTam = pulseIn(Kenh2, HIGH); GtKenh2 = GTTam;  if(GTTam <=1400){ BanhTraiLui(); VKenh2 = 0; } else if(GTTam>1700){ BanhTraiToi(); VKenh1 = 1; } else{ BanhTraiNgung(); VKenh1 = 2; }
   GTTam = pulseIn(Kenh3, HIGH); GtKenh3 = GTTam; 
   GTTam = pulseIn(Kenh4, HIGH); GtKenh4 = GTTam;
   GTTam = pulseIn(Kenh5, HIGH); GtKenh5 = GTTam; 
   GTTam = pulseIn(Kenh6, HIGH); GtKenh6 = GTTam; 
   //////////////
   
   
   
  if(UartData == "a")
  {
     Serial.print(VKenh1); Serial.print(" - "); Serial.print(VKenh2);
  }
  else
  {
    Serial.print(GtKenh1); Serial.print(" - ");
    Serial.print(GtKenh2); Serial.print(" - ");
    Serial.print(GtKenh3); Serial.print(" - ");
    Serial.print(GtKenh4); Serial.print(" - ");
    Serial.print(GtKenh5); Serial.print(" - "); 
    Serial.print(GtKenh6); 
    UartData = "";
  }

   delay(200);
}
//////////////////////////////////////////////////////////////////
void BanhPhaiToi()
{
    digitalWrite(EN_1,1);
    digitalWrite(FR_1,0);
}
//////////////////////////////////////////////////////////////////
void BanhPhaiLui()
{
    digitalWrite(EN_1,1);
    digitalWrite(FR_1,1);
}
//////////////////////////////////////////////////////////////////
void BanhPhaiNgung()
{
    digitalWrite(EN_1,0);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void BanhTraiToi()
{
    digitalWrite(EN_2,1);
    digitalWrite(FR_2,0);
}
//////////////////////////////////////////////////////////////////
void BanhTraiLui()
{
    digitalWrite(EN_2,1);
    digitalWrite(FR_2,1);
}
//////////////////////////////////////////////////////////////////
void BanhTraiNgung()
{
    digitalWrite(EN_2,0);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void XeChayToi()
{
    
}
///////////////////
void XeChayLui()
{
    
}
///////////////////
void XeNgung()
{
    
}
///////////////////
void XeQuaTrai()
{
    
}
///////////////////
void XeQuaPhai()
{
    
}


