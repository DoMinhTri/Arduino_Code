#include <Servo.h>
#include <IRremote.h>
Servo C_SungLazer;
int ToaDoChan[9]  = {0,0,0,0,0,0,0,0,0};
/////////////////////////////
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
/////////////////////////////
void setup() 
{ 
  pinMode(IR_LED,INPUT); 
  Serial.begin(9600); 
  C_SungLazer.attach(10);
}
/////////////////////////////
void loop() 
{ 
    //int GT = Trave.value ;
    int MaRemote = GiaiMaRemote();
    //MaRemote = random(2,4);
    if(MaRemote!=1)
    {
      Serial.println(MaRemote);
      int Do = random(0,180);
      SungLazer(Do);
      //Serial.print(" - ");
      //Serial.println(MaRemote);
    }
    //delay(MaRemote*1000);
} 
/////////////////////////////
void Quay1Chan(Servo &Chan, int ThuTu, int ToaDo)
{
  int TDTam = ToaDoChan[ThuTu]; ToaDoChan[ThuTu] = ToaDo;
  while(TDTam != ToaDo)
  {
    if(TDTam > ToaDo) TDTam--;
    if(TDTam < ToaDo) TDTam++;
    Chan.write(TDTam);
    delay(5);
  }
}
/////////////////////////////
void SungLazer(int GTN)
{
  int DoQuay = random(30,180);//map(GTN, 0, 1023, 30, 180);
  Quay1Chan(C_SungLazer, 8, DoQuay);
}
/////////////////////////////
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

