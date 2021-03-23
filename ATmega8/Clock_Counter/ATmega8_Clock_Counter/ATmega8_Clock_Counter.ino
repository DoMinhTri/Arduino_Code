

////////////////////////////////////////////////// EEPROM
#include <EEPROM.h>
//                               BEGIN -        COUNT 
//                 Type (Count - Timer)   (On/Off)       Begin(H)  Begin(m)   Count(H)   Count(m)
// Relay 1 bit:       1                    2             3         4          5          6 
// Relay 2 bit:       7                    8             9         10         11         12

int GiaTriLuu[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
void LuuLoadGiaTri(int Load_1_Save_2)
{
  for(int i=0;i<12;i++)
  {
    if(Load_1_Save_2 == 1) { GiaTriLuu[i] = EEPROM.read(i); }
    else                   { EEPROM.write(i, GiaTriLuu[i]); }
  }
}
////////////////////////////////////////////////// LCD
#include <ShiftRegLCD.h>
ShiftRegLCD srlcd(10, 9, TWO_WIRE,2);
////////////////////////////////////////////////// DS1307
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
//////////////////////////////////////////////////
char KyTu     ;
char MenuChon = 'n';
boolean ChinhSua = false;
int Relay1_TT, Relay2_TT, Relay, GioPhut  ;
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void setup () 
{
    Wire.begin();
    RTC.begin();
    if (! RTC.isrunning()){   RTC.adjust(DateTime(__DATE__, __TIME__));   }
    ///////////////////////
    LuuLoadGiaTri(1);
    MainMenu();
    ///////////////////////
    pinMode(2, INPUT_PULLUP); // Button 2
    pinMode(3, INPUT_PULLUP); // Button Minute    
    pinMode(4, INPUT_PULLUP); // Button Select         
    pinMode(5, INPUT_PULLUP); // Button Ok         
    pinMode(6, INPUT_PULLUP); // Button Hour
    pinMode(7, INPUT_PULLUP); // Button 1     
    
    pinMode(11, OUTPUT); // relay 1
    pinMode(12, OUTPUT); // relay 2
}

void loop () {
    DateTime now = RTC.now();
    KyTu = NutBam();
    srlcd.setCursor(0,1);
    srlcd.print(digitalRead(2));
    ////////////////////////////////////////////////////////////////////    
        if(ChinhSua == false)
        {
           if (KyTu=='1'||KyTu=='2'||KyTu=='s') MenuChon = KyTu;
           //////////////////
           switch (MenuChon) 
           {
              case 'n':  
                  MainMenu();
              break;
              ////////////////////////
              case '1':  
                  //srlcd.print(KyTu);
                  if(KyTu == '1') ChinhSua = true; 
               break;
               ////////////////////////
               case '2':  
                  //srlcd.print(KyTu);
                  if(KyTu == '2') ChinhSua = true; 
               break;                 
               ////////////////////////
               case 's':  
                  //srlcd.print(KyTu);
                  if(KyTu == 's') ChinhSua = true; 
               break;              
            }
        }
    ////////////////////////////////////////////////////////////////////        
    delay(1000);
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void MainMenu()
{
   MenuChon = 'n';
   ChinhSua = false;
   
   LCD2Dong("A","B");
}
//////////////////////////////////////////////////
void LCD2Dong(String STR1, String STR2)
{
  srlcd.setCursor(0,0);
  srlcd.print(STR1);
  srlcd.setCursor(0,1);
  srlcd.print(STR2);
}
char NutBam()
{
    if (digitalRead(2) == 0)
        return '2';
    else if (digitalRead(3) == 0)
        return 'm';
    else if (digitalRead(4) == 0)
        return 's';
    else if (digitalRead(5) == 0)
        return 'k';
    else if (digitalRead(6) == 0)
        return 'h';
    else if (digitalRead(7) == 0) 
        return 'l';

    return 'n';
    delay(50);
}
//////////////////////////////////////////////////
    /*
    lcd.begin(16, 2);
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    
    lcd.setCursor(0, 1);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    */
