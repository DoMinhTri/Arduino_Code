
#include <SoftwareSerial.h>
SoftwareSerial ESP(13,12);    // TX - RX
SoftwareSerial ATTiny13(5,6); // TX - RX

void setup()
{
  Serial.begin(9600);
  ESP.begin(9600);
  ATTiny13.begin(9600);
}
///////////////////////////////////////////////////////////////////////////////////////
String ChuoiDuLieu = "";

void loop(void)
{
//  Serial.print("1"); delay(1000); Serial.print("2"); delay(1000);
//  Serial.print("3"); delay(1000); Serial.print("4"); delay(1000);
//  Serial.print("5"); delay(1000); Serial.print("6"); delay(1000);
//  Serial.print("7"); delay(1000); Serial.print("8"); delay(1000);
  
  ATTiny13.print("1\n"); delay(1000); ATTiny13.print("2\n"); delay(1000);
  ATTiny13.print("3"); delay(1000); ATTiny13.print("4"); delay(1000);
  ATTiny13.print("5\n"); delay(1000); ATTiny13.print("6\n"); delay(1000);
  ATTiny13.print("7\n"); delay(1000); ATTiny13.print("8\n"); delay(1000);
   ///////////////////////////
//   while (ESP.available() > 0 ) 
//   {
//         char KyTu = ESP.read();
//         if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu+= KyTu; }
//         delay(5);
//   }   
//   ///////////////////////////
//   if(ChuoiDuLieu !="")
//   { 
//      String  aa = ChuoiDuLieu.substring(14, 15);
//      String  bb = ChuoiDuLieu.substring(16, 17);
//      String  cc = ChuoiDuLieu.substring(18, 19);
//      String  dd = ChuoiDuLieu.substring(20, 21);
//      Serial.print(aa); delay(500);
//      Serial.print(bb); delay(500);
//      Serial.print(cc); delay(500);
//      Serial.print(dd); delay(500);
//      ChuoiDuLieu = "";  
//   }
   /////////////////////////
}

