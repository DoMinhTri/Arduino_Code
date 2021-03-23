
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9,10);
/////////////////////////////////////////////////
const uint64_t pipes[2] = { 0xDEDEDEDEE7LL, 0xDEDEDEDEE9LL };
#define MaxChar 7
static int ThuTu = 0;
boolean GuiDi = false; 
char MangGui[MaxChar]  = "";
char MangLoad[MaxChar] = "";
char MangTam[MaxChar]  = "";
/////////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 6, 8, 7, 4, 3);
/////////////////////////////////////////////////
#define NutChon A3
#define NutOk   A2
#define TinHieu A1
#define Chuong  2
/////////////////////////////////////////////////
int SoTinHieu = 0;
int MangTinHieu[5]     = {0,0,0,0,0};
String MangTinHieuY[5] = {"Phong_Y1","Phong_Y2","Phong_Y3","Phong_Y4","Phong_Y5"};
String MangTinHieuN[5] = {"Phong_N1","Phong_N2","Phong_N3","Phong_N4","Phong_N5"};
String MangTinHieuK[5] = {"Phong_K1","Phong_K2","Phong_K3","Phong_K4","Phong_K5"};
String MangTinHieuD[5] = {"Phong_D1","Phong_D2","Phong_D3","Phong_D4","Phong_D5"};
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void setup(void) 
{
    Serial.begin(9600);
    ////////////////////  IN - OUT
    pinMode(NutChon, INPUT_PULLUP);
    pinMode(NutOk,   INPUT_PULLUP);
    pinMode(Chuong,  OUTPUT);
    pinMode(TinHieu, OUTPUT);
    //////////////////////////    nRF 2.4
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(70);
  
    radio.enableDynamicPayloads();
    radio.setRetries(15,15);
    radio.setCRCLength(RF24_CRC_16);

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);  
  
    radio.startListening();
    radio.printDetails();

    delay(200);
    serialEvent() ;
    //////////////////////////    
    lcd.begin(16, 2);
}
/////////////////////////////////////////////////////////////////////////////////////////
char KyTu = 'B'; 
String Chuoi_1,Chuoi_2;

void loop(void) 
{
    NhanTinHieu();
    GuiTinHieu();
    KyTu = SuLyNutBam();
    PhanHoiTinHieu(KyTu);
    //////////////////
}
/////////////////////////////////////////////////////////////////////////////////////////
char SuLyNutBam()
{
    int  BamCHON = digitalRead(NutChon);
    int  BamOK   = digitalRead(NutOk);
    if(BamCHON == 0) { delay(200); return 'S';}
    if(BamOK   == 0) { delay(200); return 'K';}
    return 'B';
}
/////////////////////////////////////////////////////////////////////////////////////////
void PhanHoiTinHieu(char BNut)
{
    lcd.setCursor(11, 0);
    lcd.print("Total");
    lcd.setCursor(13, 1);
    lcd.print(SoTinHieu);
    //delay(1000);
}
/////////////////////////////////////////////////////////////////////////////////////////
void SuLyTinHieu(String Chuoi)
{
    int THTam = 0;
    for(int i=0;i<5;i++)
    {
      if(Chuoi == MangTinHieuY[i]) { MangTinHieu[i] = 1;  SendGiaTri(MangTinHieuK[i]);  RungChuong(); }
      if(Chuoi == MangTinHieuN[i]) { MangTinHieu[i] = 0;  SendGiaTri(MangTinHieuD[i]);                }
      if(MangTinHieu[i] == 1) THTam++;
    }  
    SoTinHieu = THTam;
    TinHieuDen(SoTinHieu);
    //////////////////
}

/////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////    nRF 2.4       ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////  
void serialEvent() 
{
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();
      MangTam[ThuTu] = KyTu;
      ThuTu++;
      if (ThuTu > MaxChar) { GuiDi = true; break; }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void SendGiaTri(String Chuoi)
{
  Chuoi.toCharArray(MangTam, MaxChar+2);
  GuiDi = true;
  ThuTu = 8;
}
/////////////////////////////////////////////////////////////////////////////////////////
void NhanTinHieu(void)
{
  int DoDai = 0;
  if ( radio.available() ) 
  {
      boolean KetThuc = false;
      while ( !KetThuc ) 
      {
        DoDai = radio.getDynamicPayloadSize();
        KetThuc = radio.read(&MangLoad,DoDai);
        delay(5);
      }
      MangLoad[DoDai] = 0; // null terminate string
      ///////////////////
      SuLyTinHieu(MangLoad);
      Serial.print("R:");
      Serial.print(MangLoad);
      Serial.println();
      MangLoad[0] = 0;  // Clear the buffers
  }  
}
/////////////////////////////////////////////////////////////////////////////////////////
void GuiTinHieu(void)
{
  if (GuiDi) 
  { 
        strcat(MangGui,MangTam);      
        // swap TX & Rx addr for writing
        radio.openWritingPipe(pipes[1]);
        radio.openReadingPipe(0,pipes[0]);  
        radio.stopListening();
        boolean ok = radio.write(&MangGui,strlen(MangGui));
        ThuTu = 0;
        GuiDi = false;
        // restore TX & Rx addr for reading       
        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1,pipes[1]); 
        radio.startListening();  
        ///////////////////////
        Serial.print("S:");
        Serial.print(MangGui);          
        Serial.println();
        MangGui[0] = 0;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////
void TinHieuDen(int nTinHieu)
{
    if(nTinHieu > 0)
      digitalWrite(TinHieu, HIGH);
    else
      digitalWrite(TinHieu, LOW);
}
/////////////////////////////////////////////////////////////////////////////////////////
void RungChuong()
{
    digitalWrite(Chuong, HIGH);
    delay(1500);
    digitalWrite(Chuong, LOW);
}
/////////////////////////////////////////////////////////////////////////////////////////
void NhayDen()
{
    digitalWrite(TinHieu, LOW);
    delay(500);
    digitalWrite(TinHieu, HIGH);
}

/////////////////////////////////////////////////////////////////////////////////////  
////////////////////////////    LCD     /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////   
void XoaDong_1(){ lcd.setCursor(0, 2); lcd.print("                "); }
//////////////////////////////////////////////////////////////////////////////////////  
void XoaDong_2(){ lcd.setCursor(0, 1); lcd.print("                "); }
//////////////////////////////////////////////////////////////////////////////////////  
void LCD2Dong(String Dong_1, String Dong_2)
{
   lcd.begin(16, 2);
   lcd.print(Dong_1);
   lcd.setCursor(0, 1);
   lcd.print(Dong_2);
}
//////////////////////////////////////////////////////////////////////////////////////  
void NhapNhay(int ViTri, int Dong)
{
    lcd.setCursor(ViTri - 1, Dong - 1);
    lcd.cursor();
}
