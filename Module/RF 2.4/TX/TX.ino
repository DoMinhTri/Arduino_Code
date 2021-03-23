#include "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
#define VRx A0
#define VRy A1

int VRxPos = 90;
int VRyPos = 90;
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup()
{
    radio.begin();
    radio.openWritingPipe(pipe);
}

void loop()
{
    VRxPos = analogRead(VRx);
    VRxPos = map(VRxPos, 0, 1023, 0, 180);
    VRyPos = analogRead(VRy);
    VRyPos = map(VRyPos, 0, 1023, 0, 180);
    if((VRxPos==88) && (VRyPos==85))
    {
        msg[0] = 111;
        radio.write(msg, 1);
    } else {
        msg[0] = 112;
        radio.write(msg, 1);
    }
}
