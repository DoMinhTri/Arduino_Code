///////////////////////////////
#include <Wire.h>
#include "HMC5883L.h"
HMC5883L compass;
///////////////////////////////
#include <SoftwareSerial.h>
SoftwareSerial Wifi(A0, A1); // RX | TX
///////////////////////////////
#define Led    2
///////////////////////////////
#define Trig   3
#define Echo   4
///////////////////////////////
#define CoiHu  5
///////////////////////////////
#define Line_1 6
#define Line_2 7
#define Line_3 8
#define Line_4 9
///////////////////////////////
#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0
uint8_t TocDo = 0;
#define Xe_1_P 10
#define Xe_1   12
#define Xe_2   11
#define Xe_2_P 13
///////////////////////////////
#include <Servo.h>
Servo Servo_Duoi;
Servo Servo_Tren;

#define Servo_P1 A2
#define Servo_P2 A3
///////////////////////////////
#define Pin_Vol A6
float vout = 0.0;
float vin  = 0.0;
float R1   = 100000.0; // resistance of R1 (100K) 
float R2   = 10000.0;  // resistance of R2 (10K) 

void KiemTraVol()
{
     int value = analogRead(Pin_Vol);
     vout      = (value * 5.0) / 1024.0; 
     vin       = vout / (R2/(R1 + R2)); 
     //if (vin <= 10.0){ analogWrite(DenLed, 50); Serial.println("Yeu pin"); } else { analogWrite(DenLed, 0); }
     Serial.println(vin);
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
String WifiUART    = "";

int GocBanDau      = -1;
uint8_t CP4BanDau;
int GocHienTai     = 0;
uint8_t CP4HienTai;
int GocLech        = 0;
uint8_t GocLechMax = 30;

void XeChayTuDong()
{
    uint8_t Huong  = 2;
    GocHienTai =  LayGocQuay();
    CP4HienTai = GocHienTai/90 + 1;
    GocLech = GocHienTai - GocBanDau;
    /////////////////////
    if (GocHienTai > GocBanDau + GocLechMax)
    {
    }
    //////////////////////////
    //XeChayDinhHuong(Huong,100);
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  Wifi.begin(9600);
  /////////////////////////////  Khoảng cách
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  //////////////////////////
  pinMode(Led,   OUTPUT);
  pinMode(CoiHu, OUTPUT);

  pinMode(Line_1, INPUT);
  pinMode(Line_2, INPUT);
  pinMode(Line_3, INPUT);
  pinMode(Line_4, INPUT);

  pinMode(Xe_1_P, OUTPUT);
  pinMode(Xe_1,   OUTPUT);
  pinMode(Xe_2_P, OUTPUT);
  pinMode(Xe_2,   OUTPUT);

  Servo_Duoi.attach(A2);
  Servo_Tren.attach(A3);

  Servo_Duoi.write(90);
  Servo_Tren.write(90);
  
  pinMode(Pin_Vol, INPUT);

  /////////////////////// La bàn
  while (!compass.begin()){   delay(500);  }
 
  // Ustawienie zakresu pomiarowego
  // +/- 0.88 Ga: HMC5883L_RANGE_0_88GA
  // +/- 1.30 Ga: HMC5883L_RANGE_1_3GA (domyslny)
  // +/- 1.90 Ga: HMC5883L_RANGE_1_9GA
  // +/- 2.50 Ga: HMC5883L_RANGE_2_5GA
  // +/- 4.00 Ga: HMC5883L_RANGE_4GA
  // +/- 4.70 Ga: HMC5883L_RANGE_4_7GA
  // +/- 5.60 Ga: HMC5883L_RANGE_5_6GA
  // +/- 8.10 Ga: HMC5883L_RANGE_8_1GA
  compass.setRange(HMC5883L_RANGE_1_3GA);
 
  // Ustawienie trybu pracy
  // Uspienie:              HMC5883L_IDLE
  // Pojedynczy pomiar:     HMC5883L_SINGLE
  // Ciagly pomiar: HMC5883L_CONTINOUS (domyslny)
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
 
  // Ustawienie czestotliwosci pomiarow
  //  0.75Hz: HMC5883L_DATARATE_0_75HZ
  //  1.50Hz: HMC5883L_DATARATE_1_5HZ
  //  3.00Hz: HMC5883L_DATARATE_3HZ
  //  7.50Hz: HMC5883L_DATARATE_7_50HZ
  // 15.00Hz: HMC5883L_DATARATE_15HZ (domyslny)
  // 30.00Hz: HMC5883L_DATARATE_30HZ
  // 75.00Hz: HMC5883L_DATARATE_75HZ
  compass.setDataRate(HMC5883L_DATARATE_15HZ);
 
  // Liczba usrednionych probek
  // 1 probka:  HMC5883L_SAMPLES_1 (domyslny)
  // 2 probki: HMC5883L_SAMPLES_2
  // 4 probki: HMC5883L_SAMPLES_4
  // 8 probki: HMC5883L_SAMPLES_8
  compass.setSamples(HMC5883L_SAMPLES_1);
  
  if(GocBanDau < 0)
  {  
      GocBanDau =  LayGocQuay(); 
      CP4BanDau = GocBanDau/90 + 1;
  } 
  delay(1000);
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void loop() {

   XeChayTuDong();
   //KiemTraVol();
   //KhoangCach();
   //BanhTrai_Lui();
   //BanhTrai_Toi();
   //BanhPhai_Lui();
   //BanhPhai_Toi();
   while (Wifi.available() > 0 ) 
   {
        char KyTu = Wifi.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 )
        { 
          if(nbKyTu != 35){ WifiUART += KyTu; } else{break;}// # char = 35
        }
        delay(5);
   }
   /////////////
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  
       if(KyTu == 'S')      { Xe_Ngung();      }
       else if(KyTu == 'F') { Xe_Toi();        }
       else if(KyTu == 'B') { Xe_Lui();        }
       
       else if(KyTu == 'L') { XeQuay_Trai();   }
       else if(KyTu == 'R') { XeQuay_Phai();   }

       else if(KyTu == 'I') { XeToi_Phai();    }
       else if(KyTu == 'G') { XeToi_Trai();    }
       
       else if(KyTu == 'J') { XeLui_Phai();    }
       else if(KyTu == 'H') { XeLui_Trai();    }

       else if(KyTu == 'X') { CuoiDauChao();   }
       else if(KyTu == 'x') { CuoiDauChao();   }
       else if(KyTu == 'V') { LacDau();   }
       else if(KyTu == 'v') { LacDau();   }
       else
      {
          uint8_t MucTocDo = KyTu - 48;
          if(MucTocDo > 9){  MucTocDo = 9;   }
          TocDo = MucTocDo; 
          //Serial.println(KyTu);
      }
   }
  /////////////
  
  //Servo_Duoi.write(90 + GocLech);
  Serial.print(GocBanDau);  Serial.print(" _ "); Serial.print(CP4BanDau);  Serial.print(" _*_ ");  
  Serial.print(GocHienTai); Serial.print(" _ "); Serial.print(CP4HienTai); Serial.print("\n");
  Serial.println(KhoangCach());
  delay(1000);
}



