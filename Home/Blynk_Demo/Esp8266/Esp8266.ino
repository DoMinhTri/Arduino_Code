#define BLYNK_PRINT Serial    
#include <WiFi.h>
#include <BlynkSimpleEsp8266.h>
/////////////////////////////////////////////////////
char auth[] = "c_qvdrbIV_dxy__DodN3VB78LOgHvJ9B";
/////////////////////////////////////////////////////
char ssid[] = "Robo-T";
char pass[] = "dmt@123abc";
/////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
