// DMX RECEIVE USES ATMEGA328 or ATMEGA8-16PU
// connection information...
// http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
// use ATMEGA DigitalPin 1 to send DMX data to pin 4 of SN75176 chip
// SN75176: pins 2 & 5 to 0v | pin 3 & 8 to 5v | pin 6 to DMX+ | pin7 to DMX-
// Ensure 3.3v is used to supply NRF24L01 board !!
//
#include <SPI.h>
#include "nRF24L01.h" // library: https://github.com/maniacbug/RF24
#include "RF24.h"
#include <Wire.h>
#include "pins_arduino.h";

RF24 radio(9,10);

const uint64_t pipe = 0xF0F0F0F0E1LL;
#define MAXPAYLOAD 32 // max payload size for nrf24l01
#define DMXout 1      // DMX signal output pin (same as TX pin)
#define ERRout 8      // LED to show signal strength : digitalpin 8 to LED via 470 resistor to 0v
#define MAXGROUPS 17 // 17 groups of 30 channels = 510 channels

unsigned long time;
uint8_t payload[MAXPAYLOAD], DMXData[30*MAXGROUPS];
uint8_t lastStamp, channel=0, i;
unsigned long timer;

void setup(void)
{
  pinMode(DMXout, OUTPUT);
  digitalWrite(DMXout, HIGH);
  pinMode(ERRout, OUTPUT);
  digitalWrite(ERRout, LOW);
  radio.begin();
  radio.setAutoAck(false);
  radio.setPayloadSize(MAXPAYLOAD); // 9 for 6 byte packet, 12 for 6 byte data + SSD, needs to be 14 to stop failing !!
  radio.setPALevel(RF24_PA_HIGH); // try and set PA to max !   
  radio.setDataRate(RF24_250KBPS); // 250kbps for best tx/rx
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  radio.setChannel(channel);
}

void loop(void)
{
    if ( radio.available() ) {
      radio.read( payload, sizeof(payload) ); // get data packet from radio if available
    }
    if ( payload[1] == (lastStamp+1) ) { digitalWrite(ERRout,1); } // check for incontinuous timestamps, LED HI if good
    else { digitalWrite(ERRout,0); } // if time stamp error set LED low
    lastStamp = payload[1]; // reset last time stamp
    for (i = 0; i <30; i++) {
      DMXData[(30*payload[0])+i] = payload[i+2]; // recompile DMX data from RF data
    }
    if (payload[0] == MAXGROUPS-1) { // send DMX once last packet has been received
      digitalWrite(DMXout, LOW); 
      delay(10);     // send the break
      shiftDmxOut(DMXout, 0);     // send the start byte
      for (int count = 0; count <= 30*MAXGROUPS; count++){
        shiftDmxOut(DMXout, DMXData[count]);
      }
    }
}

void shiftDmxOut(int pin, int theByte){
  int port_to_output[] = { NOT_A_PORT, NOT_A_PORT, _SFR_IO_ADDR(PORTB), _SFR_IO_ADDR(PORTC), _SFR_IO_ADDR(PORTD) };
  int portNumber = port_to_output[digitalPinToPort(pin)];
  int pinMask = digitalPinToBitMask(pin);
  _SFR_BYTE(_SFR_IO8(portNumber)) |= pinMask;
  cli(); 
  _SFR_BYTE(_SFR_IO8(portNumber)) &= ~pinMask;  
  fourUSdelay(); 
  for (int i = 0; i < 8; i++){
    if (theByte & 01){ _SFR_BYTE(_SFR_IO8(portNumber)) |= pinMask; }
    else { _SFR_BYTE(_SFR_IO8(portNumber)) &= ~pinMask; }
    fourUSdelay();
    theByte >>= 1;
  }
  _SFR_BYTE(_SFR_IO8(portNumber)) |= pinMask; 
  sei();  
}

void fourUSdelay(void) {
    asm("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n");
    asm("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n");
    asm("nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n nop\n");
}
