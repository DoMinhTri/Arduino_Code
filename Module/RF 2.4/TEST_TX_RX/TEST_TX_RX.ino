
#include <SPI.h>
#include "RF24.h"


RF24 radio(9,10);


const int multicast_pin = 5 ;
const int role_pin      = 6;
bool multicast          = true ;

const uint64_t pipes[2] = { 0xEEFAFDFDEELL, 0xEEFDFAF50DFLL };
typedef enum { role_ping_out = 1, role_pong_back } role_e;


const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};
role_e role;
const int min_payload_size = 1;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size                = min_payload_size;
char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

void setup(void)
{
  //////////////////////////////////
  pinMode(multicast_pin, INPUT);
  digitalWrite(multicast_pin,HIGH);
  delay( 20 ) ;
  //////////////////////////////////
  if( digitalRead( multicast_pin ) )
    multicast = true ;
  else
    multicast = false ;
  //////////////////////////////////
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin,HIGH);
  delay( 20 ); 
  //////////////////////////////////
  if ( digitalRead(role_pin) )
    role = role_ping_out;
  else
    role = role_pong_back;
   //////////////////////////////////
  Serial.begin(115200);
  
  Serial.println(F("RF24/examples/pingpair_multi_dyn/"));
  Serial.print(F("ROLE: "));
  Serial.println(role_friendly_name[role]);
  
  Serial.print(F("MULTICAST: "));
  Serial.println(multicast ? F("true (unreliable)") : F("false (reliable)"));
  
  //
  // Setup and configure rf radio
  //

  radio.begin();

  // enable dynamic payloads
  radio.enableDynamicPayloads();
  radio.setCRCLength( RF24_CRC_16 ) ;

  // optionally, increase the delay between retries & # of retries
  radio.setRetries( 15, 5 ) ;
  radio.setAutoAck( true ) ;
  //radio.setPALevel( RF24_PA_LOW ) ;


  if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  radio.powerUp() ;
  radio.startListening();

  radio.printDetails();
}
////////////////////////////////////////////////////////
void loop(void)
{

  if (role == role_ping_out)
  {

    static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012";
    radio.stopListening();
    Serial.print(F("Now sending length "));
    Serial.println(next_payload_size);
    radio.write( send_payload, next_payload_size, multicast );
    radio.startListening();
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 500 )
        timeout = true;
    if ( timeout )
    {
       Serial.println(F("Failed, response timed out."));
    }
    else
    {

      uint8_t len = radio.getDynamicPayloadSize();
      radio.read( receive_payload, len );
      receive_payload[len] = 0;
      Serial.print(F("Got response size="));
      Serial.print(len);
      Serial.print(F(" value="));
      Serial.println(receive_payload);
    }
    
    next_payload_size += payload_size_increments_by;
    if ( next_payload_size > max_payload_size )
      next_payload_size = min_payload_size;
    delay(250);
  }

////////////////////////////////
  if ( role == role_pong_back )
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
  }
  ///////////////////////////////
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
