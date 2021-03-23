
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xDEDEDEDEE7LL, 0xDEDEDEDEE9LL };

#define MaxChar 7
#define DenLed  7

char ViTriTram     = '3';
static int ThuTu   = 0 ;
boolean GuiDi      = false; 
boolean ChuyenTiep = false; 

char MangGui[MaxChar]  = "";
char MangLoad[MaxChar] = "";
char MangTam[MaxChar]  = "";
/////////////////////////////////////////////////////////////////////////////////////////
void setup(void) {

  //Serial.begin(9600);
  pinMode(DenLed, OUTPUT);
  //digitalWrite(DenLed, LOW);
  
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
}
/////////////////////////////////////////////////////////////////////////////////////////
void loop(void) 
{
  NhanTinHieu();
  GuiTinHieu();
  
  if(ChuyenTiep) { NhayDen(); SetGiaTriVaGui(); }
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
      //Serial.print("R:"); Serial.print(MangLoad); Serial.println();
      KiemTraTinHieuNhan();
      MangLoad[0] = 0;  // Clear the buffers
  }  
}
/////////////////////////////////////////////////////////////////////////////////////////
void KiemTraTinHieuNhan()
{
  int TangPhat   = (int)MangLoad[3];
  int PhongPhat  = (int)MangLoad[4];
  int TangNhan   = (int)MangLoad[5];
  int PhongNhan  = (int)MangLoad[6];
  int TramTruyen = (int)MangLoad[7];
  ///////////
  if( TangPhat > TangNhan)
  {
      if( (int)ViTriTram <= TramTruyen ) 
      { 
          ChuyenTiep = true;   //Serial.println("Down");
      }
  }
  ///////////
  if( TangPhat < TangNhan)
  {
      if( (int)ViTriTram >= TramTruyen )
      { 
          ChuyenTiep = true;   //Serial.println("Up");
      }
  }
  ///////////
}

/////////////////////////////////////////////////////////////////////////////////////////
void SetGiaTriVaGui()
{
  MangTam[0] = 'T';
  MangTam[1] = 'E';
  MangTam[2] = 'X';
  MangTam[3] = MangLoad[3];
  MangTam[4] = MangLoad[4];
  MangTam[5] = MangLoad[5];
  MangTam[6] = MangLoad[6];
  MangTam[7] = ViTriTram  ;
  GuiDi = true;
  ThuTu = 8;
  ///
  ChuyenTiep = false;
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
        // Serial.print("S:"); Serial.print(MangGui); Serial.println();
        MangGui[0] = 0;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void NhayDen()
{
    digitalWrite(DenLed, HIGH);
    delay(300);
    //digitalWrite(DenLed, LOW);
}

