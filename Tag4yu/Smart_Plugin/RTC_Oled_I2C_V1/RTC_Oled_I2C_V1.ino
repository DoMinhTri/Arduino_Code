#include <STM32RTC.h>
STM32RTC& rtc = STM32RTC::getInstance();

 byte seconds = 39;
 byte minutes = 32;
 byte hours   = 11;
 byte weekDay = 1;
 byte day     = 8;
 byte month   = 3;
 byte year    = 19;
///////////////////////////
#include <Wire.h>
#include <SeeedGrayOLED.h>
#include <avr/pgmspace.h>
//////////////////////////
#include "x_nucleo_nfc04.h"
#define NFCReader X_Nucleo_Nfc04 
//////////////////////////
#define Pin_Buzzer      A3
#define Pin_AC1_OnF     12
#define Pin_AC2_OnF     A0
#define Pin_Oled_OnF    10
#define Pin_NFC_Read    9

//////////////////////////
uint8_t nTThai, nGio, nPhut;
unsigned long TongSoGiay = 0;
                                                 // "1112321611120301504246"; // 1-1,00,02-16_1_1,12,03_0,15,04_2,4,6
String sVD         = "1100011611120301504246";   //  1234567891234567891234567   // 1-2_34,56-78_9_1,23,45_6,78,91_2345           
String ChuoiTGNDEF = sVD; 
uint8_t nOFHeThong, nOFCDown, nCDownGio, nCDownPhut;
uint8_t nDoDai, nKieuHen, nTTBDau, nGioBDau, nPhutBDau, nTTKT, nGioKT, nPhutKT;
char*   MangThu ;
uint8_t nPhanTu ;
boolean cdGio = false, cdPhut = false, cdGiay = false;
uint8_t tpGio, tpPhut, tpGiay;
////////////////////////////////////////////////////////////
void PhanTichChuoiThoiGian()
{
  String tpsTGNFC   = X_Nucleo_Nfc04.ReadText();
  uint8_t nSoKyTu   = tpsTGNFC.length();
  String sChuoiDate = "";
  String sChuoiTG   = "";
  String sChuoiThu  = "";
  if(tpsTGNFC.substring(0,1).toInt() == 1)
  {
    sChuoiDate = tpsTGNFC.substring(1,13);
    sChuoiTG   = tpsTGNFC.substring(13,nSoKyTu);
    ///////////////
    tpsTGNFC   = "0" + sChuoiDate + sChuoiTG;
    X_Nucleo_Nfc04.WriteText(tpsTGNFC);
    delay(100); 
    ///////////////
    SeeedGrayOled.clearDisplay(); 
    /////////////////////
    year    = sChuoiDate.substring(0,2).toInt();
    month   = sChuoiDate.substring(2,4).toInt();
    day     = sChuoiDate.substring(4,6).toInt();
    hours   = sChuoiDate.substring(6,8).toInt();
    minutes = sChuoiDate.substring(8,10).toInt();
    seconds = sChuoiDate.substring(10,12).toInt();
    weekDay = 14;
    rtc.setTime(hours, minutes, seconds);
    rtc.setDate(weekDay, day, month, year);
    /////////////////////
    nOFHeThong = sChuoiTG.substring(0,1).toInt();
    nOFCDown   = sChuoiTG.substring(1,2).toInt();
    nCDownGio  = sChuoiTG.substring(2,4).toInt();
    nCDownPhut = sChuoiTG.substring(4,6).toInt();
  
    nDoDai     = sChuoiTG.substring(6,8).toInt();
    nKieuHen   = sChuoiTG.substring(8,9).toInt();
    
    nTTBDau    = sChuoiTG.substring(9,10).toInt();
    nGioBDau   = sChuoiTG.substring(10,12).toInt();
    nPhutBDau  = sChuoiTG.substring(12,14).toInt();

    nTTKT      = sChuoiTG.substring(14,15).toInt();
    nGioKT     = sChuoiTG.substring(15,17).toInt();
    nPhutKT    = sChuoiTG.substring(17,19).toInt();

    nSoKyTu     = sChuoiTG.length();
    sChuoiThu   = sChuoiTG.substring(19,nSoKyTu);
    MangThu     = Str2Char(sChuoiThu);
    nPhanTu     = sChuoiThu.length();    
    TongSoGiay  = (nCDownGio*60 + nCDownPhut)*60;
  
 
    Serial.print(sChuoiTG);    Serial.println("");
    Serial.print(nOFHeThong);  Serial.print("-");
    Serial.print(nOFCDown);    Serial.print("_");
    Serial.print(nCDownGio);   Serial.print(",");
    Serial.print(nCDownPhut);  Serial.print("-");

    Serial.print(nDoDai);      Serial.print("_");
    Serial.print(nKieuHen);    Serial.print("_");

    Serial.print(nTTBDau);     Serial.print(",");
    Serial.print(nGioBDau);    Serial.print(",");
    Serial.print(nPhutBDau);   Serial.print("_");

    Serial.print(nTTKT);       Serial.print(",");
    Serial.print(nGioKT);      Serial.print(",");
    Serial.print(nPhutKT);     Serial.print("_");
    Serial.print(sChuoiThu);   Serial.print("(");
    Serial.print(nPhanTu);
    Serial.println(")");
    Serial.println("----------------------------------");
  }
  
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void CaiDatGPIO()
{
     pinMode(Pin_Buzzer,   OUTPUT);
     pinMode(Pin_AC1_OnF,  OUTPUT);
     pinMode(Pin_AC2_OnF,  OUTPUT);
     
     pinMode(Pin_Oled_OnF, INPUT_PULLUP);  // 0 = On   , 1 = Off
     pinMode(Pin_NFC_Read, INPUT_PULLUP);  // 0 = Read , 1 = unread
}
////////////////////////////////////////////////////////////
void CaiDatNFC()
{
  if(NFCReader.begin() == 0) 
  {
    Serial.println("System Init done!");
    HuCoi_1();
  }   
  else 
  {
    Serial.println("System Init failed!");
    HuCoi_2();
    //while(1);
  }
}

////////////////////////////////////////////////////////////
void CaiDatRTC()
{
    rtc.begin(); 
    rtc.setTime(hours, minutes, seconds);
    rtc.setDate(weekDay, day, month, year);
}
////////////////////////////////////////////////////////////
/////////////////   Display    /////////////////////////////
////////////////////////////////////////////////////////////
void CaiDatOled()
{
  Wire.begin();
  SeeedGrayOled.init(SH1107G);      // initialize SEEED OLED display
  SeeedGrayOled.clearDisplay();     // Clear Display.
  SeeedGrayOled.setNormalDisplay(); // Set Normal Display Mode
  SeeedGrayOled.setVerticalMode();  // Set to vertical mode for displaying text
  SeeedGrayOled.setGrayLevel(15);   // Set Grayscale level. Any number between 0 - 15.
}
////////////////////////////////////////////////////////////
void OledDisplay(uint8_t nX, uint8_t nY, String Noidung)
{
     SeeedGrayOled.setTextXY(nX , nY);               
     SeeedGrayOled.putString(Str2Char(Noidung)); 
}

////////////////////////////////////////////////////////////
void ClearDisplay()
{
     SeeedGrayOled.clearDisplay(); 
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  CaiDatGPIO();
  CaiDatRTC();
  CaiDatOled();
  CaiDatNFC();
  /////////////////
  AC1Off();
  AC2Off();
  ///////////////////
  
  Serial.println("Begin");
}
////////////////////////////////////////////////////////////
// uint8_t nOFHeThong, nOFCDown, nCDownGio, nCDownPhut;
// uint8_t nDoDai, nKieuHen, nTTBDau, nGioBDau, nPhutBDau, nTTKT, nGioKT, nPhutKT;
// char*   MangThu ;
// uint8_t nPhanTu ;

void loop()
{
  
  ////////////////////////
  uint8_t NFC_Read = digitalRead(Pin_NFC_Read);
  uint8_t Oled_OnF = digitalRead(Pin_Oled_OnF);
  if(Oled_OnF == 0) 
  {  

  PhanTichChuoiThoiGian();
  ////////////////////////
  String sngay =  "20" + String(rtc.getYear())      + "/" + SoHangChuc(rtc.getMonth())   + "/" + SoHangChuc(rtc.getDay()) ;
  String sgio  =         SoHangChuc(rtc.getHours()) + ":" + SoHangChuc(rtc.getMinutes()) + ":" + SoHangChuc(rtc.getSeconds());
  ////////////////////////
  if(nOFHeThong == 1)
  {
      OledDisplay(1,0,sngay);
      OledDisplay(2,0,sgio);
      ////////////////////////////////
      if(nOFCDown == 1)
      {
          if(TongSoGiay > 0)
          {
             AC2On();
             tpGio  = TongSoGiay/3600;
             tpPhut = (TongSoGiay - tpGio*3600)/60;
             tpGiay = TongSoGiay%60;
             String tpTGCD   =  TaoChuoiTG(tpGio, tpPhut,  tpGiay); 
             OledDisplay(4,0,"Countdown");
             OledDisplay(5,0, tpTGCD);
             if(TongSoGiay >0) TongSoGiay--;
          }
          else
          {
              AC2Off();
          }
          CDGio(tpGio);
          CDPhut(tpPhut);
          CDGiay(tpGiay);
      }

      ////////////////////////////////
      if(nOFCDown == 0)
      {
          String ChuoiT1 =  LayTTOnOff(nTTBDau) + " " + String(nGioBDau) + ":" + String(nPhutBDau);
          String ChuoiT2 =  LayTTOnOff(nTTKT)   + " " + String(nGioKT)   + ":" + String(nPhutKT);
          OledDisplay(3,0,"      Timer" );
          OledDisplay(4,0,ChuoiT1 );
          OledDisplay(5,0,ChuoiT2 );
          ////////////////
          if(nGioBDau == hours)
          {
            if(nPhutBDau == minutes)
            {
              if(nTTBDau == 1) AC2On();
              if(nTTBDau == 0) AC2Off();
            }
          }
          ////////////////
          if(nGioKT == hours)
          {
            if(nPhutKT == minutes)
            {
              if(nTTKT == 1) AC2On();
              if(nTTKT == 0) AC2Off();
            }
          }
          ////////////////
      }
  }
  else
  {
      OledDisplay(3,0,"Device Off ");
  }
   delay(1000);
  }
  else
  {
    SeeedGrayOled.clearDisplay(); 
  }
}
////////////////////////////////////////////////////////////
String LayTTOnOff(uint8_t TT)
{
    
    if(TT == 1)
      return "ON ";
    else
       return "OFF";
}
////////////////////////////////////////////////////////////
String TaoChuoiTG(uint8_t Gio, uint8_t Phut, uint8_t Giay)
{
   String tpChuoi = "";
   if(Gio  >0) { tpChuoi += SoHangChuc(Gio)  + ":" ; cdGio  = true ; }
   if(Phut >0) { tpChuoi += SoHangChuc(Phut) + ":" ; cdPhut = true ; }
   if(Giay >0) { tpChuoi += SoHangChuc(Giay)       ; cdGiay = true;  }
   return tpChuoi;
}

void CDGio(uint8_t Num) { if(Num <=0) {  if(cdGio)  {  cdGio  = false; ClearDisplay(); }}}
void CDPhut(uint8_t Num){ if(Num <=0) {  if(cdPhut) {  cdPhut = false; ClearDisplay(); }}}
void CDGiay(uint8_t Num){ if(Num <=0) {  if(cdGiay) {  cdGiay = false; ClearDisplay(); }}}
/////////////////////////
void AC1On() {  digitalWrite(Pin_AC1_OnF, LOW);  }
void AC1Off(){  digitalWrite(Pin_AC1_OnF, HIGH); }
void AC2On() {  digitalWrite(Pin_AC2_OnF, LOW);  }
void AC2Off(){  digitalWrite(Pin_AC2_OnF, HIGH); }
/////////////////////////
void HuCoi_1(){ digitalWrite(Pin_Buzzer, HIGH); delay(500);  digitalWrite(Pin_Buzzer, LOW); }
void HuCoi_2(){ HuCoi_1(); delay(500); HuCoi_1(); }
/////////////////////////
uint8_t StrInt(String _So)
{
  return _So.toInt();
}
////////////////////////////////////////////////////////////
char* Str2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
/////////////////////////
String SoHangChuc(uint8_t So)
{
    if(So >9)
        return String(So); 
    else
        return ("0" + String(So));
}
///////////////////////
/////////////////////
