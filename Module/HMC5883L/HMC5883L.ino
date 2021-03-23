#include <Wire.h>
#include "HMC5883L.h"
 
HMC5883L compass;
 
void setup()
{
  Serial.begin(9600);
 
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
}
 
void loop()
{
  Vector raw = compass.readRaw();
  Vector norm = compass.readNormalize();
 
  Serial.print(" Xraw = ");  Serial.print(raw.XAxis);
  Serial.print(" Yraw = ");  Serial.print(raw.YAxis);
  Serial.print(" Zraw = ");  Serial.print(raw.ZAxis);
  Serial.print(" <==> "); 
  Serial.print(" Xnorm = ");  Serial.print(norm.XAxis);
  Serial.print(" Ynorm = ");  Serial.print(norm.YAxis);
  Serial.print(" ZNorm = ");  Serial.print(norm.ZAxis);
  Serial.println();  
 
  delay(100);
}


float    declinationAngle = 0.212;    // Radians
int getDegrees () 
{
  //MagnetometerScaled scaled = compass.ReadScaledAxis();
  
  Vector raw = compass.readRaw();
  Vector norm = compass.readNormalize();
  
   // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(raw.YAxis, raw.XAxis) + declinationAngle;
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2 * PI;
  // Check for wrap due to addition of declination.
  if(heading > 2 * PI)
    heading -= 2 * PI;
  // Convert radians to degrees for readability.
  return (int)  (heading * 180 / M_PI); 
}
