
#include <SPI.h>
#include "RF24.h"


RF24 radio(9,10);

bool multicast          = true ;
const int role_pin      = 5;
const uint64_t pipes[2] = { 0xEEFAFDFDEELL, 0xEEFDFAF50DFLL };
typedef enum { role_ping_out = 1, role_pong_back } role_e;
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};
role_e role;

const int min_payload_size           = 1;
const int max_payload_size           = 3;
const int payload_size_increments_by = 1;
int next_payload_size                = min_payload_size;
char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

void setup(void)
{
  Serial.begin(115200);
  ////////////
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setCRCLength( RF24_CRC_16 ) ;
  radio.setRetries( 15, 5 ) ;
  radio.setAutoAck( true ) ;
  //radio.setPALevel( RF24_PA_LOW ) ;

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
    
  radio.powerUp() ;
  //radio.stopListening();
  radio.startListening();
  radio.printDetails();
}
////////////////////////////////////////////////////////
void loop(void)
{

    if ( radio.available() )
    {
      uint8_t len;
      bool done = false;
      while (radio.available())
      {
        len = radio.getDynamicPayloadSize();
        radio.read( receive_payload, len );

        receive_payload[len] = 0;

        Serial.print(F("Got response size="));
        Serial.print(len);
        Serial.print(F(" value="));
        Serial.println(receive_payload);
      }
      radio.stopListening();
      radio.write( receive_payload, len, multicast );
      Serial.println(F("Sent response."));
      radio.startListening();
    }

   delay(1000);
  ///////////////////////////////
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
