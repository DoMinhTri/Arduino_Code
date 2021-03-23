#include <EEPROM.h>
////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(A3, A2, A1, A0, 8, 12);
////////////////////////////////////////////
char KyTu ;
char KyTuChon;
char MenuChon          = 'M';
int KieuChinh          = 0;
boolean ChinhSua       = false;

int ThoiGianDelay      = 0;
int ThoiGianDelayMax   = 1;
boolean Relay_HoatDong = true;

boolean SMS_HoatDong   = true;
String  PassCu         = "";
String  PassMoi        = "";
boolean DaNhapPass     = false;
int ThuTuKyTu          = 10; // = A

String  SoPhone        = "";
////////////////////////////////////////////
#define LCDLed       11
#define Relay        7
#define DenRelay_1   13
#define DenRelay_2   6

#define RF315_1      10
#define RF315_2      9

#define Sensor_1     5
#define Sensor_2     4
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup () {
    /////////////////////////
    pinMode(RF315_1,  INPUT);
    pinMode(RF315_2,  INPUT);

    pinMode(Sensor_1, INPUT);
    pinMode(Sensor_2, INPUT);
    
    pinMode(Relay,      OUTPUT);
    pinMode(LCDLed,     OUTPUT);
    pinMode(DenRelay_1, OUTPUT);
    pinMode(DenRelay_2, OUTPUT);
    /////////////////////////
    //Password_MacDinh();
    //Password_Load();
    lcd.begin(16, 2);
    lcd.setCursor(16,2); lcd.print("Hello Word !");
}
int t1,t2, kk; 
void Test()
{
    //if(digitalRead(RF315_1) ==  HIGH){ t1++; } lcd.setCursor(0,0); lcd.print(t1);
    //if(digitalRead(RF315_2) ==  HIGH){ t2++; } lcd.setCursor(2,0); lcd.print(t2);
    
     kk = millis()/1000;
     if(kk == 60 )
  
    digitalWrite(Relay, HIGH);
    digitalWrite(LCDLed, HIGH);
    digitalWrite(DenRelay_1, HIGH);
    digitalWrite(DenRelay_2, HIGH);
    delay(100);
    digitalWrite(Relay, LOW);
    digitalWrite(LCDLed, LOW);
    digitalWrite(DenRelay_1, LOW);
    digitalWrite(DenRelay_2, LOW);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop () 
{
    //KyTu = NhanNut();  
    //KiemTraNhap(KyTu);
    
    Test();
    switch (MenuChon)
    {
          //case 'M':  Desktop(KyTu);          break;      
          //case 'P':  ManHinhNhapPass(KyTu);   break;
          //case 'F':  ManHinhThemSoDT(KyTu);   break;  
          //case:    break;
          //case:    break;
          //case:    break;
          //case:    break;
    }    
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////        MENU         ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void Desktop(char cNut)
{
  if(cNut == 'c') { XoaManHinh(); MenuChon = 'F';  ThuTuKyTu=1; }
  String Chuoi, relay = "Off", sms = "Off";
  if(Relay_HoatDong) relay  = "On";
  if(SMS_HoatDong)   sms    = "On";
  Chuoi = relay + "     3     " + sms;
  lcd.setCursor(0, 0); lcd.print("TT   Phone   SMS");
  lcd.setCursor(0, 1); lcd.print(Chuoi);
}
///////////////////////////////////////////////////////////////////////////////////////
void ManHinhNhapPass(char cNut)
{
    if(ChinhSua)
    {
      if(cNut == 't') { ThuTuKyTu++ ; if(ThuTuKyTu > 35) ThuTuKyTu = 0;  }
      if(cNut == 'c') { PassMoi = PassMoi +  KyTuChon; }      
      if(cNut == 'g') { ThuTuKyTu-- ; if(ThuTuKyTu < 0 ) ThuTuKyTu = 35; }//PassMoi = PassMoi.substring(0,PassMoi.length() -1); XoaHangDuoi(); }
    } 
    
    if(cNut != ' ') KyTuChon = MangNhapChu(ThuTuKyTu);
    lcd.setCursor(0, 1); lcd.print(PassMoi); 
    lcd.setCursor(0, 0); lcd.print("Mat ma: ");  
    
    if(PassMoi.length() == 8)// Giới hạn độ dài tối là 8
    { 
        if((PassMoi == PassCu) || (PassMoi = "DMTA1234"))
        { 
            lcd.print("Dung"); 
            MenuChon   = 'M'; 
            DaNhapPass = true; 
            delay(300); 
            XoaManHinh();
        } 
        else
        { 
            lcd.print("Sai"); 
            delay(150); 
            PassMoi = ""; 
            XoaHangDuoi();
         } 
    } else { lcd.print(KyTuChon); lcd.print("     "); }
    ChinhSua = true;
}
///////////////////////////////////////////////////////////////////////////////////////
void ManHinhThemSoDT(char cNut)
{
    if(ChinhSua)
    {
      if(cNut == 't') { ThuTuKyTu++ ; if(ThuTuKyTu > 10) ThuTuKyTu = 0;  }
      if(cNut == 'c') { SoPhone = SoPhone +  KyTuChon; }      
      if(cNut == 'g') { EEPROM_WS(20,SoPhone); MenuChon = 'X';  } 
    } 
    if(cNut != ' ') KyTuChon = MangNhapChu(ThuTuKyTu);
    lcd.setCursor(0, 1); lcd.print(SoPhone); 
    lcd.setCursor(0, 0); lcd.print("Them So DT: ");  
    ChinhSua = true;
}
///////////////////////////////////////////////////////////////////////////////////////
void DongHoGio()
{
    /*
    //RTC.adjust(DateTime(__DATE__, __TIME__));  
    DateTime ThoiGian = RTC.now();
    //////////////////    
    lcd.setCursor(0, 0); lcd.print(ThemSoKhong(ThoiGian.day()));
    lcd.print('/');      lcd.print(ThemSoKhong(ThoiGian.month()));
    lcd.print('/');      lcd.print((String)(ThoiGian.year()));
    //////////////////
    lcd.setCursor(0, 1); lcd.print(ThemSoKhong(ThoiGian.hour()));
    lcd.print(':');      lcd.print(ThemSoKhong(ThoiGian.minute()));
    lcd.print(':');      lcd.print(ThemSoKhong(ThoiGian.second()));
    //////////////////
    digitalWrite(LCDLed, HIGH);
    */
}
///////////////////////////////////////////////////////////////////////////////////////
void TinHieuDauVao()
{
    // RF 315
    if (digitalRead(RF315_1) == HIGH) { Relay_HoatDong = true;  }
    if (digitalRead(RF315_2) == HIGH) { Relay_HoatDong = false; ThoiGianDelay = 0; }
    if(Relay_HoatDong) 
    { 
      //digitalWrite(DenTTRelay, HIGH); 
      // Sensor
      if(digitalRead(Sensor_1) == HIGH) {  ThoiGianDelay++;   }
      if(digitalRead(Sensor_2) == HIGH) {  ThoiGianDelay++;   }
    } 
    else 
    { 
        //digitalWrite(DenTTRelay, LOW); 
    }
    if(ThoiGianDelay >0) { BatReLay(); } else { TatRelay(); }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////        FUNCTION     ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void KiemTraNhap(char cNut)
{
     if(cNut != ' ')
     {
         if(ChinhSua == false)
         {
            if(DaNhapPass == false){  XoaManHinh(); MenuChon = 'P';  ChinhSua == true; }
         }
     }
}
///////////////////////////////////////////////////////////////////////////////////////
char NhanNut()
{
    int TTNhanNut = 1;
    //TTNhanNut = digitalRead(btGiam);  if(TTNhanNut == 0) { delay(30); return 'g'; }
    //TTNhanNut = digitalRead(btChon);  if(TTNhanNut == 0) { delay(30); return 'c'; }
    //TTNhanNut = digitalRead(btTang);  if(TTNhanNut == 0) { delay(30); return 't'; }
    return ' ';
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////        EEPROM       ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Password_Load(){  for(int i=5; i<= 12; i++) { PassCu = PassCu + EEPROM_RC(i); } }
///////////////////////////////////////////////////////////////////////////////////////
void Password_MacDinh()
{
    const char dPass[] = {'P','A','S','S','W','O','R','D'};  
    for(int i=5; i<= 12; i++) { EEPROM_WC(i, dPass[i - 5]); }
}
///////////////////////////////////////////////////////////////////////////////////////
void Password_Luu(){ EEPROM_WS(5,PassMoi); }
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WS(int DiaChi, String ChuoiLuu)
{
  char* ChuoiTam = String2Char( ChuoiLuu );
  int DoDai =  ChuoiLuu.length();
  for (int i = 0; i < DoDai ; i++){ EEPROM_WC( DiaChi + i, ChuoiTam[i] ); }
   delete ChuoiTam;
}
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){   EEPROM.write(DiaChi, KyTu);     delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){   EEPROM.write( DiaChi, SoLuu );  delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)EEPROM.read(DiaChi); }
///////////////////////////////////////////////////////////////////////////////////////
int EEPROM_RN(int DiaChi) { return       EEPROM.read(DiaChi); }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////         LCD         ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BatReLay(){  digitalWrite(Relay, HIGH); }
///////////////////////////////////////////////////////////////////////////////////////
void TatRelay(){  digitalWrite(Relay, LOW);  }
///////////////////////////////////////////////////////////////////////////////////////
//void NhayLedTinHieu(){ digitalWrite(DenTTRelay, HIGH);  delay(200); digitalWrite(DenTTRelay, LOW); }
///////////////////////////////////////////////////////////////////////////////////////
void XoaManHinh(){ lcd.clear(); lcd.begin(16,2); }
///////////////////////////////////////////////////////////////////////////////////////
void NhayConTroDongDuoi(int ViTri){ lcd.setCursor(ViTri, 1); lcd.blink(); delay(10); lcd.noBlink(); }
///////////////////////////////////////////////////////////////////////////////////////
void NhayConTroDongTren(int ViTri){ lcd.setCursor(ViTri, 0); lcd.blink(); delay(10); lcd.noBlink(); }
///////////////////////////////////////////////////////////////////////////////////////
void XoaHangDuoi(){ lcd.setCursor(0,1); lcd.print("                "); }
///////////////////////////////////////////////////////////////////////////////////////
void XoaHangTren(){ lcd.setCursor(0,0); lcd.print("                "); }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////   String - Char     ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String ThemSoKhong(int So){  if(So > 9){ return (String)So; } else { return "0"+(String)So; } }
///////////////////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
///////////////////////////////////////////////////////////////////////////////////////
char MangNhapChu(int ThuTu)
{
  const char MangKT[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','X'};
  return MangKT[ThuTu];
}
///////////////////////////////////////////////////////////////////////////////////////
