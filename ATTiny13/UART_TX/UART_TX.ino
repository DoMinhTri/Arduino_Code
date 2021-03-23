#include <AsmTinySerial.h>  

void setup()
{
  // Initialize the library
  // Transmit on pin 3 @ 115200 baud
  SerialInit( PB0  , 115200 );

}


void loop()
{
  SerialTx( "Init\n" ); // Send Init string
  delay(1000);
}
