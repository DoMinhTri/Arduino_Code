#include <SPI.h>
#include "RF24.h"
 
 
const byte LED = 13;
const uint64_t pipe = 0xE8E8F0F0E1LL; // địa chỉ để phát
RF24 radio(9, 10);
 
 
void setup(){ 
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  //============================================================Module NRF24
  radio.begin();                     
  radio.setAutoAck(1);               
  radio.setRetries(1,1);       
  radio.setPALevel(RF24_PA_MAX);      // Dung lượng tối đa
  radio.setChannel(10);               // Đặt kênh
  radio.openWritingPipe(pipe);        // mở kênh
}
 ///////////////////////////////
void loop()
{
      digitalWrite(LED, LOW);
      static char send_payload[] = "ABC";
      radio.write( send_payload, 3, true );
      digitalWrite(LED, HIGH);
      Serial.println("Send");
      delay(1000);
}
