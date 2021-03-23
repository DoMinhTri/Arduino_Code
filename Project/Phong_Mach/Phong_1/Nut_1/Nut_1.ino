
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xDEDEDEDEE7LL, 0xDEDEDEDEE9LL };

#define MaxChar 7
#define DenLed  6
#define NutBam  5
#define NutTat  4

static int ThuTu = 0;
boolean GuiDi = false; 

char MangGui[MaxChar]  = "";
char MangLoad[MaxChar] = "";
char MangTam[MaxChar]  = "";
/////////////////////////////////////////////////////////////////////////////////////////
void setup(void) {

  Serial.begin(9600);
  pinMode(NutBam, INPUT_PULLUP);
  pinMode(NutTat, INPUT_PULLUP);
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
  if(digitalRead(NutBam) == LOW) { SendGiaTri("Phong_Y1"); }
  //if(digitalRead(NutTat) == LOW) SendGiaTri("Phong_N1");
}
/////////////////////////////////////////////////////////////////////////////////////////
void SuLyTinHieu(String Chuoi)
{
    if(Chuoi == "Phong_K1") { digitalWrite(DenLed, LOW);  Serial.print(Chuoi); } // Ok nhan tin hieu
    if(Chuoi == "Phong_D1") { digitalWrite(DenLed, HIGH); Serial.print(Chuoi); } // Complete tin hieu
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void serialEvent() 
{
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();
      MangTam[ThuTu] = KyTu;
      ThuTu++;
      if (ThuTu > MaxChar) { GuiDi = true; break; }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void SendGiaTri(String Chuoi)
{
  Chuoi.toCharArray(MangTam, MaxChar+2);
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
      SuLyTinHieu(MangLoad);
      Serial.print("R:");
      Serial.print(MangLoad);
      Serial.println();
      MangLoad[0] = 0;  // Clear the buffers
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

