#include <EEPROM.h>
void LuuEEPROM(int nDiaChi, uint8_t nGiaTri)
{
   EEPROM.write(nDiaChi, nGiaTri);
   EEPROM.commit();
   delay(20);
}
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, uint8_t SoLuu){ LuuEEPROM(DiaChi, SoLuu); }
//////////////////////////////////////////////////////////////////////////////////
uint8_t EEPROM_RN(int DiaChi){ return EEPROM.read(DiaChi); }


#define Led1 16
#define Led2 14
#define Led3 12
#define Led4 13

#define Led5 11
#define Led6 7
#define Led7 9
#define Led8 10
#define Led9 8
#define Led10 6

#define Led11 15
#define Led12 2
#define Led13 4
#define Led14 5
//////////////////////////////////////////////////
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////
void setup() {

  EEPROM.begin(512);
  Serial.begin(9600);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  //pinMode(Led5, OUTPUT);
  //pinMode(Led6, OUTPUT);
  //pinMode(Led7, INPUT_PULLUP);
  pinMode(Led8, INPUT);
  //pinMode(Led9, OUTPUT);
  //pinMode(Led10, OUTPUT);
  pinMode(Led11, OUTPUT);
  pinMode(Led12, OUTPUT);
  pinMode(Led13, OUTPUT);
  pinMode(Led14, OUTPUT);
  //pinMode(Led15, OUTPUT);

}
//////////////////////////////////////////////////
String GiaTri    = "";
uint8_t LedChon  = -1;
bool LoadLanDau  = true; 
void loop() {
  uint8_t TH = digitalRead(Led8);
  Serial.println("");   Serial.print("TH: ");    Serial.print(TH);
  ////////////////////////////
  if(LoadLanDau) { LedChon = EEPROM_RN(1); LoadLanDau = false; }
  ////////////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ GiaTri+= KyTu; } delay(5);
  }
  ////////////////////////////
  if(GiaTri != "")
  {
     uint8_t So = GiaTri.toInt();
     Serial.println("");   Serial.print("Gia tri luu: ");    Serial.print(So);
     EEPROM_WN(1,So);
     Serial.println("");   Serial.println("RESTART");
     Restart();
  }
  ////////////////////////////
  if(LedChon >= 0)
  {
      if(LedChon <17) NhayLed(LedChon);
  }
   ////////////////////////////
}
//////////////////////////////////////////////////


void NhayLed(uint8_t Pin)
{
    Serial.println("");
    Serial.print("Pin: ");
    Serial.print(Pin);
    digitalWrite(Pin, HIGH);  delay(1000);
    digitalWrite(Pin, LOW);   delay(1000);
}

