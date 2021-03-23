#include <EEPROM.h>
////////////////////////////////////////////
#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(10);
byte Symbol_BGT[8]      = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};
byte Symbol_BGD[8]      = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte Symbol_Mua[8]      = {B00100,B00100,B00100,B01110,B10001,B10101,B10001,B01110};
byte Symbol_Nang[8]     = {B00100,B10001,B01110,B11010,B01011,B01110,B10001,B00100};
byte Symbol_TuoiCay1[8] = {B11011,B11011,B00000,B10001,B11011,B11111,B11111,B11111};
byte Symbol_TuoiCay2[8] = {B11111,B11111,B11111,B11011,B11011,B00000,B10001,B11011};
////////////////////////////////////
uint8_t TGianUuTien[4] = {0 , 0, 0, 0}; // RL1 - RL2 - RL3 - Timer[P] 
uint8_t Relay[4]       = {0 , 0, 0, 0};
uint8_t DoAmDat[3]     = {0 , 0, 0};

#define ONhoDau  31
#define TongPhut 1440   // Tổng số phút trong ngày = 24*60 = 1440

uint8_t DoAmMua = 0;
boolean TroiMua = false;
uint8_t  nThoiTiet, nNhietDo, nLanTuoi = 0;

unsigned long ThoiDiemHienTai = 0;
unsigned long ThoiDiemSauCung = 0;

uint8_t MangTimer[165]; // 3*5*11

String DataUART   = "";
String DataSensor = "";
///////////////////////////////////////////
#include <Sodaq_DS3231.h>
#include <Wire.h>
uint8_t giay, phut, gio, ngay, thang, nam, thu; 
char* ThuTrongTuan[8] = { "CN", "T2", "T3" , "T4" , "T5" , "T6" , "T7", "CN"};
///////////////////////////////////////////
#define PIN_RELAY1  7
#define PIN_RELAY2  8
#define PIN_RELAY3  6
#define PIN_RELAY4  5

