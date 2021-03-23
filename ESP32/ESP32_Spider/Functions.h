#include <ESP32_Servo.h>
Servo C_1A, C_1B, C_2A, C_2B, C_3A, C_3B, C_4A, C_4B;  

//  1B  1A  -  4A 4B
//  2B  2A  -  3A 3B
uint8_t KieuHoatDong = 0;

int ThuTuChanChon = 0;
int ThoiGianCho   = 5;  
int ToaDoChan[9]  = {0,0,0,0,0,0,0,0,0};
//////////////////////////////////////////////////////
void Nhen_Dung()
{
  C_1B.write(0);   // C_1B
  C_1A.write(0);   // C_1A
  
    
  C_2A.write(180); // C_2A
  C_2B.write(0);   // C_2B

  C_3A.write(0);   // C_3A
  C_3B.write(180); // C_3B

  C_4A.write(180); // C_4A
  C_4B.write(180); // C_4B
}
//////////////////////////////////////////////////////
void Chan1B_U(){ C_1B.write(180); } void Chan1B_C(){ C_1B.write(90); } void Chan1B_D(){ C_1B.write(0);   }
void Chan1A_U(){ C_1A.write(0);   } void Chan1A_C(){ C_1A.write(90); } void Chan1A_D(){ C_1A.write(180); }

void Chan2B_U(){ C_2B.write(180); } void Chan2B_C(){ C_2B.write(90); } void Chan2B_D(){ C_2B.write(0);   }
void Chan2A_U(){ C_2A.write(0);   } void Chan2A_C(){ C_2A.write(90); } void Chan2A_D(){ C_2A.write(180); }

void Chan3B_U(){ C_3B.write(0);   } void Chan3B_C(){ C_3B.write(90); } void Chan3B_D(){ C_3B.write(180); }
void Chan3A_U(){ C_3A.write(0);   } void Chan3A_C(){ C_3A.write(90); } void Chan3A_D(){ C_3A.write(180); }


void Nhen_DiToi()
{
    C_4B.write(90);  delay(200);
    C_4A.write(180); delay(200);
    C_4B.write(180); delay(200);
    
    C_2B.write(90);  delay(200);
    C_2A.write(90);  delay(200);
    C_2B.write(0);   delay(200);

    C_4A.write(90);  delay(200);
    C_2A.write(180); delay(200);
    ////////////
    C_1A.write(0);   delay(1000);
    C_3A.write(90);  delay(1000);
    C_1A.write(90);  delay(1000);
    C_3A.write(0);   delay(1000);
}
//////////////////////////////////////////////////////
void VeToaDoChon_4(Servo &_Chan_1, int TTu_1, int ToaDo_1, Servo &_Chan_2, int TTu_2, int ToaDo_2, Servo &_Chan_3, int TTu_3, int ToaDo_3, Servo &_Chan_4, int TTu_4, int ToaDo_4)
{
    int TDo_1 = ToaDoChan[TTu_1]; ToaDoChan[TTu_1] = ToaDo_1;
    int TDo_2 = ToaDoChan[TTu_2]; ToaDoChan[TTu_2] = ToaDo_2;
    int TDo_3 = ToaDoChan[TTu_3]; ToaDoChan[TTu_3] = ToaDo_3;
    int TDo_4 = ToaDoChan[TTu_4]; ToaDoChan[TTu_4] = ToaDo_4;
    
    int Dem_1 =1, Dem_2 =1, Dem_3 =1, Dem_4 =1;
    int QuayChanHoanTat = 0;
    
    if(TDo_1 > ToaDo_1) Dem_1 = -1; if(TDo_1 == ToaDo_1) { Dem_1 = 0; QuayChanHoanTat++; }
    if(TDo_2 > ToaDo_2) Dem_2 = -1; if(TDo_2 == ToaDo_2) { Dem_2 = 0; QuayChanHoanTat++; }
    if(TDo_3 > ToaDo_3) Dem_3 = -1; if(TDo_3 == ToaDo_3) { Dem_3 = 0; QuayChanHoanTat++; }
    if(TDo_4 > ToaDo_4) Dem_4 = -1; if(TDo_4 == ToaDo_4) { Dem_4 = 0; QuayChanHoanTat++; }
    
    while(QuayChanHoanTat !=4)
    {
      //----------
      if(Dem_1 != 0) 
      {
        TDo_1 =  TDo_1 + Dem_1 ;
        if(TDo_1 == ToaDo_1) { Dem_1 = 0; QuayChanHoanTat++; } 
      }
      //----------
      if(Dem_2 != 0) 
      {
        TDo_2 =  TDo_2 + Dem_2 ;
        if(TDo_2 == ToaDo_2) { Dem_2 = 0; QuayChanHoanTat++; } 
      }
      //----------
      if(Dem_3 != 0) 
      {
        TDo_3 =  TDo_3 + Dem_3 ;
        if(TDo_3 == ToaDo_3) { Dem_3 = 0; QuayChanHoanTat++; } 
      }      
      //----------
      if(Dem_4 != 0) 
      {
        TDo_4 =  TDo_4 + Dem_4 ;
        if(TDo_4 == ToaDo_4) { Dem_4 = 0; QuayChanHoanTat++; } 
      }  
      //----------      
      _Chan_1.write(TDo_1);
      _Chan_2.write(TDo_2);
      _Chan_3.write(TDo_3);
      _Chan_4.write(TDo_4);
      delay(ThoiGianCho);
    }
}
//////////////////////////////////////////////////////
void Quay4Chan_Chan(int ToaDo_1, int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_2A, 1, ToaDo_1, C_4A, 3, ToaDo_2, C_2B, 5, ToaDo_3, C_4B, 7, ToaDo_4); }
void Quay4Chan_Le(int ToaDo_1  , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1A, 0, ToaDo_1, C_3A, 2, ToaDo_2, C_1B, 4, ToaDo_3, C_3B, 6, ToaDo_4); }
void Quay4Chan_Dui(int ToaDo_1 , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1A, 0, ToaDo_1, C_2A, 1, ToaDo_2, C_3A, 2, ToaDo_3, C_4A, 3, ToaDo_4); }
void Quay4Chan_Ban(int ToaDo_1 , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1B, 4, ToaDo_1, C_2B, 5, ToaDo_2, C_3B, 6, ToaDo_3, C_4B, 7, ToaDo_4); }
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
void Nhen_Nam()
{
  Quay4Chan_Dui(100,100,100,100);
  Quay4Chan_Ban(45,70,110,115);
}
//////////////////////////////////////////////////////
void Nhen_NgocDau()
{
  Quay4Chan_Dui(10,100,100,170);
  Quay4Chan_Ban(180,60,120,0);
}
//////////////////////////////////////////////////////
void Nhen_NgocDich()
{
  Quay4Chan_Dui(25,100,100,170);
  Quay4Chan_Ban(60,180,0,140);
}
//////////////////////////////////////////////////////
