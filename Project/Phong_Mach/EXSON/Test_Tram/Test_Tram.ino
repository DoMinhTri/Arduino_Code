
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xDEDEDEDEE7LL, 0xDEDEDEDEE9LL };

#define MaxChar 7
#define DenLed  6
#define NutBam  5


static int ThuTu = 0;
boolean GuiDi = false; 

char MangGui[MaxChar]  = "";
char MangLoad[MaxChar] = "";
char MangTam[MaxChar]  = "";
/////////////////////////////////////////////////////////////////////////////////////////
void setup(void) {

  Serial.begin(9600);
  pinMode(NutBam, INPUT_PULLUP);
  pinMode(DenLed, OUTPUT);
  digitalWrite(DenLed, HIGH);
  
  
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(70);
  
  radio.enableDynamicPayloads();
  radio.setRetries(15,15);
  radio.setCRCLength(RF24_CRC_16);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);  
  
  radio.startListening();
  radio.printDetails();

  delay(200);
  serialEvent() ;
}
/////////////////////////////////////////////////////////////////////////////////////////
void loop(void) 
{
  NhanTinHieu();
  GuiTinHieu();
  if(digitalRead(NutBam) == LOW) { SendGiaTri(); delay(200); }
}
/////////////////////////////////////////////////////////////////////////////////////////
void serialEvent() 
{
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();
      MangTam[ThuTu] = KyTu;
      ThuTu++;
      if (ThuTu > MaxChar) { GuiDi = true; ThuTu = 0; break; }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void SendGiaTri()
{
  MangTam[0] = 'T';
  MangTam[1] = 'E';
  MangTam[2] = 'X';
  MangTam[3] = '3';
  MangTam[4] = '1';
  MangTam[5] = '2';
  MangTam[6] = '1';
  MangTam[7] = '3';
  GuiDi = true;
  ThuTu = 8;
}
/////////////////////////////////////////////////////////////////////////////////////////
void NhanTinHieu(void)
{
  int DoDai = 0;
  if ( radio.available() ) 
  {
      boolean KetThuc = false;
      while ( !KetThuc ) 
      {
        DoDai = radio.getDynamicPayloadSize();
        KetThuc = radio.read(&MangLoad,DoDai);
        delay(5);
      }
      MangLoad[DoDai] = 0; // null terminate string
      ///////////////////
      Serial.print("R:");
      Serial.print(MangLoad);
      Serial.println();
      MangLoad[0] = 0;  // Clear the buffers
      NhayDen();
  }  
}
/////////////////////////////////////////////////////////////////////////////////////////
void GuiTinHieu(void)
{
  if (GuiDi) 
  { 
        strcat(MangGui,MangTam);      
        // swap TX & Rx addr for writing
        radio.openWritingPipe(pipes[1]);
        radio.openReadingPipe(0,pipes[0]);  
        radio.stopListening();
        boolean ok = radio.write(&MangGui,strlen(MangGui));
        ThuTu = 0;
        GuiDi = false;
        // restore TX & Rx addr for reading       
        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1,pipes[1]); 
        radio.startListening();  
        ///////////////////////
        Serial.print("S:");
        Serial.print(MangGui);          
        Serial.println();
        MangGui[0] = 0;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void NhayDen()
{
    digitalWrite(DenLed, LOW);
    delay(500);
    digitalWrite(DenLed, HIGH);
}

