////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(10);

#define led    13

void setup()
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  lcd.begin(16,2);
  lcd.print("Khoi dong");
}

String ChuoiTinNhan = "";

void loop()
{
   while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read(); uint8_t nbKyTu = (int)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiTinNhan+= KyTu; }   
      delay(5);
   }
   if( ChuoiTinNhan != "") 
   { 
       lcd.print(ChuoiTinNhan);
       delay(2000);  
       ChuoiTinNhan = ""; 
       lcd.clear(); 
       lcd.begin(16,2); 
    }
}

