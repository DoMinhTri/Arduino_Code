
#include <SPI.h>
#include "RF24.h"

#define NutNhan 5
RF24 radio(9,10);
const uint64_t pipes[1] = { 0xEEFAFDFDEELL };

void setup(void)
{
  //////////////////////////////////
  Serial.begin(115200);
  pinMode(NutNhan, INPUT_PULLUP);
  ////////////
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setCRCLength( RF24_CRC_16 ) ;
  radio.setRetries( 15, 5 ) ;
  radio.setAutoAck( true ) ;
  //radio.setPALevel( RF24_PA_LOW ) ;
  radio.openWritingPipe(pipes[0]);
  radio.powerUp() ;
  radio.stopListening();
  radio.printDetails();
}
////////////////////////////////////////////////////////
bool kk = true;
void loop(void)
{
     //kk = !kk;
    //if(kk)
    {
      static char send_payload[] = "ABC";
      Serial.println(F("Now sending length "));
      radio.write( send_payload, 3, true );
    }
    delay(500);
}

