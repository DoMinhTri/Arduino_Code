  

#define Relay1  8   
#define Relay2  9   
#define Relay3  10  
#define Relay4  11

#define LedS1  A2   
#define LedS2  7   
#define LedS3  2  
#define LedS4  4


#define Nut1    A1  
#define Nut2    A0 
#define Nut3    A3
#define Nut4    A4 

uint8_t TTG1    = 0;    
uint8_t TTG2    = 0; 
uint8_t TTG3    = 0; 
uint8_t TTG4    = 0;    


String ChuoiTTCT     = "";
uint8_t MangTTDen[4] = {0,0,0,0};

bool PhatHien_1   = false;    
bool PhatHien_2   = false; 
bool PhatHien_3   = false; 
bool PhatHien_4   = false; 
/////////////////////////
///   RESTART function
void RESTART(){ asm volatile (" jmp 0"); }  
/////////////////////////
#include <SoftwareSerial.h>
SoftwareSerial Wifi(12, 13); // RX, TX
/////////////////////////
void setup() 
{ 
  Wifi.begin(9600);
  Serial.begin(9600);
  pinMode(Nut1, INPUT);
  pinMode(Nut2, INPUT);
  pinMode(Nut3, INPUT);
  pinMode(Nut4, INPUT);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  pinMode(LedS1, OUTPUT);
  pinMode(LedS2, OUTPUT);
  pinMode(LedS3, OUTPUT);
  pinMode(LedS4, OUTPUT);

}
////////////////////////////////////////
String ChuoiData = "";

void loop() 
{

  while (Wifi.available() > 0 ) 
  {
      char KyTu = Wifi.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiData+= KyTu; } delay(5);
  }
  if(ChuoiData !="")
  {
      SuLyChuoiTinHieu(ChuoiData);
      ChuoiData = "";
  }
  ///////////////////////
  SuLyNut_1();
  SuLyNut_2();
  SuLyNut_3();
  SuLyNut_4();
  ///////////////////////
  BatDen();
  delay(500);
}
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
void SuLyChuoiTinHieu(String ChuoiData)
{
  String CNhanDang  = ChuoiData.substring(1,3);
  if(CNhanDang == "CT")
  {
        uint8_t TT1  = ChuoiData.substring(5,6).toInt();
        uint8_t TT2  = ChuoiData.substring(7,8).toInt();
        uint8_t TT3  = ChuoiData.substring(9,10).toInt();
        uint8_t TT4  = ChuoiData.substring(11,12).toInt();
        MangTTDen[0] = TT1;
        MangTTDen[1] = TT2;
        MangTTDen[2] = TT3;
        MangTTDen[3] = TT4;
  }
  else
  {
      if(ChuoiData == "MCU_RESTART") RESTART();  
  }
}
/////////////////////////////////////
void BatDen()
{
    digitalWrite(Relay1, MangTTDen[0]);   digitalWrite(LedS1, MangTTDen[0]);  
    digitalWrite(Relay2, MangTTDen[1]);   digitalWrite(LedS2, MangTTDen[1]);  
    digitalWrite(Relay3, MangTTDen[2]);   digitalWrite(LedS3, MangTTDen[2]);  
    digitalWrite(Relay4, MangTTDen[3]);   digitalWrite(LedS4, MangTTDen[3]);  

    ChuoiTTCT = String(MangTTDen[0]) + "," +  String(MangTTDen[1]) +  "," +  String(MangTTDen[2]) +  "," +  String(MangTTDen[3]);
    Serial.println(ChuoiTTCT);
}

/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
void SuLyNut_1()
{
    ///////////////////////
  TTG1 = digitalRead(Nut1);
  if( TTG1 == 0) PhatHien_1 = true;
  if(PhatHien_1)
  { 
      if(TTG1 == 1)
      {
          MangTTDen[0] = (MangTTDen[0] + 1)%2;
          PhatHien_1   = false;
      }
  }
}
/////////////////////////////////////
void SuLyNut_2()
{
  TTG2 = digitalRead(Nut2);
  if( TTG2 == 0) PhatHien_2 = true;
  if(PhatHien_2)
  { 
      if(TTG2 == 1)
      {
          MangTTDen[1] = (MangTTDen[1] + 1)%2;
          PhatHien_2   = false;
      }
  }
}
/////////////////////////////////////
void SuLyNut_3()
{
  TTG3 = digitalRead(Nut3);
  if( TTG3 == 0) PhatHien_3 = true;
  if(PhatHien_3)
  { 
      if(TTG3 == 1)
      {
          MangTTDen[2] = (MangTTDen[2] + 1)%2;
          PhatHien_3   = false;
      }
  }
}
/////////////////////////////////////
void SuLyNut_4()
{
  TTG4 = digitalRead(Nut4);
  if( TTG4 == 0) PhatHien_4 = true;
  if(PhatHien_4)
  { 
      if(TTG4 == 1)
      {
          MangTTDen[3] = (MangTTDen[3] + 1)%2;
          PhatHien_4   = false;
      }
  }
}

