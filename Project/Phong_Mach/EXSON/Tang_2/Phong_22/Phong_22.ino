
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xDEDEDEDEE7LL, 0xDEDEDEDEE9LL };

#define MaxChar 7
#define DenLed  6
#define NutBam  5

char Tang   = '2';
char Phong  = '2';

static int ThuTu = 0 ;
boolean GuiDi      = false; 

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

}
/////////////////////////////////////////////////////////////////////////////////////////
void loop(void) 
{
  NhanTinHieu();
  GuiTinHieu();
  if(digitalRead(NutBam) == LOW) { SetGiaTriVaGui(); BatDen(); delay(200); }
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
      Serial.print("R:"); Serial.print(MangLoad); Serial.println();
      KiemTraTinHieuNhan();
      MangLoad[0] = 0;  // Clear the buffers
  }  
}
/////////////////////////////////////////////////////////////////////////////////////////
void KiemTraTinHieuNhan()
{
  int TangNhan   = (int)MangLoad[5];
  int PhongNhan  = (int)MangLoad[6];
  ///////////
  if( TangNhan == (int)Tang)
  {
      if( PhongNhan == (int)Phong){ TatDen(); }
  }
  ///////////
}
/////////////////////////////////////////////////////////////////////////////////////////
void SetGiaTriVaGui()
{
  MangTam[0] = 'T';
  MangTam[1] = 'E';
  MangTam[2] = 'X';
  MangTam[3] = Tang  ;
  MangTam[4] = Phong ;
  MangTam[5] = '1';
  MangTam[6] = '1';
  MangTam[7] = Tang;
  GuiDi = true;
  ThuTu = 8;
}
/////////////////////////////////////////////////////////////////////////////////////////
void GuiTinHieu(void)
{
  if (GuiDi) 
  { 
        strcat(MangGui,MangTam);      
        radio.openWritingPipe(pipes[1]);
        radio.openReadingPipe(0,pipes[0]);  
        radio.stopListening();
        boolean ok = radio.write(&MangGui,strlen(MangGui));
        ThuTu = 0;
        GuiDi = false;
        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1,pipes[1]); 
        radio.startListening();  
        ///////////////////////
        Serial.print("S:"); Serial.println(MangGui);
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
/////////////////////////////////////////////////////////////////////////////////////////
void BatDen(){ digitalWrite(DenLed, LOW); }
/////////////////////////////////////////////////////////////////////////////////////////
void TatDen(){ digitalWrite(DenLed, HIGH);  }
