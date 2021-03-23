#include <EEPROM.h>
////////////////////////////////////////////
#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(10);
byte Symbol_BGT[8]      = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};
byte Symbol_BGD[8]      = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte Symbol_Mua[8]      = {B00100,B00100,B00100,B01110,B10001,B10101,B10001,B01110};
byte Symbol_Nang[8]     = {B00100,B10001,B01110,B11010,B01011,B01110,B10001,B00100};
byte Symbol_TuoiCay1[8] = {0b11011,0b11011,0b00000,0b10001,0b11011,0b11111,0b11111,0b11111};
byte Symbol_TuoiCay2[8] = {0b11111,0b11111,0b11111,0b11011,0b11011,0b00000,0b10001,0b11011};
////////////////////////////////////
uint8_t TuoiUuTien[4] = {0 , 0, 0, 0};
uint8_t TuoiCay[4]    = {1 , 0, 0, 1};
uint8_t DoAmDat[4]    = {0 , 0, 0, 0};

uint8_t SS1, SS2, SS3, SS4;
unsigned long ThoiDiemHienTai  = 0;
uint8_t NhietDo, SoThoiDiemHen = 0;
///////////////////////////////////////////
#include <Sodaq_DS3231.h>
#include <Wire.h>
uint8_t giay, phut, gio, ngay, thang, nam, thu; 
String ThuTrongTuanEn[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
String ThuTrongTuan[7]   = {"T2" , "T3" , "T4" , "T5" , "T6" , "T7" , "CN" };
///////////////////////////////////////////
#define PIN_RELAY1  7
#define PIN_RELAY2  8
#define PIN_RELAY3  6
#define PIN_RELAY4  5

#define PIN_SS1     A3
#define PIN_SS2     A2
#define PIN_SS3     A1
#define PIN_SS4     A0

void RESTART(){ asm volatile ("  jmp 0"); }
//////////////////////////////////////////////////////////////////////////////
void CapNhatThoiGian()
{
    DateTime now = rtc.now(); 
    giay  = now.second();
    phut  = now.minute();
    gio   = now.hour();
    ngay  = now.date();
    thang = now.month();
    nam   = now.year() - 2000;
    thu   = now.dayOfWeek();
    rtc.convertTemperature();
    NhietDo = rtc.getTemperature();
}
//////////////////////////////////////////////////////////////////////////////
void LoadThongTinBanDau(){ SoThoiDiemHen = Load_SoLuongThoiDiemHen(); }
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////        Kiem Tra - Desktop       ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Desktop()
{
     String ChuoiTG = SoKhongDau(gio)  + ":" + SoKhongDau(phut)  + ":" + SoKhongDau(giay)   ;
     if((ThoiDiemHienTai%7)==0){ ChuoiTG = SoKhongDau(ngay) + "/" + SoKhongDau(thang) + "/" + SoKhongDau(nam); }
     
     String LCD_Dong1 = ChuoiTG + "-" + ThuTrongTuan[thu] + "  " + (String)NhietDo + (char)223;
     String LCD_Dong2 = SoKhongDau(SS1) + "  " +  SoKhongDau(SS2) + "  "  +  SoKhongDau(SS3) + "   " + SoKhongDau(SS4);
     
     LCD_2Dong(LCD_Dong1,LCD_Dong2);
     LCD_TrangThaiTuoiCay(2,  TuoiCay[0]);
     LCD_TrangThaiTuoiCay(6,  TuoiCay[1]);
     LCD_TrangThaiTuoiCay(10, TuoiCay[2]);
     LCD_ThongBaoMua();
}
//////////////////////////////////////////////////////////////////////////////
void KiemTraRelay(){   } 

//////////////////////////////////////////////////////////////////////////////
void KiemTraTuoiCay()
{
    /*
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
                if(minute < (nPhut + nTGian)){   }
            }
            if(nKieuTuoi > 0)
            {
                if(DoAmDat >= nAmMax) {  }
                if(nKieuTuoi == 2)
                {
                    if(NhietDo > nNhietMax ) {  }
                    if(NhietDo < nNhietMin ) {  }
                }
            }
        }
    }
    KiemTraGioUuTien();
  */
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///////////////////////////   Cai Dat Gia Tri   //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void SuLyGiaTriChuoiNhan(String ChuoiNhan)
{
  bool timthay         = false;
  String ChuoiGiaTri   = "";
  String ChuoiNhanDang = "";
  
  uint8_t nKyTu = ChuoiNhan.length(), i;
  for( i = 0; i< nKyTu; i++)
  {
    String sKyTu = ChuoiNhan.substring(i, i+1);
    if(sKyTu == ":"){ timthay = true; break; }
  }
  if(timthay)
  {
      ChuoiNhanDang = ChuoiNhan.substring(0, i);
      ChuoiGiaTri   = ChuoiNhan.substring(i+1, nKyTu);
  } else { ChuoiNhanDang = ChuoiNhan; }
  LCD_1DongTrenDelay(ChuoiNhanDang, 500); 
  
  if(ChuoiNhanDang      == "UTS_DATE")       { CaiDatDate(ChuoiGiaTri);  } // Cài đặt giờ hệ thống
  else if(ChuoiNhanDang == "UTS_TIMER")      { CaiDatTimer(ChuoiGiaTri); } // Cài đặt giờ tưới
  else if(ChuoiNhanDang == "UTS_TIMER_CLEAR"){ TimerClearAll();          } //  Xóa tất cả giờ tưới
  else if(ChuoiNhanDang == "UTS_TUOICAY")    { CaiDatUuTien(ChuoiGiaTri);} // Chế độ tưới ưu tiên
  else if(ChuoiNhanDang == "UTS_RESTART")    { RESTART();                } // Restart
}
//////////////////////////////////////////////////////////////////////////////////
void CaiDatDate(String ChuoiGio)
{
     String tmp;
     tmp  = ChuoiGio.substring(0, 1);    thu   = tmp.toInt() - 2;
     tmp  = ChuoiGio.substring(2, 4);    ngay  = tmp.toInt();
     tmp  = ChuoiGio.substring(5, 7);    thang = tmp.toInt();
     tmp  = ChuoiGio.substring(8, 10);   nam   = tmp.toInt();
     tmp  = ChuoiGio.substring(11, 13);  gio   = tmp.toInt();
     tmp  = ChuoiGio.substring(14, 16);  phut  = tmp.toInt();
     tmp  = ChuoiGio.substring(17, 19);  giay  = tmp.toInt();
     
     //LCD_1DongTrenDelay(Thu, 1000); LCD_1DongTrenDelay(Ngay, 1000); LCD_1DongTrenDelay(Thang, 1000); LCD_1DongTrenDelay(Nam, 1000); 
     //LCD_1DongTrenDelay(Gio, 1000); LCD_1DongTrenDelay(Phut, 1000); LCD_1DongTrenDelay(Giay, 1000); 
     DateTime dt(nam, thang, ngay, gio, phut, giay, thu);
     rtc.setDateTime(dt);
     delay(1000);
}
//////////////////////////////////////////////////////////////////////////////////
void TimerClearAll(){ Luu_SoLuongThoiDiemHen(0); }
//////////////////////////////////////////////////////////////////////////////////
void CaiDatTimer(String sChuoi)
{
  uint8_t i;
  String tmpStr  = "";
  uint8_t nThuTu = 0, nDauPhay = 0, tmpGT = 0; 
  String ChuoiTG[11] = {"","","","","","","","","","",""};
  for( i=0; i< sChuoi.length(); i++)
  {
    if(sChuoi.substring(i, i+1) == ",")
    { 
       ChuoiTG[nThuTu] = sChuoi.substring(nDauPhay + tmpGT, i) ;
       tmpGT = 1; nDauPhay = i; nThuTu++;
    }
  }
  ChuoiTG[nThuTu] = sChuoi.substring(nDauPhay + 1, sChuoi.length()) ;
  ///////////
  uint8_t nSoGioHen = Load_SoLuongThoiDiemHen();
  int nDiaChiLuu    = nSoGioHen*11 + 31;
  for(i=0; i< 11; i++)
  {
    uint8_t nGTTian = ChuoiTG[i].toInt();
    EEPROM_WN(nDiaChiLuu + i, nGTTian);
  }
  nSoGioHen++;
  Luu_SoLuongThoiDiemHen(nSoGioHen);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void KiemTraGioUuTien()
{
  //if(ThoiGianTuoiUuTien > 0)
  {
     // if((millis()/1000 - ThoiDiemCaiDatUuTien) > ThoiGianTuoiUuTien){ ThoiGianTuoiUuTien = 0; }
  }
}
//////////////////////////////////////////////////////////////////////////////////
void CaiDatUuTien(String nTimer)
{
   //ThoiGianTuoiUuTien   = nTimer.toInt()*60;
   //ThoiDiemCaiDatUuTien = millis()/1000;
}
//////////////////////////////////////////////////////////////////////////////////
void BatTatRelay(String sRelay)
{

}


//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////        Cam Bien  - Thoi Gian    ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
int DocDoAmDat()
{
  //int nDoAmDat = analogRead(DOAMDAT);
  //return  map(nDoAmDat, 0, 1023, 0, 100);
}
//////////////////////////////////////////////////////////////////////////////////
int DocTroiMua()
{
  //int nTroiMua = analogRead(TROIMUA);
  //return  map(nTroiMua, 0, 1023, 0, 100);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////       LCD     ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_TrangThaiTuoiCay(uint8_t ViTri, uint8_t TrangThai)
{
    if(TrangThai == 1)
    { 
      lcd.setCursor(ViTri,1);
      if(ThoiDiemHienTai%2==0){ lcd.write(byte(4)); }
      else{ lcd.write(byte(5)); }
    }
}
//////////////////////////////////////////////////////////////////////////////
void LCD_ThongBaoMua()
{
    lcd.setCursor(15,1);
    if(SS4 < 50){ lcd.write(byte(2)); lcd.noBlink();  }
    else{ lcd.write(byte(3));  lcd.setCursor(15,1);  lcd.noBlink();  delay(100);  lcd.blink();  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_2Dong(String Chuoi_1,String Chuoi_2)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(Chuoi_1);
    lcd.setCursor(0,1);
    lcd.print(Chuoi_2);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_2DongDelay(String Chuoi_1,String Chuoi_2, int nDelay)
{
    LCD_2Dong(Chuoi_1, Chuoi_2);
    delay(nDelay);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTren(String sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenDelay(String sChuoi, int nTimer)
{
    LCD_1DongTren(sChuoi);
    delay(nTimer);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenN(int sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenNDelay(int nChuoi, int tDelay)
{
    LCD_1DongTrenN(nChuoi);
    delay(tDelay);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoi(String sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiDelay(String sChuoi, int nTimer)
{
    LCD_1DongDuoi(sChuoi);
    delay(nTimer);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiN(int sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiNDelay(int sChuoi, int nDelay)
{
    LCD_1DongDuoiN(sChuoi);
    delay(nDelay);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DDViDelay(String sChuoi, uint8_t nViTri, int nDelay)
{
    lcd.setCursor(nViTri,1);
    lcd.print(sChuoi);
    delay(nDelay);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      EEPROM      ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void LuuEEPROM(int nDiaChi, int nGiaTri){ EEPROM.write(nDiaChi, nGiaTri);  delay(10); }
//////////////////////////////////////////////////////////////////////////////////
int LoadEEPROM(int nDiaChi){ delay(10);  return EEPROM.read(nDiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){ EEPROM.write( DiaChi, KyTu );  delay(20); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){ EEPROM.write( DiaChi, SoLuu ); delay(20); }
//////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
int EEPROM_RN(int DiaChi){ return EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WS(int DiaChi, String ChuoiLuu)
{
  char* ChuoiTam = String2Char( ChuoiLuu );
  int DoDai  = ChuoiLuu.length();
  for (int i = 0; i < DoDai ; i++){ EEPROM_WC(DiaChi + i,ChuoiTam[i]); }
  delete ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////
String EEPROM_RS(int BatDau, int KetThuc)
{
    String ChuoiDoc = "";
    for(int i = BatDau; i< (BatDau + KetThuc); i++) { ChuoiDoc+= EEPROM_RC(i); }
    return ChuoiDoc;
}
//////////////////////////////////////////////////////////////////////////////////
void LuuVungDuLieu(int _BatDau, int _DoDai, String _ChuoiLuu)
{
  for(int i = _BatDau; i< _BatDau + _DoDai; i++){ EEPROM_WN( i,245);}
  EEPROM_WS(_BatDau, _ChuoiLuu);
}
//////////////////////////////////////////////////////////////////////////////////
String LoadVungDuLieu(int _BatDau, int _DoDai)
{
  String _strChuoi = "";
  for(int i = _BatDau; i< _BatDau + _DoDai; i++)
  { 
      int tmpChar = EEPROM_RN(i);
      if(tmpChar != 245){ _strChuoi+=  (char)tmpChar ; }
  }
  return _strChuoi;
}
////////////////////////////////////////////////////////////////////////////////// So luong thoi diem hen
uint8_t Load_SoLuongThoiDiemHen(){ return EEPROM_RN(30); }
//////////////////////////////////////////////////////////////////////////////////
void Luu_SoLuongThoiDiemHen(uint8_t nSoLuong){ EEPROM_WN(30, nSoLuong); };
//////////////////////////////////////////////////////////////////////////////////
void LayMangTongThoiGianHen()
{
  SoThoiDiemHen = Load_SoLuongThoiDiemHen();
  if(SoThoiDiemHen > 4 ) 
  { 
      Luu_SoLuongThoiDiemHen(0); 
      LCD_1DongDuoiDelay("TIMER ERROR", 1000);  
  }
  else 
  {
     if( SoThoiDiemHen > 0 ) 
     {
       //for(uint8_t i=0; i< SoThoiDiemHen*9; i++){ MangTGHen[i] = EEPROM_RN(71+i); }
     }
  }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      Text function      ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////
int String2Int(String sSo){  return sSo.toInt();}
//////////////////////////////////////////////////////////////////////////////////
String SoKhongDau(uint8_t sSo){  
     if(sSo >9){  return String(sSo); }
     else{ return ("0" + String(sSo)); }
}
//////////////////////////////////////////////////////////////////////////////////
String TrangThaiTuoi(uint8_t sSo)
{
   if(sSo == 0) { return " "; }
   else{ if(ThoiDiemHienTai%2==0){ return "."; }else{ return ":"; } }
} 
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
uint8_t CapNhat = 0;
void CaiDatChiSoDemo()
{
   //uint8_t SoLuu[16] = { 5, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
   //for(uint8_t i=0; i<16;i++){  EEPROM_WN(30+i, SoLuu[i]); }
   if(ThoiDiemHienTai%2 == 0)
   {
      CapNhat++;
      if(CapNhat == 1)
      {
        SS1 = random(99);
        SS2 = random(99);
        SS3 = random(99);
        SS4 = random(99);
      }
   }
   else{ CapNhat = 0; }
}

//////////////////////////////////////////////////////////////////////////////////

void setup() 
{
   Serial.begin(115200);
   ////////////////   
   Wire.begin();
   rtc.begin();
   ////////////////
   pinMode(PIN_RELAY1, OUTPUT);
   pinMode(PIN_RELAY2, OUTPUT);
   pinMode(PIN_RELAY3, OUTPUT);
   pinMode(PIN_RELAY4, OUTPUT);
   ////////////////
   pinMode(PIN_SS1, INPUT);
   pinMode(PIN_SS2, INPUT);
   pinMode(PIN_SS3, INPUT);
   pinMode(PIN_SS4, INPUT);
   ////////////////
   lcd.createChar(0, Symbol_BGT);
   lcd.createChar(1, Symbol_BGD);
   lcd.createChar(2, Symbol_Nang);
   lcd.createChar(3, Symbol_Mua);
   lcd.createChar(4, Symbol_TuoiCay1);
   lcd.createChar(5, Symbol_TuoiCay2);
   ////////////////
   lcd.begin(16, 2); 
   ////////////////
   CaiDatChiSoDemo();
   LoadThongTinBanDau();
}
//////////////////////////////////////////////////////////////////////////////
String ChuoiData = "";
void loop() 
{
  ThoiDiemHienTai = millis()/1000;
  CaiDatChiSoDemo();
  CapNhatThoiGian();
  ///////////////////////////////////////
  //KiemTraTuoiCay();
  //KiemTraRelay();
  Desktop();
  /////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiData+= KyTu; } delay(5);
  }
  /////////////
  if( ChuoiData != ""){ SuLyGiaTriChuoiNhan(ChuoiData); ChuoiData = ""; }
  /////////////
  delay(300);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
