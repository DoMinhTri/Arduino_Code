#include <Servo.h> 
///////////////////
#define IR_LED 13 
#define GiaTriLonNhatCuaDayTinHieu 128 
#define SoBuocSong 10 
#define CauTinHieu_IDLE 4000 
unsigned long MangSungNhip[GiaTriLonNhatCuaDayTinHieu]; 
unsigned char IR_SoByte[7]; 
int intIRtmp; 
int IR_SoNguyen; 
unsigned long longIRtmp; 
byte IR_KieuByte = B00000000; 
///////////////////
Servo C_SungLazer, ChanChon, ChanChon_1, ChanChon_2, ChanChon_3, ChanChon_4, ChanChon_5, ChanChon_6, ChanChon_7, ChanChon_8,C_1A,C_1B,C_2A,C_2B,C_3A,C_3B,C_4A,C_4B,C_5A,C_5B,C_6A,C_6B;  
///////////////////
#define IR_BIT_LENGTH 1
#define BIT_1 1000          //Binary 1 threshold (Microseconds)
#define BIT_0 400           //Binary 0 threshold (Microseconds)
#define BIT_START 2000      //Start bit threshold (Microseconds)
#define DEBUG 0             //Serial connection must be started to debug
int runtime_debug = 0;
int output_key = 0;
int power_button = 0;
///////////////////
//int led   = 12;
int MaRemote     =0 ;
///////////////////
int HanhDong     = 0;  // 1=Hanh dong   ---- 0= Khong lam gi
int TocDo        = 5;  // 
int KieuAuto     = 0;  // (0 = step) ---- (1= auto)
int GocQuay      = 0;  //
int ThoiGianCho  = 5;  //
int KieuHoatDong = 0;  // =1 nhom(1,2,3,4,5,6,7,8) -------  =2 nhom (11,22,33,44) --------------- =3 Nhom(41,42 - 4 tren hoac 4 duoi )
/////////////////////////
int ThuTuChanChon = 0;
int ToaDoChan[9]  = {0,0,0,0,0,0,0,0,0};
///////////////////
void setup() 
{ 
  Serial.begin(9600);
  pinMode(IR_LED,INPUT); 
 // pinMode(led, OUTPUT); 
  ////////////// 
  C_1A.attach(2); 
  C_2A.attach(3); 
  C_3A.attach(4); 
  C_4A.attach(5); 
  //C_5A.attach(6);
  //C_6A.attach(7); 
 
  C_1B.attach(6); 
  C_2B.attach(7); 
  C_3B.attach(8); 
  C_4B.attach(9); 
  //C_5B.attach(12); 
  //C_6B.attach(13);
 C_SungLazer.attach(10);
} 

