#include <SPI.h>
#include "RF24.h"
 
const uint64_t pipe = 0xE8E8F0F0E1LL; // địa chỉ phát
RF24 radio(9,10);//thay 10 thành 53 với mega
byte msg[3];
const int led = 13;
int led_st = 0;
 
void setup(){
  Serial.begin(115200);
  radio.begin();                     
  radio.setAutoAck(1);              
  radio.setDataRate(RF24_1MBPS);    // Tốc độ dữ liệu
  radio.setChannel(10);             // Đặt kênh
  radio.openReadingPipe(1,pipe);     
  radio.startListening();            
  pinMode(led, OUTPUT);
}
 
void loop()
{
  ////////////////////////////
  if (radio.available())
  {
    while (radio.available())
    {
      radio.read(&msg, sizeof(msg));
      Serial.print("Value: ");
      Serial.println((char)msg[1]);
      analogWrite(led, msg[1]);
    }
  }
  ////////////////////////////
}
