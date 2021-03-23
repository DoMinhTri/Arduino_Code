


#include <EEPROM.h>

///////////////////////////////////////////
#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;
bool Century = false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;
////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(A3, A2, A1, A0, 8, 12);

#define AutoReset    2

#define LCDLed       11
#define Relay        7
#define DenRelay_1   13
#define DenRelay_2   6

#define RF315_B      10
#define RF315_C      9
#define RF315_A      3

#define Sensor_1     5
#define Sensor_2     4

int TinHieu_1       = 0;
int TinHieu_2       = 0;

const int ChiSoBaoDong = 15;
String  SoDangChon     = "";
int ThuTuSoChon        = 0;
String  textSMS        = "Do Minh Tri";
const int TongDanhSach = 3 ;
String  phoneNumber[]  = {"0914748979","0917874234","0945770906"};

boolean Sensor_HD_1  = true;
boolean Sensor_HD_2  = true;
boolean BaoDong_TH   = true;
////////////////////////////////////////////////////////////////////////////////////////// 
void setup()
{
    Wire.begin();
    Serial.begin(9600);
  
    pinMode(RF315_B,  INPUT);
    pinMode(RF315_C,  INPUT);
    pinMode(RF315_A,  INPUT);

    pinMode(Sensor_1, INPUT);
    pinMode(Sensor_2, INPUT);
    
    pinMode(Relay,      OUTPUT);
    pinMode(LCDLed,     OUTPUT);
    pinMode(DenRelay_1, OUTPUT);
    pinMode(DenRelay_2, OUTPUT);
    pinMode(AutoReset,  OUTPUT);
    
    //ThuTuSoChon = DocSoChon();
    //if(ThuTuSoChon > TongDanhSach ) { ThuTuSoChon = 0; }
    //SoDangChon = phoneNumber[ThuTuSoChon];
    SoDangChon  = "0917874234";
    
    lcd.begin(16, 2); lcd.print("  -- So Chon --"); 
    lcd.setCursor(4, 1);  lcd.print(SoDangChon); 
    
    BatDenLCD(); BatDenHieu_1();
    ////////////////////  
    KhoiDongSim();  
    delay(20000); 
    ////////////////////
    lcd.clear(); lcd.begin(16, 2); 
    BatDenHieu_2();
    ////////////////////
    //if(DocBaoDong() == 1){ XoaBaoDong(); BatRelay();  } // Trạng thái báo động
}
////////////////////////////////////////////////////////////////////////////////////////// 
void loop()
{
  TinHieuDauVao_Va_Remote(); 
  Desktop();
  BaoDong();
  delay(100);
}
//////////////////////////////////////////////////////////////////////////////////////////
void Desktop()
{
  int second,minute,hour,date,month,year,temperature; 
  second = Clock.getSecond();
  minute = Clock.getMinute();
  hour   = Clock.getHour(h12, PM);
  date   = Clock.getDate();
  month  = Clock.getMonth(Century);
  year   = Clock.getYear();
  temperature=Clock.getTemperature();
  
  lcd.setCursor(0,0);
  lcd.print(20); 
  lcd.print(year,DEC);    lcd.print('/');
  lcd.print(month,DEC);   lcd.print('/');
  lcd.print(date,DEC);    
  lcd.print("  ["); lcd.print(temperature); lcd.print((char)223); lcd.print(']');
  
  lcd.setCursor(0,1);
  lcd.print(hour,DEC);    lcd.print(':');
  lcd.print(minute,DEC);  lcd.print(':');
  lcd.print(second,DEC);  
  
  lcd.setCursor(9,1); lcd.print("       ");
  lcd.setCursor(9,1); lcd.print(TinHieu_1);
  lcd.setCursor(12,1); lcd.print(' '); 
  lcd.setCursor(13,1); lcd.print(TinHieu_2);
}
//////////////////////////////////////////////////////////////////////////////////////////
void TinHieuDauVao_Va_Remote()
{
   if(Sensor_HD_1){ if(digitalRead(Sensor_1) == HIGH){ TinHieu_1++; } } 
   if(Sensor_HD_2){ if(digitalRead(Sensor_2) == HIGH){ TinHieu_2++; } }
   /////
    if(digitalRead(RF315_C) == HIGH)
   { 
       BaoDong_TH  = !BaoDong_TH ; 
       Sensor_HD_1 = BaoDong_TH;  SuLyTinHieu_1();
       Sensor_HD_2 = BaoDong_TH;  SuLyTinHieu_2();
       delay(150); 
       /*
       if(BaoDong_TH)
       {
       ThuTuSoChon++; 
       if(ThuTuSoChon>TongDanhSach){ ThuTuSoChon = 0; }
       LuuSoChon(ThuTuSoChon);
       SoDangChon = phoneNumber[ThuTuSoChon];
       lcd.clear();lcd.begin(16, 2);
       lcd.setCursor(4, 1);  lcd.print(SoDangChon); 
       delay(3000);
       lcd.clear();lcd.begin(16, 2);
       }
       */
   }
   /////
   if(digitalRead(RF315_A) == HIGH){ Sensor_HD_1 = !Sensor_HD_1 ; SuLyTinHieu_1(); delay(150); } 
   if(digitalRead(RF315_B) == HIGH){ Sensor_HD_2 = !Sensor_HD_2 ; SuLyTinHieu_2(); delay(150); } 
}
//////////////////////////////////////////////////////////////////////////////////////////
void SuLyTinHieu_1(){ if(Sensor_HD_1){ BaoDong_TH = Sensor_HD_1; BatDenHieu_1(); } else { TatDenHieu_1();  TinHieu_1=0; } }
//////////////////////////////////////////////////////////////////////////////////////////
void SuLyTinHieu_2(){ if(Sensor_HD_2){ BaoDong_TH = Sensor_HD_2; BatDenHieu_2(); } else { TatDenHieu_2();  TinHieu_2=0; } }
//////////////////////////////////////////////////////////////////////////////////////////
void Nhayled(){  digitalWrite(DenRelay_1, HIGH); delay(100); digitalWrite(DenRelay_1, LOW); }