//----------------------------------- 
void loop() { 
  
  MaRemote = GiaiMaRemote();
  if(MaRemote!=1)
  {
    //Serial.println(MaRemote);  
    CaiDatGiaTriHoatDong(MaRemote);
    ShowGiaTriTest(MaRemote);
    ///////////////////////////////////////
    //Serial.print(results.value, HEX);
    SungLazer(MaRemote);
  }
 ////////////
 if(HanhDong == 1)
 {
   ///////////////
   switch (KieuHoatDong) 
   {
      case 1:
            //Nhen_QuaPhai();
            break;  
      case 2:
            //Nhen_DiLui();
            break; 
      case 3:
            //Nhen_QuaTrai();
            break; 
      case 4:
            Nhen_DiToi();
            break;  
      case 5:
            Nhen_NgocDau();
            HanhDong = 0;
            break;
      case 6:
            Nhen_Nam();
            HanhDong = 0;
            break;      
      case 7:
            Nhen_NgocDich();
            HanhDong = 0;
            break;
      case 8:
            Nhen_Dung();
            HanhDong = 0;
            break;      
      //---      
     case 50:
           if(KieuAuto==1)
             ChanQuayTuDong();
           else
             Chan_1ChonQuayTheoDo(GocQuay);
           break;          
   }

 } 
   ///////////////
}
//-----------------------------------
int GiaiMaRemote()
{
  if(digitalRead(IR_LED)==LOW) 
  { 
  int count = 0; 
  int exit = 0; 
  while(!exit) 
  { 
    while(digitalRead(IR_LED)==LOW) 
    delayMicroseconds(SoBuocSong); 
    unsigned long start = micros(); 
    int max_high = 0; 
    while(digitalRead(IR_LED)==HIGH) 
    { 
      delayMicroseconds(SoBuocSong); 
      max_high += SoBuocSong; 
      if(max_high > CauTinHieu_IDLE) 
      { 
        exit = 1; 
        break; 
      } 
    } 

    unsigned long duration = micros()-start; 
    MangSungNhip[count++]=duration; 
  }

  for(int i=3;i<4;i++) 
  { 
    for(int j=0;j<8;j++) 
    { 
      if(MangSungNhip[i*8+j+1] > CauTinHieu_IDLE) 
      { 
        IR_SoByte[i] = IR_SoByte[i]>>1; 
        if((MangSungNhip[i*8+j+1])>1000 ) 
        { 
          IR_SoByte[i] = IR_SoByte[i]|0x80; 
        } 
      } 
      longIRtmp = MangSungNhip[i*8+j+1]; 
      if(longIRtmp<800) 
          intIRtmp = 10000000; 
      else 
          intIRtmp = 00000000; 
      IR_KieuByte = IR_KieuByte >> 1; 
      IR_KieuByte = IR_KieuByte + intIRtmp ; 
    } 
  } 
   IR_SoNguyen=int(IR_KieuByte); 
   //Serial.println(n); 
   return IR_SoNguyen;
  } 
}
//-----------------------------------
void CaiDatGiaTriHoatDong(int MaRIR)
{
   switch (MaRIR) 
   {
    //---------------------------- Chan le hoat dong
     case 12:
           ChanChon      = C_1A;
           HanhDong      = 1;
           KieuHoatDong  = 50;
           ThuTuChanChon = 0;
           GocQuay = ToaDoChan[0];
      break;
    ///////////
    case 24:
          ChanChon      = C_2A; 
          HanhDong      = 1;
          KieuHoatDong  = 50;
          ThuTuChanChon = 1;
          GocQuay = ToaDoChan[1];
      break;
    ///////////  
    case 94:
         ChanChon      = C_3A;
         HanhDong      = 1;
         KieuHoatDong  = 50;
         ThuTuChanChon = 2;
         GocQuay = ToaDoChan[2];
      break;
    ///////////  
    case 8:
         ChanChon      = C_4A;
         HanhDong      = 1;
         KieuHoatDong  = 50;
         ThuTuChanChon = 3;
         GocQuay = ToaDoChan[3];
      break;
    ///////////  
    case 28:
         ChanChon      = C_1B;
         HanhDong      = 1;
         KieuHoatDong  = 50;
         ThuTuChanChon = 4;
         GocQuay = ToaDoChan[4];
      break;
    ///////////  
    case 90:
         ChanChon      = C_2B;
         HanhDong      = 1;
         KieuHoatDong  = 50;
         ThuTuChanChon = 5;
         GocQuay = ToaDoChan[5];
      break;
    ///////////  
    case 66:
         ChanChon      = C_3B;
         HanhDong      = 1;
         KieuHoatDong  = 50;
         ThuTuChanChon = 6;
         GocQuay = ToaDoChan[6];
      break;
    ///////////  
    case 82:
         ChanChon      = C_4B;
         HanhDong      = 1;
         KieuHoatDong  = 50;
         ThuTuChanChon = 7;
         GocQuay = ToaDoChan[7];
      break;  
    //----------------------- Cac chi so
    case 25:
             GocQuay  = GocQuay + TocDo;
             if(GocQuay > 180) GocQuay = 180;
             KieuHoatDong = 50;
             HanhDong = 1;
      break;    
    /////////
    case 22:
             GocQuay  = GocQuay - TocDo;
             if(GocQuay < 0) GocQuay = 0;
             KieuHoatDong = 50;
             HanhDong = 1;
      break;     
    /////////
    case 21:
             ThoiGianCho = ThoiGianCho + 1;
      break;    
    /////////
    case 7:
             ThoiGianCho = ThoiGianCho - 1;
             if(ThoiGianCho<1) ThoiGianCho = 1;
      break;         
    //----------------------- Cac hanh dong
    case 9:
         TocDo = (TocDo + 1) % 101;
         if(TocDo==0) TocDo = 1;
      break;   
    /////////      
    case 70:
         KieuAuto = (KieuAuto + 1)%2;
      break;  
    //-----  lui - toi - trai - phai - default
    case 67:
         HanhDong     = 1;
         if(KieuHoatDong > 4) KieuHoatDong = 0;         
         KieuHoatDong = (KieuHoatDong + 1) % 5;
         if(KieuHoatDong == 0) KieuHoatDong = 1; 
      break;        
    /////////
    case 68:
         HanhDong     = 1;
         if(KieuHoatDong < 4) KieuHoatDong = 4;
         if(KieuHoatDong > 8) KieuHoatDong = 4;
         KieuHoatDong = (KieuHoatDong + 1) % 9;
         if(KieuHoatDong == 0) KieuHoatDong = 5;
      break;
    /////////
    case 64:
         HanhDong     = 1;
         KieuHoatDong = 4;  
      break;      
    /////////// default 
    case 13:
          HanhDong     = 0; // 1=Hanh dong   ---- 0= Khong lam gi
          TocDo        = 1; // 
          KieuAuto     = 0; // (0 = step) ---- (1= auto)
          GocQuay      = 0; //
          ThoiGianCho  = 5; //
          KieuHoatDong = 0; // =1 nhom(1,2,3,4,5,6,7,8) -------  =2 nhom (11,22,33,44) --------------- =3 Nhom(41,42 - 4 tren hoac 4 duoi )
          Nhen_Nam();
      break;      
     
  }
}
//-----------------------------------
void ChanQuayTuDong()
{
    for(int i=-45;i<=190;i++)
    {
      ChanChon.write(i);
      delay(ThoiGianCho);
    }
    //////////
    for(int i=190;i>=-45;i--)
    {
      ChanChon.write(i);
      delay(ThoiGianCho);
    }
}
//-----------------------------------
//-----------------------------------
void Chan_1ChonQuayTheoDo(int GocChonQuay) { Quay1Chan(ChanChon, ThuTuChanChon, GocChonQuay); };
//-----------------------------------
void SungLazer(int GTN)
{
  int DoQuay = random(30,180);//map(GTN, 0, 1023, 30, 180);
  Quay1Chan(C_SungLazer, 8, DoQuay);
}
//-----------------------------------
void Nhen_DiToi()
{
  //Quay4Chan_Dui(50,100,100,130);
 // Quay4Chan_Ban(180,180,0,0);
    //ThoiGianCho = 5; 
    Quay2Chan_15(40,150 );
    QuayChan_5(180);
    Quay2Chan_37( 40,30);
    QuayChan_7(   0);
    Quay2Chan_68(150,30 );
    Quay2Chan_13( 110,110);
    Quay2Chan_48(150,0 );
    Quay2Chan_26( 150,180);
    Quay2Chan_57( 150,30);
    Quay2Chan_24( 80,80);
}
//-----------------------------------
void Nhen_Dung()
{
  Quay4Chan_Dui(50,100,100,130);
  Quay4Chan_Ban(180,180,0,0);
}
//-----------------------------------
void Nhen_Nam()
{
  Quay4Chan_Dui(100,100,100,100);
  Quay4Chan_Ban(45,70,110,115);
}
//-----------------------------------
void Nhen_NgocDau()
{
  Quay4Chan_Dui(10,100,100,170);
  Quay4Chan_Ban(180,60,120,0);
}
//-----------------------------------
void Nhen_NgocDich()
{
  Quay4Chan_Dui(25,100,100,170);
  Quay4Chan_Ban(60,180,0,140);
}
//-----------------------------------
//-----------------------------------
void QuayChan_1(int TDo){ Quay1Chan(C_1A, 0, TDo) ;}
void QuayChan_2(int TDo){ Quay1Chan(C_2A, 1, TDo) ;}
void QuayChan_3(int TDo){ Quay1Chan(C_3A, 2, TDo) ;}
void QuayChan_4(int TDo){ Quay1Chan(C_4A, 3, TDo) ;}
void QuayChan_5(int TDo){ Quay1Chan(C_1B, 4, TDo) ;}
void QuayChan_6(int TDo){ Quay1Chan(C_2B, 5, TDo) ;}
void QuayChan_7(int TDo){ Quay1Chan(C_3B, 6, TDo) ;}
void QuayChan_8(int TDo){ Quay1Chan(C_4B, 7, TDo) ;}
//------
void Quay1Chan(Servo &Chan, int ThuTu, int ToaDo)
{
  int TDTam = ToaDoChan[ThuTu]; ToaDoChan[ThuTu] = ToaDo;
  while(TDTam != ToaDo)
  {
    if(TDTam > ToaDo) TDTam--;
    if(TDTam < ToaDo) TDTam++;
    Chan.write(TDTam);
    delay(ThoiGianCho);
  }
}
//------

