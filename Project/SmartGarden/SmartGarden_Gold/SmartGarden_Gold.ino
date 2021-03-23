




///////////////////////////////////////////
#include <SoftwareSerial.h>
SoftwareSerial Esp8266(6, 5); // RX, TX 
///////////////////////////////////////////
void setup() 
{
   
   Wire.begin();
   Esp8266.begin(38400);   // ESP8266
   ////////////////
   pinMode(RELAY, OUTPUT);
   pinMode(RESET, OUTPUT);
   pinMode(RESETWIFI, OUTPUT);
   
   ////////////////
   rtc.begin();
   ////////////////
 
   ////////////////


   LayMangTongThoiGianHen();
   ResetWifi();
}
//////////////////////////////////////////////////////////////////////////////
String ChuoiTinNhan  = "";
String ChuoiWifiData = "";
void loop() 
{

  KiemTraTuoiCay();
  KiemTraRelay();
  Desktop();
  /////////////
  while (Esp8266.available() > 0 ) 
  {
      char KyTu = Esp8266.read();  uint8_t nbKyTu = (int)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiWifiData+= KyTu; } delay(5);
  }
  /////////////
  if( ChuoiWifiData != "")
  { 
      SuLyGiaTriChuoiNhan(ChuoiWifiData);
      ChuoiWifiData = "";  
  }
  String sChuoiSS = "MG_SENSORDATA:" + (String)NhietDo + "," + (String)DoAmDat + "," + (String)TroiMua + "," + (String)ChoPhepTuoiCay;
  Esp8266.print(sChuoiSS);

  LayThoiGianHienTai();
  TinhNhietDoThoiGianThuc();
  delay(1000);
}

//////////////////////////////////////////////////////////////////////////////
void KiemTraRelay(){  if(ChoPhepTuoiCay){ digitalWrite(RELAY, HIGH); } else { digitalWrite(RELAY, LOW); } }
void ResetHeThong(){ digitalWrite(RESET, HIGH); }
void ResetWifi(){ digitalWrite(RESETWIFI, HIGH); delay(1000); digitalWrite(RESETWIFI, LOW); }
void ATCOMMAND(String AT)   
{ 
  if(AT == "1")  Esp8266.print("AT");
}
//////////////////////////////////////////////////////////////////////////////
void KiemTraTuoiCay()
{
    ChoPhepTuoiCay = false;
    uint8_t nGSoioHen = Load_SoLuongThoiDiemHen();
    for(uint8_t i=0; i<nGSoioHen; i++)
    {
        uint8_t nKieuTuoi = MangTGHen[1 + 9*i];
        uint8_t nGio      = MangTGHen[2 + 9*i];
        uint8_t nPhut     = MangTGHen[3 + 9*i];
        uint8_t nTGian    = MangTGHen[4 + 9*i];
        uint8_t nAmMax    = MangTGHen[5 + 9*i];
        uint8_t nAmMin    = MangTGHen[6 + 9*i];
        uint8_t nNhietMax = MangTGHen[7 + 9*i];
        uint8_t nNhietMin = MangTGHen[8 + 9*i];
        
        if(nGio == hour)
        {
            if(nPhut <= minute)
            { 
                if(minute < (nPhut + nTGian)){  ChoPhepTuoiCay = true; }
            }
            if(nKieuTuoi > 0)
            {
                if(DoAmDat >= nAmMax) { ChoPhepTuoiCay = false; }
                if(nKieuTuoi == 2)
                {
                    if(NhietDo > nNhietMax ) { ChoPhepTuoiCay = false; }
                    if(NhietDo < nNhietMin ) { ChoPhepTuoiCay = false; }
                }
            }
        }
        if(TroiMua > 75) { ChoPhepTuoiCay = false; }
    }
    KiemTraGioUuTien();
    if(ThoiGianTuoiUuTien > 0) { ChoPhepTuoiCay = true; }
}

//////////////////////////////////////////////////////////////////////////////////
void KiemTraGioUuTien()
{
  if(ThoiGianTuoiUuTien > 0)
  {
      if((millis()/1000 - ThoiDiemCaiDatUuTien) > ThoiGianTuoiUuTien){ ThoiGianTuoiUuTien = 0; }
  }
}
//////////////////////////////////////////////////////////////////////////////////
void CaiDatUuTien(String nTimer)
{
   ThoiGianTuoiUuTien   = nTimer.toInt()*60;
   ThoiDiemCaiDatUuTien = millis()/1000;
}
//////////////////////////////////////////////////////////////////////////////////
void BatTatRelay(String sRelay)
{
  String ON = sRelay.substring(0, 2);
  if(ON == "ON"){ digitalWrite(RELAY, HIGH); delay(5000); }
  else { digitalWrite(RELAY, LOW);}
}
//////////////////////////////////////////////////////////////////////////////////
void ThoiGianClearAll(){ Luu_SoLuongThoiDiemHen(0); }

