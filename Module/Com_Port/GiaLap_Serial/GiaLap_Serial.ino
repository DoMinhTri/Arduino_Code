
#include <SoftwareSerial.h>
SoftwareSerial ESP(8,9); // TX - RX

void setup()
{
  Serial.begin(9600);
  ESP.begin(9600);
}
///////////////////////////////////////////////////////////////////////////////////////
String ChuoiDuLieu_1 = "";
String ChuoiDuLieu_2  = "";
boolean TraLoi = false;

void loop(void)
{
   ///////////////////////////
   while (Serial.available() > 0 ) 
   {
         char KyTu = Serial.read();
         if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu_1+= KyTu; }
         delay(5);
   }
   ///////////////////////////
   while (ESP.available() > 0 ) 
   {
         char KyTu = ESP.read();
         if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu_2+= KyTu; }
         delay(5);
   }   
   ///////////////////////////
   if(ChuoiDuLieu_1 !=""){ ESP.println(ChuoiDuLieu_1);   ChuoiDuLieu_1  = "";  }
   if(ChuoiDuLieu_2 !=""){ Serial.println(ChuoiDuLieu_2); ChuoiDuLieu_2 = "";  }
   /////////////////////////
}

