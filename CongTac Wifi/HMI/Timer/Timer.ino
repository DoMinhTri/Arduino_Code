
#include <Wire.h>              
#include <RtcDS3231.h>         
RtcDS3231<TwoWire> DS3231(Wire); 
time_t Nam,Thang,Ngay,Gio,Phut,Giay;
//////////////////////////////////////////////////////////////////////////////////


void setup() {

  Serial.begin(9600);  delay(100);
  //////////////////////////////
  DS3231.Begin();  
  TGHienTai();
  CaiDatThoiGian();  
}
////////////////////////////////////////////////////////////////////////////////////
void loop() 
{

  LayThoiGian();
  XemTG();
  delay(1000);
}
//////////////////////////////////////////////////////////////////////////////////
void LayThoiGian()
{
  RtcDateTime ThoiGian = DS3231.GetDateTime();   
  Nam   =  ThoiGian.Year();    
  Thang =  ThoiGian.Month();   
  Ngay  =  ThoiGian.Day();     
  Gio   =  ThoiGian.Hour();    
  Phut  =  ThoiGian.Minute(); 
  Giay  =  ThoiGian.Second(); 
}

void CaiDatThoiGian()
{
  RtcDateTime  TG = RtcDateTime(Nam, Thang, Ngay, Gio, Phut, Giay);
  DS3231.SetDateTime(TG); 
}

void XemTG()
{
  Serial.println("");
  Serial.print(Nam);
  Serial.print("/");
  Serial.print(Thang);
  Serial.print("/");
  Serial.print(Ngay);
  Serial.print(" - ");
  Serial.print(Gio);
  Serial.print(":");
  Serial.print(Phut);
  Serial.print(":");
  Serial.print(Giay);
}



void TGHienTai()
{
  Nam   =  2018;    
  Thang =  6;   
  Ngay  =  16;     
  Gio   =  3;    
  Phut  =  13; 
  Giay  =  30;
}