int LaySoDuong(int So)
{
  if(So <0 ) So = -So;
  return So;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////   Đo khoảng cách   ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
int KhoangCach()
{
  long duration, inches, cm;
  
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  duration = pulseIn(Echo, HIGH);
  
  cm     = microsecondsToCentimeters(duration);
  inches = microsecondsToInches(duration);
  return cm;
}

long microsecondsToInches(long microseconds){  return microseconds / 74 / 2; }
long microsecondsToCentimeters(long microseconds){  return microseconds / 29 / 2; }
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////   La bàn   /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
float  declinationAngle = 0.212;        // Radians
int LayGocQuay () 
{
  Vector raw = compass.readRaw();
  Vector norm = compass.readNormalize();
  float heading = atan2(raw.YAxis, raw.XAxis) + declinationAngle;
  if(heading < 0)      heading += 2 * PI;
  if(heading > 2 * PI) heading -= 2 * PI;
  return (int)  (heading * 180 / M_PI); 
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Servo        /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void CuoiDauChao()
{
  Servo_Tren.write(90);  delay(500);
  Servo_Tren.write(180); delay(1000);
  Servo_Tren.write(90);  delay(500);
}

void LacDau()
{
  Servo_Duoi.write(90);  delay(500);
  Servo_Duoi.write(180); delay(500);
  Servo_Duoi.write(0);   delay(500);
  Servo_Duoi.write(90);  delay(500);
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////   Xe Dieu Khien   /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void XeChayDinhHuong(uint8_t Huong, int ThoiGian)
{
  if(Huong == 0) Xe_Ngung();
  if(Huong == 1) XeQuay_Trai();
  if(Huong == 2) Xe_Toi();
  if(Huong == 3) XeQuay_Phai();
  delay(ThoiGian);
}
///////////////////////
void XeQuay_Phai()
{
  BanhTrai_Toi();
  BanhPhai_Lui();
}
///////////////////////
void XeQuay_Trai()
{
  BanhPhai_Toi();
  BanhTrai_Lui();  
}
///////////////////////
void XeToi_Phai()
{
  BanhPhai_Dung();
  BanhTrai_Toi();
}
///////////////////////
void XeToi_Trai()
{
  BanhTrai_Dung();
  BanhPhai_Toi();
}
///////////////////////
void XeLui_Trai()
{
  BanhTrai_Dung();
  BanhPhai_Lui();
}
///////////////////////
void XeLui_Phai()
{
  BanhPhai_Dung(); 
  BanhTrai_Lui();
}

///////////////////////
void Xe_Toi()
{
  BanhTrai_Toi();
  BanhPhai_Toi();
}
///////////////////////
void Xe_Lui()
{
  BanhTrai_Lui();
  BanhPhai_Lui();
}
///////////////////////
void Xe_Ngung()
{
  BanhTrai_Dung();
  BanhPhai_Dung();
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void BanhPhai_Dung() {
  digitalWrite(Xe_1, LOW);
  digitalWrite(Xe_1_P, LOW);
}
 
void BanhTrai_Dung() {
  digitalWrite(Xe_2, LOW);
  digitalWrite(Xe_2_P, LOW);
}
///////////////////////
void BanhTrai_Lui()
{
  TocDo = constrain(TocDo, 0, 255);
  digitalWrite(Xe_1, LOW);
  analogWrite(Xe_1_P, 255); // 255
}
///////////////////////
void BanhTrai_Toi()
{
  TocDo = constrain(TocDo, 0, 254);
  digitalWrite(Xe_1, HIGH);
  analogWrite(Xe_1_P, 254); // 0 - 254
}
///////////////////////
void BanhPhai_Lui()
{
  TocDo = constrain(TocDo, 0, 127);
  digitalWrite(Xe_2, HIGH);
  analogWrite(Xe_2_P, 10); // 0 - 127
}
///////////////////////
void BanhPhai_Toi()
{
  TocDo = constrain(TocDo, 128, 255);
  digitalWrite(Xe_2, LOW);
  analogWrite(Xe_2_P, 255); // 128 - 255
}
