#include <SD.h>
#define SD_ChipSelectPin 10  


#include <TMRpcm.h>    
TMRpcm wavplayer; 


 
 

void setup()
{
 Serial.begin(9600);
 /////////////////
 wavplayer.speakerPin = 9; // 11 on Mega, 9 on Uno, Nano, etc
 /////////////////
 if (!SD.begin(SD_ChipSelectPin)) {  return; }
 wavplayer.volume(10);
}

//////////////////////////////////////////////////
String ChuoiDuLieu;
void loop()
{  
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
   if(ChuoiDuLieu != "")
   {
      if(ChuoiDuLieu == "1"){ wavplayer.play("1.wav"); }
      if(ChuoiDuLieu == "2"){ wavplayer.play("2.wav"); }
      if(ChuoiDuLieu == "3"){ wavplayer.play("3.wav"); }
      if(ChuoiDuLieu == "4"){ wavplayer.play("4.wav"); }
      if(ChuoiDuLieu == "5"){ wavplayer.play("5.wav"); }
      if(ChuoiDuLieu == "6"){ wavplayer.play("6.wav"); }
      if(ChuoiDuLieu == "7"){ wavplayer.play("7.wav"); }
      Serial.println(ChuoiDuLieu);
      ChuoiDuLieu = "";
   }
}

