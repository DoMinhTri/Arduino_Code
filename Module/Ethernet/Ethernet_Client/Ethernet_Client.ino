#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h> 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
IPAddress ip(192,168,1,15); 
EthernetServer server(80); 
EthernetClient client; 

boolean tieude = true;

int nLed = 2;

void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  
  pinMode(nLed,OUTPUT);
  
  Serial.println("Server started");
}

void loop()
{
  client = server.available();
  
  if (client) 
  {
    String commandStr ="";
    while (client.connected()) 
    {
     
      if (client.available()) 
      {
        kk();
        char c = client.read();
        commandStr+=c;
        Serial.println(commandStr);
        if (c == '*') 
        {
          String value= commandStr.substring(0,commandStr.length()-1);
          if(value == "ok") digitalWrite(nLed, LOW);
          if(value == "no") digitalWrite(nLed, HIGH);
          Serial.println(value);
          commandStr="";
        }
      }
    }
    delay(1);
    client.stop();
  }
}

int convertToInt(String value){
  char buf[value.length()];
  value.toCharArray(buf,value.length());
  return atoi(buf);
}


void kk()
{
     if(tieude)
      {
        client.println("HTTP/1.1 200 OK"); //send new page
        client.println("Content-Type: text/html");
        client.println();
         client.println("HTML");
       }
         tieude = false;       
}
