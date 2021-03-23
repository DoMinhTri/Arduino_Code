#include <SPI.h>
#include "RF24.h"
 
const uint64_t pipe = 0xE8E8F0F0E1LL; // địa chỉ phát
RF24 radio(9,10);//thay 10 thành 53 với mega
byte msg[3];

int Max = 0;
#define Led 3
#define Loa A2
int led_st = 0;

void setup(){
  Serial.begin(9600);
  radio.begin();                     
  radio.setAutoAck(1);              
  radio.setDataRate(RF24_1MBPS);    // Tốc độ dữ liệu
  radio.setChannel(10);             // Đặt kênh
  radio.openReadingPipe(1,pipe);     
  radio.startListening();            
  pinMode(Led, OUTPUT);
  pinMode(Loa, OUTPUT);
  digitalWrite(Loa, HIGH);
  digitalWrite(Led, LOW);
}
 
void loop(){
  if (radio.available()){
    while (radio.available()){
      radio.read(&msg, sizeof(msg));
      if(Max < msg[0]) { Max = msg[0];}
      Serial.println(""); Serial.print(msg[0]); Serial.print(" - "); Serial.print(Max); Serial.print(" - "); Serial.print(Max + 1);
      if( msg[0] > 9) 
      { 
        Serial.println("Phat hien hut thuoc");
        for(int i=1; i<= 7 ; i++)
        {
             PlayMusic();
             NhayLed(); 
        }
        StopMusic();
        ////////////////
      }
      
    }
  }
}

///////////////////////////

void NhayLed()
{
  digitalWrite(Led, LOW);
  delay(1000);
  digitalWrite(Led, HIGH);
  delay(1000);
  
}

void PlayMusic()
{
  digitalWrite(Loa, LOW);
}

void StopMusic()
{
  digitalWrite(Loa, HIGH);
}
