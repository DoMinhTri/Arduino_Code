#include <AsmTinySerial.h>  

#define Nut1  0
#define Nut2  2
#define Nut3  3
#define Nut4  4

uint8_t TTG1    = 0;    
uint8_t TTG2    = 0; 
uint8_t TTG3    = 0; 
uint8_t TTG4    = 0;  

bool PhatHien_1   = false;    
bool PhatHien_2   = false; 
bool PhatHien_3   = false; 
bool PhatHien_4   = false; 
//////////////////////////////////////////////////////////////////////////////////
void SuLyNut_1()
{
    ///////////////////////
  TTG1 = digitalRead(Nut1);
  if( TTG1 == 1) { PhatHien_1 = true; }
  if(PhatHien_1)
  { 
      if(TTG1 == 0)
      {
          SerialTx( "1\n" );
          PhatHien_1   = false;
      }
  }
}
//////////////////////////////////////////////////////////////////////////////////
void SuLyNut_2()
{
  TTG2 = digitalRead(Nut2);
  if( TTG2 == 1) {  PhatHien_2 = true; }
  if(PhatHien_2)
  { 
      if(TTG2 == 0)
      {
          SerialTx( "2\n" );
          PhatHien_2   = false;
      }
  }
}
//////////////////////////////////////////////////////////////////////////////////
void SuLyNut_3()
{
  TTG3 = digitalRead(Nut3);
  if( TTG3 == 1) { PhatHien_3 = true; }
  if(PhatHien_3)
  { 
      if(TTG3 == 0)
      {
          SerialTx( "3\n" );
          PhatHien_3   = false;

      }
  }
}
//////////////////////////////////////////////////////////////////////////////////
void SuLyNut_4()
{
  TTG4 = digitalRead(Nut4);
  if( TTG4 == 1) {  PhatHien_4 = true; }
  if(PhatHien_4)
  { 
      if(TTG4 == 0)
      {
          SerialTx( "4\n" );
          PhatHien_4   = false;
      }
  }
}

void setup()
{
  SerialInit( PB1  , 115200 );

}

void loop()
{
   SuLyNut_1();
   SuLyNut_2();
   SuLyNut_3();
   SuLyNut_4();
}

