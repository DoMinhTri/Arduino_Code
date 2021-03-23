// TRANSMITTER

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define ledPin 13
#define Button 5
RF24 radio(9,10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void)
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(Button, INPUT_PULLUP);
  Serial.begin(9600);
  
  
  radio.begin();
  radio.setDataRate(RF24_2MBPS); // RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(70);
  
  radio.enableDynamicPayloads();
  radio.setRetries(15,15);
  radio.setCRCLength(RF24_CRC_8);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);  
  
  radio.startListening();
  radio.printDetails();
  delay(500);
}
/////////////////////////////////////////////////////////////////
void loop(void)
{
  unsigned long DuLieuVao = DocDuLieu();
  if(DuLieuVao == 111) {   Serial.println(DuLieuVao);   NhayDen();  }
  //if(digitalRead(Button) == LOW)  
  GuiData(222);
  delay(1000);
}
///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
unsigned long DocDuLieu()
{
  delay(50);
  unsigned long  got_time;  
  if ( radio.available() )  
  {
    bool done = false;
    while (!done)
    {
      //done = radio.read( &got_time, sizeof(unsigned long ) ); 
    }
  }
  return(got_time);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void GuiData(int Data)
{
    boolean recived_data;  
    do{
      recived_data = KiemTraSend(Data);
    }while(recived_data == false);  
}
/////////////////////////////////////////////////////////////////
boolean KiemTraSend(unsigned long dataToSend)
{
  bool ok = radio.write( &dataToSend, sizeof(unsigned long ) );
  if (ok) { delay(200); return(true); }
  else    { return false;             }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void NhayDen()
{
    digitalWrite(ledPin,LOW);
    delay(200);
    digitalWrite(ledPin,HIGH);
    delay(200);
}



