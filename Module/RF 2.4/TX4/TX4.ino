// TRANSMITTER

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//#include "printf.h"


#define ledPin 6
#define sensorPin 5

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void)
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(sensorPin, INPUT_PULLUP);

  Serial.begin(57600);
//  printf_begin();
 // printf("TRANSMITTER\n");

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  //if ( role == role_ping_out )
  {
    radio.openWritingPipe(pipes[0]);
    //radio.openReadingPipe(1,pipes[1]);
  }
  //else
  {
    //radio.openWritingPipe(pipes[1]);
    // radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  // radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  //radio.printDetails();
}

boolean sensorStat = HIGH;
void loop(void)
{
  boolean recived_data;  
  //boolean sensorStat = digitalRead(sensorPin);
  sensorStat = !sensorStat;
  if(sensorStat == LOW) // connection has been made
  {
    boolean recived_data;  
    do{
      recived_data = sendData(999);
      Serial.println("data has not been recived");
    }while(recived_data == false);
    
    //if(sensorStat == HIGH)
    //sendData(111);
  }
}
boolean sendData(unsigned long dataToSend)
{
   // Take the time, and send it.  This will block until complete
   // printf("Now sending %lu...",dataToSend);
   
   bool ok = radio.write( &dataToSend, sizeof(unsigned long ) );

  if (ok)
  {
     // printf("messege was recived\n"); // confirmed it has been recived
      digitalWrite(ledPin, HIGH);
      delay(200);
      return(true);
  }
  else
  {
      //  printf("failed.\n\r");
      digitalWrite(ledPin,LOW);
      return false;
  }
  
}

