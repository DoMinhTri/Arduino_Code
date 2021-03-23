#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
 
RF24 SongRadio(9,10);
const uint64_t TanSo = 0xE8E8F0F0E1LL;

int MangChuoiThu[1];
int KichThuocChuoi = 12;
String NoiDung     = "";

void setup(void)
{
  Serial.begin(9600);
  SongRadio.begin();
  SongRadio.openReadingPipe(1,TanSo);
  SongRadio.startListening();
}

void loop(void)
{
  if (SongRadio.available())
  {
      bool done = false;  
      done = SongRadio.read(MangChuoiThu, 1); 
      char KyTu = MangChuoiThu[0];
      
      if (MangChuoiThu[0] != 2)
      {  
        NoiDung.concat(KyTu); 
      }
      else 
      {
         if (NoiDung.length() == KichThuocChuoi) 
         { 
           Serial.println(NoiDung);
         }
         
         NoiDung = ""; 
      }
   }
}
