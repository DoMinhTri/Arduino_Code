int digit1 = 8;  //
int digit2 = 5; //
int digit3 = 3; //
int digit4 = A0; //

int segA = 7; //
int segB = 4; //
int segC = 12; //
int segD = 10; //
int segE = 9; //
int segF = 6; //
int segG = 13; //

#include <EEPROM.h>
int TThai = 0, Gio = 0, Phut = 0, Giay = 61, TGian = 0, TGBD = 0;

void setup() { 
    //Serial.begin(9600);
    pinMode(segA, OUTPUT);
    pinMode(segB, OUTPUT);
    pinMode(segC, OUTPUT);
    pinMode(segD, OUTPUT);
    pinMode(segE, OUTPUT);
    pinMode(segF, OUTPUT);
    pinMode(segG, OUTPUT);

    pinMode(digit1, OUTPUT);
    pinMode(digit2, OUTPUT);
    pinMode(digit3, OUTPUT);
    pinMode(digit4, OUTPUT);

    ///////////////
    pinMode(11, OUTPUT);// DPin
    digitalWrite(11, LOW);
    ///////////////    
    pinMode(2, OUTPUT);// Relay
    DongRelay();
    ///////////////    
    pinMode(A1, INPUT_PULLUP); // VALUE
    pinMode(A2, INPUT_PULLUP); // STATE
    Gio  = EEPROM.read(0);
    Phut = EEPROM.read(1);
    TThai = 0;
    TGBD = millis()/1000;
}

//////////////////////////////////////
void loop()
{
     if(digitalRead(A2) == LOW) 
     { 
         TThai++; 
         if(TThai > 3) TThai = 0; 
         delay(300); 
         DongRelay();
     }
     switch(TThai) 
     {
            case 0:
                DemGio();
                break;
            case 1:
                ChinhGio();
                break;
            case 2:
                ChinhPhut();
                break;
            case 3:
                CaiDat();
                break;
      }
     //////////////////////////////////////////     
}
///////////////////////////////////////////////////////////
void ChinhGio()
{
    DongRelay();
    if(digitalRead(A1) == LOW)
    {
        Gio++;
        if(Gio > 23) Gio = 0;
        delay(200);
    }
    dispNumber(Gio*100+Phut);    
}

void ChinhPhut()
{
    if(digitalRead(A1) == LOW)
    {
        Phut++;
        if(Phut > 59) Phut = 0;
        delay(200);
    } 
    dispNumber(Gio*100+Phut);   
}
void CaiDat()
{
       TThai = 0; 
       TGian = millis()/1000;
       TGBD  = TGian - 1;
       Giay  = 61;
       EEPROM.write(0,Gio);
       EEPROM.write(1,Phut);
       DongRelay();
       delay(100);
       //Serial.println("ok");
}
///////////////////////////////////////////////////////////
void DemGio()
{
       if(Gio*100+Phut > 0)
       {  
         MoRelay();
         dispNumber(Gio*100+Phut);
         if((TGian - TGBD) == 1) 
          { 
              TGBD=TGian; 
              Giay --;
              if(Giay == 0) Phut--;
              if(Phut < 0)
              {
                  Gio--;
                  Phut= 59;
              }
              
              if(Giay == 0) Giay = 60;
             delay(100);  
             //Serial.print(Gio);Serial.print(" : ");Serial.print(Phut);Serial.print(" : ");Serial.println(Giay);
          }
          TGian = millis()/1000;
       }
       else
       {
          DongRelay();
          dispNumber(0);
       } 
}
///////////////////////////////////////////////////////////
void DongRelay(){ digitalWrite(2,LOW); }
void MoRelay(){ digitalWrite(2,HIGH); }
///////////////////////////////////////////////////////////
void dispNumber(int toDisplay) {
    #define DISPLAY_BRIGHTNESS 500

    #define DIGIT_ON HIGH
    #define DIGIT_OFF LOW

    long beginTime = millis();

    for(int digit = 4 ; digit > 0 ; digit--) {

        //Turn on a digit for a short amount of time
        switch(digit) {
            case 1:
                digitalWrite(digit1, DIGIT_ON);
                break;
            case 2:
                digitalWrite(digit2, DIGIT_ON);
                break;
            case 3:
                digitalWrite(digit3, DIGIT_ON);
                break;
            case 4:
                digitalWrite(digit4, DIGIT_ON);
                break;
        }

        //Turn on the right segments for this digit
        ligNumber(toDisplay % 10);
        toDisplay /= 10;

        delayMicroseconds(DISPLAY_BRIGHTNESS); //Display this digit for a fraction of a second

        //Turn off all segments
        ligNumber(10);

        //Turn off all digits
        digitalWrite(digit1, DIGIT_OFF);
        digitalWrite(digit2, DIGIT_OFF);
        digitalWrite(digit3, DIGIT_OFF);
        digitalWrite(digit4, DIGIT_OFF);
    }

    while( (millis() - beginTime) < 10) ; //Wait for 20ms to pass before we paint the display again
}