void ResetHeThong(){ digitalWrite(AutoReset, HIGH); }

void BatDenLCD()   { digitalWrite(LCDLed, HIGH); }
void TatDenLCD()   { digitalWrite(LCDLed, LOW);  }

void BatDenHieu_1(){ digitalWrite(DenRelay_1, HIGH); }
void TatDenHieu_1(){ digitalWrite(DenRelay_1, LOW);  }

void BatDenHieu_2(){ digitalWrite(DenRelay_2, HIGH); }
void TatDenHieu_2(){ digitalWrite(DenRelay_2, LOW);  }

void BatRelay(){ digitalWrite(Relay, HIGH); }
void TatRelay(){ digitalWrite(Relay, LOW);  }
//////////////////////////////////////////////////////////////////////////////////////////
void BaoDong()
{
   if(BaoDong_TH)
   {
      if((TinHieu_1 <= ChiSoBaoDong) && (TinHieu_2 <= ChiSoBaoDong)) { TatRelay(); }
      else
      {   
          BatRelay(); 
          //LuuBaoDong(); 
          NhanTin( SoDangChon, "Chuong Bao Dong");   
          KhoiDongSim();
          GoiDien( SoDangChon );
          //KhoiDongSim();  GoiDien( phoneNumber[1] );
          //ResetHeThong();   
          TinHieu_1 = 0;
          TinHieu_2 = 0;
      }
   }
   else{ TatRelay(); }
}
//////////////////////////////////////////////////////////////////////////////////////////
void NhayRelay(){ digitalWrite(Relay, HIGH); }//delay(1000); digitalWrite(Relay, LOW);  }
//////////////////////////////////////////////////////////////////////////////////////////
void GoiDien(String phonenumber)
{
  Serial.print("ATD"); 
  Serial.print(phonenumber);
  Serial.print(";");
  Serial.print("\r\n");
  delay(30000);            
  Serial.print("ATH\r\n");   
  KhoiDongSim();
}
//////////////////////////////////////////////////////////////////////////////////////////
void NhanTin(String phonenumber, String message)
{
  Serial.print("AT+CMGF=1\r\n");
  delay(500);
  Serial.print("AT+CMGS=\""); 
  Serial.print(phonenumber);
  Serial.print("\"\r");
  delay(500);
  // At this time, SIM900 will return '>'
  //SIM900_response(500);
  Serial.print(message); 
  Serial.print((char)26); // Ctrl+Z      
  Serial.flush();  // clear buffer 
  delay(5000); 
  ///////////////////////
  KhoiDongSim();
}
//////////////////////////////////////////////////////////////////////////////////////////
void  KhoiDongSim()
{
  digitalWrite(DenRelay_1, HIGH); delay(1000);
  digitalWrite(DenRelay_1, LOW);  delay(1000);
  digitalWrite(DenRelay_1, HIGH); delay(5000); // waiting for Finding network
}
 
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
void LuuBaoDong(){ EEPROM_WN(1,1); }
void XoaBaoDong(){ EEPROM_WN(1,0); }
int DocBaoDong() { return EEPROM_RN(1); }
//////////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){   EEPROM.write(DiaChi, KyTu);     delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){   EEPROM.write( DiaChi, SoLuu );  delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)EEPROM.read(DiaChi); }
///////////////////////////////////////////////////////////////////////////////////////
int EEPROM_RN(int DiaChi) { return       EEPROM.read(DiaChi); }

void LuuSoChon(int ThuTu){ EEPROM_WN(0,ThuTu); }
int DocSoChon(){ return EEPROM_RN(0); }
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
