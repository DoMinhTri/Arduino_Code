
bool  DeviceUpdate     = 0;
bool SendDataServer    = false;
String ChuoiTTCT       = "";
uint8_t MangTTDen[4]   = {0,0,0,0};
/////////////////////////
#define Relay1  8   
#define Relay2  9   
#define Relay3  10  
#define Relay4  11
/////////////////////////
#define Led1  7   
#define Led2  2   
#define Led3  4  
#define Led4  A2
/////////////////////////
#define Nut1    A1  
#define Nut2    A0 
#define Nut3    A3
#define Nut4    A4 
/////////// Trang Thai Cong Tac
uint8_t TTG1     = 0;    
uint8_t TTG2     = 0; 
uint8_t TTG3     = 0; 
uint8_t TTG4     = 0;   
/////////// Thoi Gian Nhan Nut
uint16_t TGNN1   = 0;    
uint16_t TGNN2   = 0; 
uint16_t TGNN3   = 0; 
uint16_t TGNN4   = 0; 
/////////// Phat hien nhan nut
bool PhatHien_1  = false;    
bool PhatHien_2  = false; 
bool PhatHien_3  = false; 
bool PhatHien_4  = false; 
/////////// Su ly sau khi nhan nut va tha ra
bool DaSuLy_1    = false;    
bool DaSuLy_2    = false; 
bool DaSuLy_3    = false; 
bool DaSuLy_4    = false; 
/////////////////////////
///   RESTART function
void RESTART(){Serial.println("RESTART MCU"); delay(1000); asm volatile (" jmp 0"); }  
/////////////////////////
#include <SoftwareSerial.h>
SoftwareSerial Wifi(13, 12); // RX, TX
/////////////////////////
uint32_t ThoiGianThuc = 0;
void ResetTuDong()
{
   ThoiGianThuc = millis()/1000;
   uint8_t nTTCT = 0;
   nTTCT += MangTTDen[0];
   nTTCT += MangTTDen[1];
   nTTCT += MangTTDen[2];
   nTTCT += MangTTDen[3];
   if(ThoiGianThuc > 3600) { if(nTTCT == 0) RESTART(); }
}
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

  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
}
////////////////////////////////////////
String ChuoiData = "";

