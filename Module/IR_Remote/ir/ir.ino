#include <IRremote.h>
///////////////////
int led   = 10;
int pinIR = 11;
int SoMin = 0, SoMax = 0;
///////////////////
IRrecv irrecv(pinIR);
decode_results results;
///////////////////
void setup() {
   Serial.begin(9600);
   irrecv.enableIRIn();
   pinMode(led, OUTPUT);     
}

void loop() {

  if (irrecv.decode(&results)) 
 {
    
    int GT = results.value ;
    if(GT > SoMax) SoMax = GT ;
    if(GT < SoMin) SoMin = GT ;
    Serial.print("* ");
    Serial.print(SoMin);
    Serial.print(" <==> ");
    Serial.println(SoMax);
   ///////////////////////////////////////
    //Serial.print(results.value, HEX);
    //Serial.print(" - ");
    //Serial.println(GT);
   /////////////////////////////////////////
   digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
   delay(200);               // wait for a second
   digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW\
   delay(200);  

   irrecv.resume();
 }

}