void Quay2Chan_13(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_1A, 0, ToaDo_1, C_3A, 2, ToaDo_2); }
void Quay2Chan_15(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_1A, 0, ToaDo_1, C_1B, 4, ToaDo_2); }

void Quay2Chan_24(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_2A, 1, ToaDo_1, C_4A, 3, ToaDo_2); }
void Quay2Chan_26(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_2A, 1, ToaDo_1, C_2B, 5, ToaDo_2); }

void Quay2Chan_37(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_3A, 2, ToaDo_1, C_3B, 6, ToaDo_2); }
void Quay2Chan_46(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_4A, 3, ToaDo_1, C_2B, 5, ToaDo_2); }
void Quay2Chan_48(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_4A, 3, ToaDo_1, C_4B, 7, ToaDo_2); }
void Quay2Chan_57(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_1B, 4, ToaDo_1, C_3B, 6, ToaDo_2); }
void Quay2Chan_68(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_2B, 5, ToaDo_1, C_4B, 7, ToaDo_2); }
void Quay2Chan_56(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_1B, 4, ToaDo_1, C_2B, 5, ToaDo_2); }
void Quay2Chan_78(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_3B, 6, ToaDo_1, C_4B, 7, ToaDo_2); }


void Quay2Chan(Servo &_Chan_1, int TTu_1, int ToaDo_1, Servo &_Chan_2, int TTu_2, int ToaDo_2)
{
    int TDo_1 = ToaDoChan[TTu_1]; ToaDoChan[TTu_1] = ToaDo_1;
    int TDo_2 = ToaDoChan[TTu_2]; ToaDoChan[TTu_2] = ToaDo_2;
    
    int Dem_1 =1, Dem_2 =1;
    int QuayChanHoanTat = 0;
    
    if(TDo_1 > ToaDo_1) Dem_1 = -1; if(TDo_1 == ToaDo_1) { Dem_1 = 0; QuayChanHoanTat++; }
    if(TDo_2 > ToaDo_2) Dem_2 = -1; if(TDo_2 == ToaDo_2) { Dem_2 = 0; QuayChanHoanTat++; }
    while(QuayChanHoanTat !=2)
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
      _Chan_1.write(TDo_1);
      _Chan_2.write(TDo_2);
      delay(ThoiGianCho);
    }
}
//------
void Quay4Chan_Chan(int ToaDo_1, int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_2A, 1, ToaDo_1, C_4A, 3, ToaDo_2, C_2B, 5, ToaDo_3, C_4B, 7, ToaDo_4); }
void Quay4Chan_Le(int ToaDo_1  , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1A, 0, ToaDo_1, C_3A, 2, ToaDo_2, C_1B, 4, ToaDo_3, C_3B, 6, ToaDo_4); }
void Quay4Chan_Dui(int ToaDo_1 , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1A, 0, ToaDo_1, C_2A, 1, ToaDo_2, C_3A, 2, ToaDo_3, C_4A, 3, ToaDo_4); }
void Quay4Chan_Ban(int ToaDo_1 , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1B, 4, ToaDo_1, C_2B, 5, ToaDo_2, C_3B, 6, ToaDo_3, C_4B, 7, ToaDo_4); }
//------
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