#define PIN_SS1     A3
#define PIN_SS2     A2
#define PIN_SS3     A1
#define PIN_SS4     A0
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///   Reset function
void RESTART(){ 
  LCD_1DongTrenDelay("RESTART", 500); 
  asm volatile (" jmp 0"); 
}               
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
    nNhietDo = rtc.getTemperature() - 4;
}
//////////////////////////////////////////////////////////////////////////////
void Desktop()
{
     String LCD_Dong1 = "";
     if(ThoiDiemHienTai%10 < 8)
            LCD_Dong1 = ChuSo2DonVi(gio)  + ":" + ChuSo2DonVi(phut)  + ":" + ChuSo2DonVi(giay) + "-" + ThuTrongTuan[thu] + "  " + (String)nNhietDo + (char)223 ;
     else
            LCD_Dong1 = ChuSo2DonVi(ngay) + ":" + ChuSo2DonVi(thang) + ":" + ChuSo2DonVi(nam)  + "-" + ThuTrongTuan[thu] + "  " + (String)nNhietDo + (char)223 ;
          
     String LCD_Dong2 = ChuSo2DonVi(DoAmDat[0]) + "  " +  ChuSo2DonVi(DoAmDat[1]) + "  "  +  ChuSo2DonVi(DoAmDat[2]) + "   " + ChuSo2DonVi(DoAmMua);
     
     LCD_2Dong(LCD_Dong1,LCD_Dong2);
     LCD_TrangThaiTuoiCay(2,  Relay[0]);
     LCD_TrangThaiTuoiCay(6,  Relay[1]);
     LCD_TrangThaiTuoiCay(10, Relay[2]);
     LCD_ThongBaoMua();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////        Relay          //////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void BatTatRelay(String sRelay)
{
    uint8_t RL = sRelay.substring(0,1).toInt()-1;
    uint8_t TT = sRelay.substring(2,3).toInt();
    if(RL <3){ Relay[RL] = TT; }
}
//////////////////////////////////////////////////////////////////////////////////
void CaiDatUuTien(String nTimer)
{
      TGianUuTien[0]  = nTimer.substring(0,1).toInt();  // RL 1
      TGianUuTien[1]  = nTimer.substring(2,3).toInt();  // RL 2
      TGianUuTien[2]  = nTimer.substring(4,5).toInt();  // RL 3
      TGianUuTien[3]  = nTimer.substring(6,8).toInt();  // TG Tưới [Phút]  - VD: 1,1,0,90
      ThoiDiemSauCung = ThoiDiemHienTai + TGianUuTien[3]*60;
      if((TGianUuTien[0] == 0) && (TGianUuTien[1] == 0) && (TGianUuTien[2] == 0) && (TGianUuTien[3] == 0)){ RESTART(); }
      //Serial.println(ThoiDiemSauCung);
}
//////////////////////////////////////////////////////////////////////////////////
void KiemTraTuoiUuTien()
{
    if( TGianUuTien[3] > 0)
    {
          if( ThoiDiemHienTai < ThoiDiemSauCung )
          {
              Relay[0] = TGianUuTien[0];
              Relay[1] = TGianUuTien[1];
              Relay[2] = TGianUuTien[2];
          }
          else {  RESTART(); }
    }
}
//////////////////////////////////////////////////////////////////////////////
void KiemTraRelay()
{  
    if((Relay[0] == 1) || (Relay[1] == 1) || (Relay[2] == 1))
       Relay[3] = 1;
    else
       Relay[3] = 0;
      
    digitalWrite(PIN_RELAY1, Relay[0]);
    digitalWrite(PIN_RELAY2, Relay[1]);
    digitalWrite(PIN_RELAY3, Relay[2]);
    digitalWrite(PIN_RELAY4, Relay[3]);
} 
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////       Thoi tiet             ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void LoadThoiTiet()
{ 
    nThoiTiet = Load_ThoiTiet(); 
    if(nThoiTiet > 200){ nThoiTiet = 30; Luu_ThoiTiet(30); }  // sử lý trường hợp load lần đầu
}
//////////////////////////////////////////////////////////////////////////////////
void KiemTraThoiTiet()
{
  if( DoAmMua > nThoiTiet )
  { 
      TroiMua  = true; 
      Relay[0]= 0; Relay[1]= 0; Relay[2]= 0; Relay[3]= 0;
  }
  else { TroiMua = false; }
}
//////////////////////////////////////////////////////////////////////////////
void CaiDatThoiTiet(String ThoiTiet)
{
  Luu_ThoiTiet(ThoiTiet.toInt());
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////      Date -  Timer          ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void TimerClearAll(){ nLanTuoi = 0; Luu_SoLanTuoi(0); }
//////////////////////////////////////////////////////////////////////////////
void CaiDatDate(String ChuoiGio)
{
     String tmp;      // UTS_DATE: dw/dd/mm/yy-hh:mm:ss  -   UTS_DATE:1/19/09/16-12:23:35       dw[1-7] = T2,T3,T4,T5,T6,T7,CN
     tmp  = ChuoiGio.substring(0, 1);    thu   = tmp.toInt();
     tmp  = ChuoiGio.substring(2, 4);    ngay  = tmp.toInt();
     tmp  = ChuoiGio.substring(5, 7);    thang = tmp.toInt();
     tmp  = ChuoiGio.substring(8, 10);   nam   = tmp.toInt();
     tmp  = ChuoiGio.substring(11, 13);  gio   = tmp.toInt();
     tmp  = ChuoiGio.substring(14, 16);  phut  = tmp.toInt();
     tmp  = ChuoiGio.substring(17, 19);  giay  = tmp.toInt();
     DateTime dt(nam, thang, ngay, gio, phut, giay, thu);
     rtc.setDateTime(dt);
     delay(1000);
}
//////////////////////////////////////////////////////////////////////////////
void CaiDatTimer(String sChuoi)
{
  LoadSoLanTuoi();   // UTS_TIMER:0,0,3,3,06,00,02,20,75,20,32   -   UTS_TIMER:0,0,1,1,06,05,03,22,33,44,55
  uint8_t tmpGiaTri   = 0;
  uint16_t nDiaChiLuu = nLanTuoi*11 + ONhoDau;
  uint8_t nTimer[11]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  ///////////
  tmpGiaTri = sChuoi.substring(0, 1).toInt();   nTimer[0]  = tmpGiaTri; // xTrang Thai Hoat Dong - Trong ngày tưới hay chưa
  tmpGiaTri = sChuoi.substring(2, 3).toInt();   nTimer[1]  = tmpGiaTri; // xTuoi hay Ngưng
  tmpGiaTri = sChuoi.substring(4, 5).toInt();   nTimer[2]  = tmpGiaTri; // xCamBien  [1-3]
  tmpGiaTri = sChuoi.substring(6, 7).toInt();   nTimer[3]  = tmpGiaTri; // xKieuTuoi [1-3]
  tmpGiaTri = sChuoi.substring(8, 10).toInt();  nTimer[4]  = tmpGiaTri; // xGio
  tmpGiaTri = sChuoi.substring(11, 13).toInt(); nTimer[5]  = tmpGiaTri; // xPhut
  tmpGiaTri = sChuoi.substring(14, 16).toInt(); nTimer[6]  = tmpGiaTri; // xThoiGian
  tmpGiaTri = sChuoi.substring(17, 19).toInt(); nTimer[7]  = tmpGiaTri; // xDoAmMax
  tmpGiaTri = sChuoi.substring(20, 22).toInt(); nTimer[8]  = tmpGiaTri; // xDoAmMin
  tmpGiaTri = sChuoi.substring(23, 25).toInt(); nTimer[9]  = tmpGiaTri; // xNhietDoMax
  tmpGiaTri = sChuoi.substring(26, 28).toInt(); nTimer[10] = tmpGiaTri; // xNhietDoMin
  ///////////
  for(uint8_t i=0; i< 11; i++){  EEPROM_WN( nDiaChiLuu + i, nTimer[i] );  }
  nLanTuoi++;
  Luu_SoLanTuoi(nLanTuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LoadMangTimer()
{ 
    for(uint8_t i = 0; i< nLanTuoi*11; i++)
    {
       MangTimer[i] = EEPROM_RN( i + ONhoDau );
       //Serial.println( MangTimer[i]);
    }
}
//////////////////////////////////////////////////////////////////////////////
uint8_t cTTHoatDong, cTuoiHayChua, cTTRelay, cKieuTuoi, cGioTuoi, cPhutTuoi, cTGTuoi, cDoAmMax, cDoAmMin, cNhietDoMax, cNhietDoMin;
           
void KiemTraTuoiCay()
{
    if(nLanTuoi >0)
    {
       for(uint8_t i = 0; i< nLanTuoi; i++)
       {
           cTTHoatDong  = MangTimer[i*11 + 0];   //Serial.println(cTTHoatDong);
           cTuoiHayChua = MangTimer[i*11 + 1];   //Serial.println(cTuoiHayChua);
           cTTRelay     = MangTimer[i*11 + 2];   //Serial.println(cTTRelay);
           cKieuTuoi    = MangTimer[i*11 + 3];   //Serial.println(cKieuTuoi);
           cGioTuoi     = MangTimer[i*11 + 4];   //Serial.println(cGioTuoi);
           cPhutTuoi    = MangTimer[i*11 + 5];   //Serial.println(cPhutTuoi);
           cTGTuoi      = MangTimer[i*11 + 6];   //Serial.println(cTGTuoi);
           cDoAmMax     = MangTimer[i*11 + 7];   //Serial.println(cDoAmMax);
           cDoAmMin     = MangTimer[i*11 + 8];   //Serial.println(cDoAmMin);
           cNhietDoMax  = MangTimer[i*11 + 9];   //Serial.println(cNhietDoMax);
           cNhietDoMin  = MangTimer[i*11 + 10];  //Serial.println(cNhietDoMin); Serial.println("---------------");
           //giay, phut, gio, ngay, nam, thu, nThoiTiet
           //////////////////
           switch (cKieuTuoi)
          {
              case 1:
              {
                  Relay[cTTRelay - 1] = KiemTraGioTuoi(cGioTuoi, cPhutTuoi, cTGTuoi);
                  break;
              }
              case 2:
              {
                  
                  break;
              }
           }
           //////////////////
      }
      //////////////////
    }
    //////////////////
}
//////////////////////////////////////////////////////////////////////////////
boolean KiemTraGioTuoi(uint8_t nGio, uint8_t nPhut, uint8_t nTGian)
{
  boolean KiemTra = false;
  //uint8_t nPhutDu    = (nPhut + nTGian)%60 ;
  //uint8_t nGioDu     = (nPhut + nTGian)/60 ;
  
  uint16_t nGioHienTai = gio*60  + phut;
  uint16_t nGioBatDau  = nGio*60 + nPhut;
  uint16_t nGioKetThuc = nGio*60 + nPhut + nTGian;

  if(nGioKetThuc < TongPhut)
  {
      if((nGioBatDau <= nGioHienTai) && (nGioHienTai <= nGioKetThuc))
          KiemTra = true;
       else
          KiemTra = false;
      /////////////
  }
  else
  {
    
  }
  return KiemTra;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////        Cam Bien  - Load         ////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
uint8_t DocDoAmDat(uint8_t Pin)
{
  uint8_t nDoAmDat = analogRead(Pin);
  return  map(nDoAmDat, 0, 255, 0, 99);
}
//////////////////////////////////////////////////////////////////////////////
void LoadSoLanTuoi()
{ 
    nLanTuoi = Load_SoLanTuoi(); 
    if(nLanTuoi > 100){ TimerClearAll(); } // sử lý trường hợp load lần đầu
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////       LCD     ////////////////////////////////
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
    if(TroiMua){ lcd.write(byte(3));  lcd.setCursor(15,1);  lcd.noBlink();  delay(100);  lcd.blink(); }
    else{ lcd.write(byte(2)); lcd.noBlink(); }
}
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
void LCD_2DongDelay(String Chuoi_1,String Chuoi_2, uint16_t nDelay)
{
    LCD_2Dong(Chuoi_1, Chuoi_2);
    delay(nDelay);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTren(String sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenDelay(String sChuoi, uint16_t nTimer)
{
    LCD_1DongTren(sChuoi);
    delay(nTimer);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenN(uint16_t sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongTrenNDelay(uint16_t nChuoi, uint16_t tDelay)
{
    LCD_1DongTrenN(nChuoi);
    delay(tDelay);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoi(String sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiDelay(String sChuoi, uint16_t nTimer)
{
    LCD_1DongDuoi(sChuoi);
    delay(nTimer);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiN(uint16_t sChuoi)
{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(sChuoi);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DongDuoiNDelay(uint16_t sChuoi, uint16_t nDelay)
{
    LCD_1DongDuoiN(sChuoi);
    delay(nDelay);
}
//////////////////////////////////////////////////////////////////////////////
void LCD_1DDViDelay(String sChuoi, uint8_t nViTri, uint16_t nDelay)
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

void LuuEEPROM(uint16_t nDiaChi, uint8_t nGiaTri){ EEPROM.write(nDiaChi, nGiaTri);  delay(20); }
//////////////////////////////////////////////////////////////////////////////////
int LoadEEPROM(uint16_t nDiaChi){ delay(10);  return EEPROM.read(nDiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(uint16_t DiaChi, char KyTu){ EEPROM.write( DiaChi, KyTu );  delay(20); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(uint16_t DiaChi, uint8_t SoLuu){ EEPROM.write( DiaChi, SoLuu ); delay(20); }
//////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(uint16_t DiaChi){ return (char)EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
int EEPROM_RN(uint16_t DiaChi){ return EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WS(uint16_t DiaChi, String ChuoiLuu)
{
  char* ChuoiTam = String2Char( ChuoiLuu );
  uint16_t DoDai  = ChuoiLuu.length();
  for (uint16_t i = 0; i < DoDai ; i++){ EEPROM_WC(DiaChi + i,ChuoiTam[i]); }
  delete ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////
String EEPROM_RS(uint16_t BatDau, uint16_t KetThuc)
{
    String ChuoiDoc = "";
    for(uint16_t i = BatDau; i< (BatDau + KetThuc); i++) { ChuoiDoc+= EEPROM_RC(i); }
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
uint8_t Load_SoLanTuoi(){ return EEPROM_RN(30); }
//////////////////////////////////////////////////////////////////////////////////
void Luu_SoLanTuoi(uint8_t nSoLuong){ EEPROM_WN(30, nSoLuong); };
////////////////////////////////////////////////////////////////////////////////// So luong thoi diem hen
uint8_t Load_ThoiTiet(){ return EEPROM_RN(29); }
//////////////////////////////////////////////////////////////////////////////////
void Luu_ThoiTiet(uint8_t nChiSo){ EEPROM_WN(29, nChiSo); };

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      Text function      ////////////////////////////////////
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
String ChuSo2DonVi(uint8_t sSo)
{  
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
///////////////////////////   UART Function     //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void SuLyGiaTriChuoiNhan(String ChuoiNhan)
{
  bool timthay         = false;
  String ChuoiGiaTri   = "";
  String ChuoiNhanDang = "";
  uint8_t i;
  uint8_t nKyTu = ChuoiNhan.length();
  for( i = 0; i< nKyTu; i++)
  {
    String sKyTu = ChuoiNhan.substring(i, i+1);
    if(sKyTu == ":"){ timthay = true; break; }
  }
  if(timthay)
  {
      ChuoiNhanDang = ChuoiNhan.substring(0, i);
      ChuoiGiaTri   = ChuoiNhan.substring(i+1, nKyTu);
  }
  
  if(ChuoiNhanDang      == "UTS_DATE")       { CaiDatDate(ChuoiGiaTri);     } // Cài đặt giờ hệ thống
  else if(ChuoiNhanDang == "UTS_THOITIET")   { CaiDatThoiTiet(ChuoiGiaTri); } // Cài đặt chỉ số thời tiết
  else if(ChuoiNhanDang == "UTS_TIMER")      { CaiDatTimer(ChuoiGiaTri);    } // Cài đặt giờ tưới
  else if(ChuoiNhanDang == "UTS_TIMERCLEAR") { TimerClearAll();             } // Xóa tất cả giờ tưới
  else if(ChuoiNhanDang == "UTS_RELAY")      { BatTatRelay(ChuoiGiaTri);    } // Bật tắt relay
  else if(ChuoiNhanDang == "UTS_TUOIUUTIEN") { CaiDatUuTien(ChuoiGiaTri);   } // Chế độ tưới ưu tiên
  else if(ChuoiNhanDang == "UTS_RESTART")    {  RESTART();                  } // Restart
  if(timthay){ LCD_2DongDelay(ChuoiNhanDang, ChuoiGiaTri, 1000); }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
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
   LoadSoLanTuoi();
   LoadThoiTiet();
   LoadMangTimer();
   //Serial.println("UTS_BEGIN:1");
}
//////////////////////////////////////////////////////////////////////////////

void loop() 
{
  CapNhatThoiGian();
  ThoiDiemHienTai = millis()/1000;
  DoAmDat[0] = DocDoAmDat(PIN_SS1);
  DoAmDat[1] = DocDoAmDat(PIN_SS2);
  DoAmDat[2] = DocDoAmDat(PIN_SS3);
  DoAmMua    = DocDoAmDat(PIN_SS4);
  ///////////////////////////////////////
  KiemTraTuoiCay();
  KiemTraTuoiUuTien();
  KiemTraThoiTiet(); // Khi có trời mưa, tất cả các Relay ngưng hoạt động - Ngưng tưới cây
  KiemTraRelay();
  Desktop();
  ShowThongTinMoiTruong();
  /////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ DataUART+= KyTu; } delay(5);
  }
  /////////////
  if( DataUART != "")
  { 
      DataUART.trim();
      SuLyGiaTriChuoiNhan(DataUART); 
      DataUART = ""; 
  }
  /////////////
  delay(1000);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void ShowThongTinMoiTruong()
{
    DataSensor  = "UTS_MCU:UpToSmart.VN_" + (String)Relay[0]   + "," + (String)Relay[1] + "," + (String)Relay[2] + "," + (String)Relay[3] + "," +  nNhietDo           + ",";
    DataSensor += ChuSo2DonVi(DoAmDat[0]) + "," + ChuSo2DonVi(DoAmDat[1]) + "," + ChuSo2DonVi(DoAmDat[2]) + "," + ChuSo2DonVi(DoAmMua) + "," + (String)nThoiTiet  + ","  + (String)TroiMua + ",_";
    DataSensor += ChuSo2DonVi(ngay) + "/" + ChuSo2DonVi(thang) + "/" + ChuSo2DonVi(nam) + "/" + ChuSo2DonVi(gio) + "/" + ChuSo2DonVi(phut)+ "/" + ChuSo2DonVi(giay)   + "/_" + (String)nLanTuoi ;     
    Serial.println(DataSensor);
    DataSensor  = "";
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
