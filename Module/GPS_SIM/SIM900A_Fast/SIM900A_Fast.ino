#include <EEPROM.h>
////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(A3, A2, A1, A0, 8, 12);

#define AutoReset    2

#define LCDLed       11
#define Relay        7
#define DenRelay_1   13
#define DenRelay_2   6

#define RF315_B      10
#define RF315_C      9
#define RF315_A      3

#define Sensor_1     5
#define Sensor_2     4

int TinHieu_1       = 0;
int TinHieu_2       = 0;
int TinHieu_3       = 0;
int TinHieu_4       = 0;
int TinHieu_5       = 0;

String  textSMS     = "Do Minh Tri";
String  phoneNumber = "+84908221404";
boolean HoatDong    = true;
////////////////////////////////////////////////////////////////////////////////////////// 
void setup()
{
    Serial.begin(9600);
  
    pinMode(RF315_B,  INPUT);
    pinMode(RF315_C,  INPUT);
    pinMode(RF315_A,  INPUT);

    pinMode(Sensor_1, INPUT);
    pinMode(Sensor_2, INPUT);
    
    pinMode(Relay,      OUTPUT);
    pinMode(LCDLed,     OUTPUT);
    pinMode(DenRelay_1, OUTPUT);
    pinMode(DenRelay_2, OUTPUT);
    pinMode(AutoReset,  OUTPUT);
    
    lcd.begin(16, 2); lcd.print("  -- Waiting --"); 
    digitalWrite(LCDLed, HIGH);
    ////////////////////  
    KhoiDongSim();  
    digitalWrite(DenRelay_1, HIGH); delay(20000); digitalWrite(DenRelay_1, LOW);
    ////////////////////
    lcd.clear(); lcd.begin(16, 2); 
}
////////////////////////////////////////////////////////////////////////////////////////// 
void loop()
{
   if(digitalRead(Sensor_1) == HIGH){ TinHieu_1++; } lcd.setCursor(0,0); lcd.print(TinHieu_1);  
   if(digitalRead(Sensor_2) == HIGH){ TinHieu_2++; } lcd.setCursor(4,0); lcd.print(TinHieu_2); 
   
   if(digitalRead(RF315_B) == HIGH){  TinHieu_3++; Nhayled(); } lcd.setCursor(0,1); lcd.print(TinHieu_3);  
   if(digitalRead(RF315_C) == HIGH){  TinHieu_4++; Nhayled(); } lcd.setCursor(4,1); lcd.print(TinHieu_4);
   if(digitalRead(RF315_A) == HIGH){  TinHieu_5++; Nhayled(); } lcd.setCursor(9,1); lcd.print(TinHieu_5);
   //////
   delay(300);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Nhayled()
{
  digitalWrite(DenRelay_1, HIGH);  
  delay(100);  
  digitalWrite(DenRelay_1, LOW); 
}
//////////////////////////////////////////////////////////////////////////////////////////
void GoiDien(String phonenumber)
{
  Serial.print("ATD"); 
  Serial.print(phonenumber);
  Serial.print(";");
  Serial.print("\r\n");
  delay(30000);            
  Serial.print("ATH\r\n");   
  KhoiDongSim();
}
//////////////////////////////////////////////////////////////////////////////////////////
void NhanTin(String message,String phonenumber)
{
  Serial.print("AT+CMGF=1\r\n");
  delay(500);
  Serial.print("AT+CMGS=\""); 
  Serial.print(phonenumber);
  Serial.print("\"\r");
  delay(500);
  // At this time, SIM900 will return '>'
  //SIM900_response(500);
  Serial.print(message); 
  Serial.print((char)26); // Ctrl+Z      
  Serial.flush();  // clear buffer 
  delay(5000); 
  ///////////////////////
  KhoiDongSim();
}
//////////////////////////////////////////////////////////////////////////////////////////
void  KhoiDongSim()
{
  digitalWrite(DenRelay_1, HIGH); delay(1000);
  digitalWrite(DenRelay_1, LOW);  delay(1000);
  digitalWrite(DenRelay_1, HIGH); delay(5000); // waiting for Finding network
  digitalWrite(DenRelay_1, LOW);
}
 