//-----------------------------------
//-----------------------------------
void NhayDenLed()
{
   //digitalWrite(led, HIGH);  // turn the LED on (HIGH is the voltage level)
   //delay(100);               // wait for a second
   //digitalWrite(led, LOW);   // turn the LED off by making the voltage LOW
   //delay(100);  
}
//-----------------------------------
void ShowGiaTriTest(int MaRemote)
{
  char* _TrangThai[]   ={"Off","On"};
  char* _MangAuto[]    ={"Chinh tay","Tu dong"};
  char* _MangHoatDong[]={"","Qua phai","Di lui","Quan trai","Di toi","Ngoc dau","Nam","Doc dich","Dung","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Chon chan"};
  Serial.println("--------------------------------------------------------------------");
  Serial.print("Ma IR: ")         ;Serial.println(MaRemote);
  Serial.print("Hanh dong : ")    ;Serial.println(_TrangThai[HanhDong]);  
  Serial.print("Toc do: ")        ;Serial.println(TocDo);
  Serial.print("Auto: ")          ;Serial.println(_TrangThai[KieuAuto]);
  Serial.print("GocQuay: ")       ;Serial.println(GocQuay);
  Serial.print("Thoi gian cho: ") ;Serial.println(ThoiGianCho);
  Serial.print("Nhom chon : ")    ;Serial.println(_MangHoatDong[KieuHoatDong]);
}

