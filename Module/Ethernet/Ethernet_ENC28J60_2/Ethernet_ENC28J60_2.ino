// CS     10
// MOSI	  11
// MISO	  12
// SCK	  13


#include "etherShield.h"
#include "ETHER_28J60.h" //simplified lib do Simon Monk
#include <stdio.h>

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};
static uint8_t ip[4] = {192, 168, 1, 15}; //alterar por um IP válido na rede local
static uint16_t port = 80;

ETHER_28J60 e;
#define maxrelays 10 //máximo de relay a controlar + 2

void setup()
{ 
  e.setup(mac, ip, port);
  for(int(relay)=2;relay<(maxrelays);relay++) //começo no 2 para não usar os pino rx e tx do arduino
  {
    pinMode(relay,OUTPUT);
    digitalWrite(relay,HIGH); //o relé desliga com o pino a HIGH
  }
}

void loop()
{
  char* params;
  while (params = e.serviceRequest())
  {
    e.print("<html><head><title>Web Controlled Relays</title></head><body><table  border=\"1\" width=\"20%\"><tr><td>Relay</td><td>Status</td>");
    for(int(relay)=2;relay<(maxrelays);relay++)
    {
      char resp[3];
      sprintf(resp, "%s%d","?", relay);
      if (strcmp(params,resp) == 0) digitalWrite(relay,!digitalRead(relay));
      if (strcmp(params,"?allon") == 0) for (int(relay)=2;relay<maxrelays;relay++) digitalWrite(relay,LOW);
      if (strcmp(params,"?alloff") == 0) for (int(relay)=2;relay<maxrelays;relay++) digitalWrite(relay,HIGH);
      e.print("<tr><td>");
      e.print(relay);
      e.print("</td><td><a href=?");
      e.print(relay);
      e.print(">");
      if (digitalRead(relay)==LOW) e.print("ON");
      if (digitalRead(relay)==HIGH) e.print("OFF");
      e.print("</a></td></tr>");      
    }
    e.print("</table><br><a href=\"?allon\">All On</a><br><a href=\"?alloff\">All Off</a></body></html>");
    e.respond();
  }
}
