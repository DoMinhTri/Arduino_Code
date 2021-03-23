int digit1 = 12;  //-
int digit2 = A0; //-
int digit3 = A2; //-
int digit4 = 10; //-

int segA = 11; //-
int segB = A3; //-
int segC = 9; //-
int segD = 7; //-
int segE = 8; //-
int segF = A1; //-
int segG = 5; //-

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
    pinMode(6, OUTPUT);// DPin
    digitalWrite(6, LOW);
    ///////////////    
    pinMode(4, OUTPUT);// Relay
    DongRelay();
    ///////////////    
    pinMode(3, INPUT_PULLUP); // HOUR
    pinMode(2, INPUT_PULLUP);  // MINUTE
    pinMode(A5, INPUT_PULLUP); // OK
    pinMode(A4, INPUT_PULLUP); // SELECT
    Gio  = EEPROM.read(0);
    Phut = EEPROM.read(1);
    TThai = 0;
    TGBD = millis()/1000;
}

//////////////////////////////////////
void loop()
{
     if(digitalRead(A4) == LOW) 
     { 
         TThai++; 
         if(TThai > 1) TThai = 0; 
         delay(300); 
         DongRelay();
         //Serial.println("Chon");
     }
     if(digitalRead(A5) == LOW) 
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
     //////////////////////////////////////////
     if(TThai == 0)
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
     else
     {
          DongRelay();
         if(digitalRead(3) == LOW)
         {
             Gio++;
             if(Gio > 23) Gio = 0;
             delay(200);
         }
         if(digitalRead(2) == LOW)
        {
             Phut++;
             if(Phut > 59) Phut = 0;
             delay(200);
        } 
        //Serial.print(Gio);Serial.print(" : ");Serial.println(Phut);
        dispNumber(Gio*100+Phut);    
     }
     //////////////////////////////////////////     
}

void DongRelay(){ digitalWrite(4,HIGH); }
void MoRelay(){ digitalWrite(4,LOW); }


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