//-----------------------------------
//////////////////////////
void read_pulse(int data[], int num_bits)
{
  for (int i = 0; i < num_bits; i++)
  {
    data[i] = pulseIn(IR_LED, LOW);
  }
}
//////////////////////////
void pulse_to_bits(int pulse[], int bits[], int num_bits)
{
  //Serial.println(num_bits);  
  if (DEBUG || runtime_debug) { Serial.println("-----"); }

  for(int i = 0; i < num_bits ; i++)
  {
    if (DEBUG || runtime_debug) { Serial.println(pulse[i]); }

    if(pulse[i] > BIT_1) //is it a 1?
    {
      bits[i] = 1;
    }
    else if(pulse[i] > BIT_0) //is it a 0?
    {
      bits[i] = 0;
    } 

    else //data is invalid...
    {
      //Serial.println("Error");
    }
  }
}
//////////////////////////
int bits_to_int(int bits[], int num_bits)
{
  int result = 0;
  int seed = 1;

  //Convert bits to integer
  for(int i = 0 ; i < num_bits ; i++)
  {
    if(bits[i] == 1)
    {
	result += seed;
    }

    seed *= 2;
  }

  return result;
}
//////////////////////////
int get_ir_key()
{
  int pulse[IR_BIT_LENGTH];
  int bits[IR_BIT_LENGTH];  

  do {} //Wait for a start bit
  while(pulseIn(IR_LED, LOW) < BIT_START);

  read_pulse(pulse, IR_BIT_LENGTH);

  pulse_to_bits(pulse, bits, IR_BIT_LENGTH);

  return bits_to_int(bits, IR_BIT_LENGTH);
}


