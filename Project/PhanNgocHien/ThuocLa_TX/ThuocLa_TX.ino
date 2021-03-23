#include <SPI.h>
#include "RF24.h"
 
const uint64_t pipe = 0xE8E8F0F0E1LL; // địa chỉ để phát
RF24 radio(9,10); //thay 10 thành 53 với mega
byte msg[3];
const int sensor = A0;
int value = 0;
 
void setup(){ 
  Serial.begin(9600);
  //============================================================Module NRF24
  radio.begin();                     
  radio.setAutoAck(1);               
  radio.setRetries(1,1);             
  radio.setDataRate(RF24_1MBPS);    // Tốc độ truyền
  radio.setPALevel(RF24_PA_MAX);    // Dung lượng tối đa
  radio.setChannel(10);             // Đặt kênh
  radio.openWritingPipe(pipe);      // mở kênh
  pinMode(sensor, INPUT);
  pinMode(13, OUTPUT);
}
 
void loop(){
  value = analogRead(sensor);
  msg[0] = value;
  radio.write(&msg, sizeof(msg));
  Serial.println(msg[0]);
  
  if(msg[0] > 0)
  {
    NhayLed();
  }
  delay(100);
}
/////////////////////
void NhayLed()
{
   digitalWrite(13, HIGH);
   delay(300);
   digitalWrite(13, LOW);
   delay(300);
}

