/*
How to use/install:
1. connect M24SR
   Pinout:
   -------------------------------------------------------------------------------
   M24SR             -> Arduino / resistor / antenna
   -------------------------------------------------------------------------------
   1 RF disable      -> TODO not used
   2 AC0 (antenna)   -> Antenna
   3 AC1 (antenna)   -> Antenna
   4 VSS (GND)       -> Arduino Gnd
   5 SDA (I2C data)  -> Arduino A4
   6 SCL (I2C clock) -> Arduino A5
   7 GPO             -> Arduino D7 + Pull-Up resistor (>4.7kOhm) to VCC
   8 VCC (2...5V)    -> Arduino 5V
   -------------------------------------------------------------------------------
*/



#include <I2Clib.h>
#include <Wire.h>



void setup() 
{
  Serial.begin(9600);

  ADPCFG = 0xFFFF;           // initialize AN pins as digital
  LATB   = 0;
  TRISB  = 0;                // Configure PORTB as output

  I2C1_Init(100000);         // initialize I2C communication
  I2C1_Start();              // issue I2C start signal
  I2C1_Write(0xA2);          // send byte via I2C  (device address + W)
  I2C1_Write(2);             // send byte (address of EEPROM location)
  I2C1_Write(0xF0);          // send data (data to be written)
  I2C1_Stop();               // issue I2C stop signal

  Delay_100ms();

  I2C1_Start();              // issue I2C start signal
  I2C1_Write(0xA2);          // send byte via I2C  (device address + W)
  I2C1_Write(2);             // send byte (data address)
  I2C1_Restart();            // issue I2C signal repeated start
  I2C1_Write(0xA3);          // send byte (device address + R)
  LATB = I2C1_Read(1u);      // Read the data (NO acknowledge)
  I2C1_Stop();               // issue I2C stop signal
}

void loop() 
{
}

 
