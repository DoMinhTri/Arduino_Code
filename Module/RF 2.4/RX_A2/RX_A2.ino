
#include <SPI.h>
#include "RF24.h"


RF24 radio(10,8);
#define DenLed  6
const uint64_t pipes[1] = { 0xEEFAFDFDEELL };


const int min_payload_size           = 1;
const int max_payload_size           = 3;
const int payload_size_increments_by = 1;
int next_payload_size                = min_payload_size;
char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

void setup(void)
{
  Serial.begin(115200);
  pinMode(DenLed, OUTPUT);
  digitalWrite(DenLed, HIGH);
  ////////////
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setCRCLength( RF24_CRC_16 ) ;
  radio.setRetries( 15, 5 ) ;
  radio.setAutoAck( true ) ;
  //radio.setPALevel( RF24_PA_LOW ) ;

  radio.openReadingPipe(1,pipes[0]);
    
  radio.powerUp() ;
  radio.startListening();
  radio.printDetails();
}
////////////////////////////////////////////////////////
void loop(void)
{
    digitalWrite(DenLed, HIGH);
    if ( radio.available() )
    {
      uint8_t len;
      while (radio.available())
      {
        len = radio.getDynamicPayloadSize();
        radio.read( receive_payload, len );
        receive_payload[len] = 0;
        Serial.print(F("Got response size="));
        Serial.print(len);
        Serial.print(F(" value="));
        Serial.println(receive_payload);
        digitalWrite(DenLed,LOW);
        delay(500);
        digitalWrite(DenLed,HIGH);
        delay(500);
      }
    }

   delay(1000);
  ///////////////////////////////
}

