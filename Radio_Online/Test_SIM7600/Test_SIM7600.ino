//#include "SoftwareSerial.h"
//
//#define SIM800_RX_PIN 10
//#define SIM800_TX_PIN 11
//SoftwareSerial mySerial(SIM800_TX_PIN, SIM800_RX_PIN);

#define MODEM_RX 16
#define MODEM_TX 17

#define mySerial Serial2


void setup() 
{
  Serial.begin(115200);
  delay(500);
  mySerial.begin(115200);
  delay(500);
   mySerial.println("ATI");
  delay(100);
  mySerial.println("AT+CSQ");
  delay(100);
  mySerial.println("AT+CIMI");
  delay(100);
}
///////////////////////////
boolean NewLine  = false;
String ChuoiData = "", ChuoiData2 = "";
void loop() 
{
  if (mySerial.available()){ Serial.write(mySerial.read()); }
  //////////////////
//  while (mySerial.available() > 0 ) 
//  {
//      char KyTu = mySerial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
//      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiData2+= KyTu; } delay(5);
//  }
//  //////////////////
//  if(ChuoiData2 != ""){ Serial.println(ChuoiData2); ChuoiData2 = ""; }
  //////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiData+= KyTu; } delay(5);
  }
  //////////////////
  if(ChuoiData != ""){ mySerial.println(ChuoiData); ChuoiData = ""; }
  //////////////////

  //////////////////
}
  