void loop() 
{
  ResetTuDong();
  ////////////
  SuLyNut_1();
  SuLyNut_2();
  SuLyNut_3();
  SuLyNut_4();
  SendTTDenLenServer();
  ////////////
  Nhan4NutReset();
  ////////////
  while (Wifi.available() > 0 ) 
  {
      char KyTu = Wifi.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiData+= KyTu; } delay(5);
  }
  if(ChuoiData !="")
  {
      Serial.println(ChuoiData);
      SuLyChuoiTinHieu(ChuoiData);
  }
  ///////////////////////
  BatDen();
  ChuoiData = "";
  delay(300);
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
        if(DeviceUpdate == 1) KiemTraDataOnline( TT1,  TT2,  TT3,  TT4);
        if(DeviceUpdate == 0)
        {
            MangTTDen[0] = TT1;
            MangTTDen[1] = TT2;
            MangTTDen[2] = TT3;
            MangTTDen[3] = TT4;
        }
  }
  else
  {
      if(ChuoiData == "MCU_RESTART") RESTART();  
  }
}
/////////////////////////////////////
void BatDen()
{
    digitalWrite(Relay1, MangTTDen[0]); 
    digitalWrite(Led1,   MangTTDen[0]);
    
    digitalWrite(Relay2, MangTTDen[1]); 
    digitalWrite(Led2,   MangTTDen[1]);
     
    digitalWrite(Relay3, MangTTDen[2]); 
    digitalWrite(Led3,   MangTTDen[2]);
    
    digitalWrite(Relay4, MangTTDen[3]); 
    digitalWrite(Led4,   MangTTDen[3]);
}
/////////////////////////////////////
void SendTTDenLenServer()
{
    ChuoiTTCT = String(MangTTDen[0]) + "," +  String(MangTTDen[1]) +  "," +  String(MangTTDen[2]) +  "," +  String(MangTTDen[3]) ;
    Serial.println(ChuoiTTCT);
    if( DeviceUpdate == 1)  
    { 
       if(SendDataServer == false) 
       {
          SendDataServer = true;
          Wifi.print(ChuoiTTCT);  
          Serial.print("Send: ");  Serial.println(ChuoiTTCT);
       }
    }
}
/////////////////////////////////////
void KiemTraDataOnline(uint8_t TT1, uint8_t TT2, uint8_t TT3, uint8_t TT4)
{
    if(TT1 == MangTTDen[0])
    {
        if(TT2 == MangTTDen[1])
        {
            if(TT3 == MangTTDen[2])
            {
                if(TT4 == MangTTDen[3]) DeviceUpdate = 0;
            }
        }
    }
}
/////////////////////////////////////
/////////////////////////////////////
/////////////////////////////////////
void SuLyNut_1()
{
  TTG1 = digitalRead(Nut1);
  //Serial.println(TTG1);
  ///////////////////////
  if(TTG1 > 0)
  {
      if(PhatHien_1 == false) {  PhatHien_1 = true;  TGNN1 = millis()/1000;  }
      if(( millis()/1000 - TGNN1 ) > 1)
      {
        MangTTDen[0] = (MangTTDen[0] + 1)%2;
        PhatHien_1   = false;
        DaSuLy_1     = true;
        DeviceUpdate = 1;
        SendDataServer = false;
       //Serial.println("11");
      }
      //////
  }
  else
  {
        ///////////
        if(PhatHien_1)
        { 
           if(DaSuLy_1 == false)
           {
                MangTTDen[0] = (MangTTDen[0] + 1)%2;
                PhatHien_1   = false;
                DeviceUpdate = 1;
                SendDataServer = false;
                //Serial.println("12");
           }
           DaSuLy_1   = false;
           PhatHien_1 = false;
        }
  }
}
/////////////////////////////////////
void SuLyNut_2()
{
  TTG2 = digitalRead(Nut2);
  ///////////////////////
  if(TTG2 > 0)
  {
      if(PhatHien_2 == false) {  PhatHien_2 = true;  TGNN2 = millis()/1000;   }
      if(( millis()/1000 - TGNN2 ) > 1)
      {
        MangTTDen[1] = (MangTTDen[1] + 1)%2;
        PhatHien_2   = false;
        DaSuLy_2     = true;
        DeviceUpdate = 1;
        SendDataServer = false;
        //Serial.println("21");
      }
  }
  else
  {
        
        if(PhatHien_2)
        { 
           if(DaSuLy_2 == false)
           {
                MangTTDen[1] = (MangTTDen[1] + 1)%2;
                PhatHien_2   = false;
                DeviceUpdate = 1;
                SendDataServer = false;
                //Serial.println("22");
           }
           DaSuLy_2   = false;
           PhatHien_2 = false;
        }
  }
}
/////////////////////////////////////
void SuLyNut_3()
{
  TTG3 = digitalRead(Nut3);
  ///////////////////////
  if(TTG3 > 0)
  {
      if(PhatHien_3 == false) {  PhatHien_3 = true;  TGNN3 = millis()/1000;   }
      if(( millis()/1000 - TGNN3 ) > 1)
      {
        MangTTDen[2] = (MangTTDen[2] + 1)%2;
        PhatHien_3   = false;
        DaSuLy_3     = true;
        DeviceUpdate = 1;
        SendDataServer = false;
        //Serial.println("31");
      }
  }
  else
  {
        
        if(PhatHien_3)
        { 
           if(DaSuLy_3 == false)
           {
                MangTTDen[2] = (MangTTDen[2] + 1)%2;
                PhatHien_3   = false;
                DeviceUpdate = 1;
                SendDataServer = false;
                //Serial.println("32");
           }
           DaSuLy_3   = false;
           PhatHien_3 = false;
        }
  }
}
/////////////////////////////////////
void SuLyNut_4()
{
  TTG4 = digitalRead(Nut4);
  ///////////////////////
  if(TTG4 > 0)
  {
      if(PhatHien_4 == false) {  PhatHien_4 = true;  TGNN4 = millis()/1000;   }
      if(( millis()/1000 - TGNN4 ) > 1)
      {
        MangTTDen[3] = (MangTTDen[3] + 1)%2;
        PhatHien_4   = false;
        DaSuLy_4     = true;
        DeviceUpdate = 1;
        SendDataServer = false;
        //Serial.println("41");
      }
  }
  else
  {
        
        if(PhatHien_4)
        { 
           if(DaSuLy_4 == false)
           {
                MangTTDen[3] = (MangTTDen[3] + 1)%2;
                PhatHien_4   = false;
                DeviceUpdate = 1;
                SendDataServer = false;
                //Serial.println("42");
           }
           DaSuLy_4   = false;
           PhatHien_4 = false;
        }
  }
}
//////////////////////////////////////////////////////////////////////////
void Nhan4NutReset()
{
  TTG1 = digitalRead(Nut1);
  TTG2 = digitalRead(Nut2);
  TTG3 = digitalRead(Nut3);
  TTG4 = digitalRead(Nut4);
  ///////////////////////
  if(TTG1 > 0){if(TTG2 > 0){if(TTG3 > 0){if(TTG4 > 0){ Wifi.print("ESP_RESTART"); delay(1000); RESTART(); }}}}
}

