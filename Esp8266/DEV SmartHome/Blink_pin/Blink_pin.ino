
#define GPIO2  2
#define GPIO0  0
#define GPIO5  5
#define GPIO4  4
#define GPIO15 15

void setup() {
 
  Serial.begin(9600);
  pinMode(GPIO2,  OUTPUT);
  pinMode(GPIO0,  OUTPUT);
  pinMode(GPIO5,  OUTPUT);
  pinMode(GPIO4,  OUTPUT);
  pinMode(GPIO15, OUTPUT);
}

/////////////////////////////////////////////
void loop() 
{

    NhayDen(GPIO2);
    NhayDen(GPIO0);
    NhayDen(GPIO4);
    NhayDen(GPIO5);
    NhayDen(GPIO15);
}

void NhayDen(uint8_t Pin)
{
    Serial.println(Pin); 
    digitalWrite(Pin, HIGH); delay(500);             
    digitalWrite(Pin, LOW);  delay(500);  
}
