
#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 6, 8, 7, 4, 3);
//////////////////////////////////////
#include <DS1307.h>
DS1307 rtc(A4, A5);
//////////////////////////////////////
#define NutChon A3
#define NutOk   A2
#define TinHieu A1
#define Chuong  2
//////////////////////////////////////
void setup() 
{
    Serial.begin(9600);
    ////////////////////  
    pinMode(NutChon, INPUT_PULLUP);
    pinMode(NutOk, INPUT_PULLUP);
    pinMode(Chuong, OUTPUT);
    pinMode(TinHieu, OUTPUT);
    
    // Setup LCD to 16x2 characters
    lcd.begin(16, 2);
    ////////////////////
    // Set the clock to run-mode
    rtc.halt(false);
    rtc.setDOW(TUESDAY);         // Set Day-of-Week to SUNDAY
    rtc.setTime(15, 59, 40);     // Set the time to 12:00:00 (24hr format)
    rtc.setDate(27,  1, 2015);   // Set the date to October 3th, 2010
    
    // Set SQW/Out rate to 1Hz, and enable SQW
    rtc.setSQWRate(SQW_RATE_1);
    rtc.enableSQW(true);
    //////////////////////////
}
/////////////////////////////////////////////////////////////////////////////////////  
char KyTu = 'b'; 
String Chuoi_1,Chuoi_2;
void loop() 
{
     DongHo();
     KyTu = 'b'; 
    ////////////////////////    
    int  BamCHON = digitalRead(NutChon);
    int  BamOK   = digitalRead(NutOk);
    if(BamCHON == LOW) KyTu = '1';
    if(BamOK   == LOW) KyTu = '2';

    ////////////////////////  
    switch (KyTu) 
    {
        case '1': // 1.ID + 
        digitalWrite(Chuong, HIGH);
        break;
        /////////////////////////////////      
        case '2': // 2.View 
        digitalWrite(TinHieu, HIGH);

        break;
        /////////////////////////////////                
        case 'b': // 
        digitalWrite(TinHieu, LOW);    
        digitalWrite(Chuong, LOW);
        break;
    }
    //////////////////
}
/////////////////////////////////////////////////////////////////////////////////////  
void DongHo()
{
  // Display time centered on the upper line
  lcd.setCursor(4, 0);
  lcd.print(rtc.getTimeStr());
  
  // Display abbreviated Day-of-Week in the lower left corner
  lcd.setCursor(0, 1);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  
  // Display date in the lower right corner
  lcd.setCursor(6, 1);
  lcd.print(rtc.getDateStr());

  // Wait one second before repeating :)
  delay (1000);
}
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

void NhapNhay(int ViTri, int Dong)
{
    lcd.setCursor(ViTri - 1, Dong - 1);
    lcd.cursor();
}