void ligNumber(int numToDisplay) {

    #define SEGMENT_ON LOW
    #define SEGMENT_OFF HIGH

    switch (numToDisplay){

    case 0:
        digitalWrite(segA, SEGMENT_ON);
        digitalWrite(segB, SEGMENT_ON);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_ON);
        digitalWrite(segE, SEGMENT_ON);
        digitalWrite(segF, SEGMENT_ON);
        digitalWrite(segG, SEGMENT_OFF);
        break;

    case 1:
        digitalWrite(segA, SEGMENT_OFF);
        digitalWrite(segB, SEGMENT_ON);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_OFF);
        digitalWrite(segE, SEGMENT_OFF);
        digitalWrite(segF, SEGMENT_OFF);
        digitalWrite(segG, SEGMENT_OFF);
        break;

    case 2:
        digitalWrite(segA, SEGMENT_ON);
        digitalWrite(segB, SEGMENT_ON);
        digitalWrite(segC, SEGMENT_OFF);
        digitalWrite(segD, SEGMENT_ON);
        digitalWrite(segE, SEGMENT_ON);
        digitalWrite(segF, SEGMENT_OFF);
        digitalWrite(segG, SEGMENT_ON);
        break;

    case 3:
        digitalWrite(segA, SEGMENT_ON);
        digitalWrite(segB, SEGMENT_ON);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_ON);
        digitalWrite(segE, SEGMENT_OFF);
        digitalWrite(segF, SEGMENT_OFF);
        digitalWrite(segG, SEGMENT_ON);
        break;

    case 4:
        digitalWrite(segA, SEGMENT_OFF);
        digitalWrite(segB, SEGMENT_ON);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_OFF);
        digitalWrite(segE, SEGMENT_OFF);
        digitalWrite(segF, SEGMENT_ON);
        digitalWrite(segG, SEGMENT_ON);
        break;

    case 5:
        digitalWrite(segA, SEGMENT_ON);
        digitalWrite(segB, SEGMENT_OFF);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_ON);
        digitalWrite(segE, SEGMENT_OFF);
        digitalWrite(segF, SEGMENT_ON);
        digitalWrite(segG, SEGMENT_ON);
        break;

    case 6:
        digitalWrite(segA, SEGMENT_ON);
        digitalWrite(segB, SEGMENT_OFF);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_ON);
        digitalWrite(segE, SEGMENT_ON);
        digitalWrite(segF, SEGMENT_ON);
        digitalWrite(segG, SEGMENT_ON);
        break;

    case 7:
        digitalWrite(segA, SEGMENT_ON);
        digitalWrite(segB, SEGMENT_ON);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_OFF);
        digitalWrite(segE, SEGMENT_OFF);
        digitalWrite(segF, SEGMENT_OFF);
        digitalWrite(segG, SEGMENT_OFF);
        break;

    case 8:
        digitalWrite(segA, SEGMENT_ON);
        digitalWrite(segB, SEGMENT_ON);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_ON);
        digitalWrite(segE, SEGMENT_ON);
        digitalWrite(segF, SEGMENT_ON);
        digitalWrite(segG, SEGMENT_ON);
        break;

    case 9:
        digitalWrite(segA, SEGMENT_ON);
        digitalWrite(segB, SEGMENT_ON);
        digitalWrite(segC, SEGMENT_ON);
        digitalWrite(segD, SEGMENT_ON);
        digitalWrite(segE, SEGMENT_OFF);
        digitalWrite(segF, SEGMENT_ON);
        digitalWrite(segG, SEGMENT_ON);
        break;

    case 10:
        digitalWrite(segA, SEGMENT_OFF);
        digitalWrite(segB, SEGMENT_OFF);
        digitalWrite(segC, SEGMENT_OFF);
        digitalWrite(segD, SEGMENT_OFF);
        digitalWrite(segE, SEGMENT_OFF);
        digitalWrite(segF, SEGMENT_OFF);
        digitalWrite(segG, SEGMENT_OFF);
        break;
    }
}
