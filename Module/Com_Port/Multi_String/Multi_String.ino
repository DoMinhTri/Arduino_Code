////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(A3, A2, A1, A0, 8, 12);

#define led          13
#define LCDLed       11
void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(LCDLed,     OUTPUT);
  digitalWrite(LCDLed, HIGH);
  digitalWrite(led, HIGH);

  lcd.begin(16,2);
  lcd.print("Khoi dong");
  
  delay(20000); // give time to log on to network.
  Serial.print("AT+CMGF=1\r"); // set SMS mode to text
  delay(100);
  Serial.print("AT+CNMI=2,2,0,0,0\r");
  // blurt out contents of new SMS upon receipt to the GSM shield's serial out
  delay(100);
  digitalWrite(led, LOW);
  lcd.clear(); lcd.begin(16,2);
  
}

int count=0;
unsigned char buffer[128];
String ChuoiTinNhan = "";

void loop()
{
   while (Serial.available() > 0 ) 
  {
       char KyTu = Serial.read();
       buffer[count] = KyTu;
       ChuoiTinNhan+= KyTu;
       count++;
       if(count == 128) break;
   }
   
   lcd.print(ChuoiTinNhan);
   Serial.println(ChuoiTinNhan);
   if( ChuoiTinNhan != "") 
   { 
       delay(5000);  
       ChuoiTinNhan = ""; 
       lcd.clear(); lcd.begin(16,2); 
       Serial.println("AT+CMGD=1,4"); // delete all SMS
    }
}

