// RECIVER

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//#include "printf.h"

#define ledPin 13

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(9,10);


// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


void setup(void)
{
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin,HIGH);
  Serial.begin(57600);
 // printf_begin();
 // printf("THIS IS THE RECIVER");

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();


  //radio.printDetails(); uncomment to debug
}

void loop(void)
{
  unsigned long data_in = getData();
  if(data_in == 999)
  {
    Serial.println("THE VALUE IS HIGH!");
    digitalWrite(ledPin,LOW);
    delay(200);
    digitalWrite(ledPin,HIGH);
  }
  else if(data_in == 111)
  {
    Serial.println("THE VALUE IS LOW");
    digitalWrite(ledPin, HIGH);
  }
  else if(data_in != 111 || data_in != 999)
    Serial.println("UNEXPECTED DATA IN");
}

unsigned long  getData()
{
  delay(50);
  unsigned long  got_time;  
  if ( radio.available() )  
  {
    // Dump the payloads until we've gotten everything
    bool done = false;
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = radio.read( &got_time, sizeof(unsigned long ) ); // return true when done
      // Spew it
     // printf("Got payload %lu...",got_time);
    }
  }
  return(got_time);
}





