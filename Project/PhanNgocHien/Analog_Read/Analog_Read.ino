
#define Pin_TinHieu A0


void setup() {

  Serial.begin(9600);
  
}

void loop() {

  int sensorValue = analogRead(Pin_TinHieu);
  Serial.println(1024 - sensorValue);
  delay(200);       
}
