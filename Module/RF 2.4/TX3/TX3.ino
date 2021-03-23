#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
 

 
RF24 SongRadio(9,10);
const uint64_t TanSo = 0xE8E8F0F0E1LL;

int MangChuoiGoi[1];

void setup(void)
{
  Serial.begin(9600);
  SongRadio.begin();
  SongRadio.openWritingPipe(TanSo);
}
void loop(void)
{
  String ChuoiGoi = "ccc nnn mmm!";
  int KichThuoc = ChuoiGoi.length();
  for (int i = 0; i < KichThuoc; i++) 
  {
    int charToSend[1];
    charToSend[0] = ChuoiGoi.charAt(i);
    SongRadio.write(charToSend,1);
  }  

  MangChuoiGoi[0] = 2; 
  SongRadio.write(MangChuoiGoi,1);
  
  Serial.print("Send: ");
  Serial.println(ChuoiGoi);
    
  SongRadio.powerDown(); 
  delay(1000);
  SongRadio.powerUp();
}
