#include   <IRremote.h>

int STATUS_PIN = 13;
int BUTTON_PIN = 10;
int RECV_PIN = 11;


IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(BUTTON_PIN, INPUT);
  pinMode(STATUS_PIN, OUTPUT); 
  randomSeed(analogRead(0)); 
}

void loop() {
  if (irrecv.decode(&results)) {
    digitalWrite(STATUS_PIN, HIGH);
    Serial.println(results.value, HEX);
    for (int i = 0; i < random(3,10) ; i++) {
// Change the method below and its two parameters based on the output of the IRrecvDump.ino sketch.
       irsend.sendNEC(0x80BF4BB4, 32); 
    // sendNEC(unsigned long data, int nbits)
    // sendSony(unsigned long data, int nbits)
    // sendRC5(unsigned long data, int nbits)
    // sendRC6(unsigned long data, int nbits)
       delay(100);
    }   
    digitalWrite(STATUS_PIN, LOW);
    irrecv.enableIRIn();
    irrecv.resume();
  } 
}
