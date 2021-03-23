
#include <SPI.h>
#include "RF24.h"


RF24 radio(9,10);

bool multicast          = true ;
const int role_pin      = 5;
const uint64_t pipes[1] = { 0xEEFAFDFDEELL };

const int min_payload_size           = 1;
const int max_payload_size           = 3;
const int payload_size_increments_by = 1;
int next_payload_size                = min_payload_size;
char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

void setup(void)
{
  
  //////////////////////////////////
  pinMode(role_pin, INPUT_PULLUP);
  digitalWrite(role_pin,HIGH);
  delay( 20 ); 
  //////////////////////////////////
  Serial.begin(115200);
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
  //radio.startListening();
  radio.printDetails();
}
////////////////////////////////////////////////////////
void loop(void)
{
    int kk = digitalRead(role_pin);
    Serial.print(kk);
    
    if(kk)
    {
      radio.stopListening();
      static char send_payload[] = "CCC";
      Serial.print(F("Now sending length "));
      Serial.println(next_payload_size);
      radio.write( send_payload, next_payload_size, true );
      next_payload_size += payload_size_increments_by;
      if ( next_payload_size > max_payload_size )
        next_payload_size = min_payload_size;
    }

    

    delay(1000);
}

