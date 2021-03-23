// CS     10
// MOSI	  11
// MISO	  12
// SCK	  13

#include <SPI.h>
#include <Ethernet.h>
 
byte mac[]     = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // physical mac address
byte ip[]      = { 192, 168, 1, 123};                    // ip in lan
byte gateway[] = { 192, 168, 1, 1};                      // internet access via router
byte subnet[]  = { 255, 255, 255, 0};                    // subnet mask
EthernetServer server(80);                               // server port
String readString;
/////////////////////////////////////////////////
#include "IRremote.h"
int receiver = 5; 
IRrecv irrecv(receiver);        
decode_results results;  
/////////////////////////////////////////////////
int Relay_1 = 2;
int Relay_2 = 3;
/////////////////////////////////////////////////
int Remote_a = 9;
int Remote_b = 8;

/////////////////////////////////////////////////
 
void setup(){
  Serial.begin(9600);  
  /////////////
  pinMode(Relay_1, OUTPUT); 
  pinMode(Relay_2, OUTPUT); 
  /////////////
  pinMode(Remote_a, INPUT); 
  pinMode(Remote_b, INPUT); 

  /////////////
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  /////////////
  Serial.println("-------------------");
  Serial.println("| B1    B2    B3  |");
  Serial.println("| B4    B5    B6  |");
  Serial.println("| B7    B8    B9  |");
  Serial.println("| B10   B11   B12 |");
  Serial.println("| N_1   N_2   N_3 |");
  Serial.println("| N_4   N_5   N_6 |");
  Serial.println("| N_7   N_8   N_9 |");
  Serial.println("-------------------");  
  irrecv.enableIRIn(); 
  /////////////

}
 
void loop(){
  if (irrecv.decode(&results))
  {
    translateIR(); 
    irrecv.resume(); 
  } 
  
  NetFunction();
  
  DKRemote();
}

//*****************************************************************
void DKRemote()
{
  int TinHieu_10 =digitalRead(Remote_a);
  digitalWrite(Relay_1, TinHieu_10);   
  
  int TinHieu_20 =digitalRead(Remote_b);
  digitalWrite(Relay_2, TinHieu_20);   
}
//*****************************************************************
void translateIR() 
{
  switch(results.value)
  {
  case 0xFFA25D:  
    Serial.println(" B1"); 
    break;
  case 0xFF629D:  
    Serial.println(" B2"); 
    break;
  case 0xFFE21D:  
    Serial.println(" B3"); 
    break;
  case 0xFF22DD:  
    Serial.println(" B4"); 
    break;
  case 0xFF02FD:  
    Serial.println(" B5"); 
    break;
  case 0xFFC23D:  
    Serial.println(" B6"); 
    break;
  case 0xFFE01F:  
    Serial.println(" B7"); 
    break;
  case 0xFFA857:  
    Serial.println(" B8"); 
    break;
  case 0xFF906F:  
    Serial.println(" B9"); 
    break;
  case 0xFF6897:  
    Serial.println(" B10"); 
    break;
  case 0xFF9867:  
    Serial.println(" B11"); 
    break;
  case 0xFFB04F:  
    Serial.println(" B12"); 
    break;
  case 0xFF30CF:  
    Serial.println(" 1              "); 
    digitalWrite(Relay_1, LOW);    
    break;
  case 0xFF18E7:  
    digitalWrite(Relay_1, HIGH);  
    Serial.println(" 2              "); 
    break;
  case 0xFF7A85:  
    Serial.println(" 3              "); 
    break;
  case 0xFF10EF: 
    digitalWrite(Relay_2, LOW);     
    Serial.println(" 4              "); 
    break;
  case 0xFF38C7:  
    digitalWrite(Relay_2, HIGH);    
    Serial.println(" 5              "); 
    break;
  case 0xFF5AA5:  
    Serial.println(" 6              "); 
    break;
  case 0xFF42BD:  
    Serial.println(" 7              "); 
    break;
  case 0xFF4AB5:  
    Serial.println(" 8              "); 
    break;
  case 0xFF52AD:  
    Serial.println(" 9              "); 
    break;
  default: 
    Serial.println(" other button   ");
  }
  delay(500);
}
//*****************************************************************
void NetFunction()
{
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
 
        //read char by char HTTP request
        if (readString.length() < 100) {
 
          //store characters to string
          readString += c;
          //Serial.print(c);
        }
 
        //if HTTP request has ended
        if (c == '\n') {
 
          ///////////////
          Serial.println(readString); //print to serial monitor for debuging
 
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();
 
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
          client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
          client.println("<link rel='stylesheet' type='text/css' href='http://homeautocss.net84.net/a.css' />");
          client.println("<TITLE>Home Automation</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<H1>Home Automation</H1>");
          client.println("<hr />");
          client.println("<br />");
         
          client.println("<a href=?on=1>Bat den</a>");
          client.println("<a href=?off=1>Tat den</a><br />");        
 
          client.println("</BODY>");
          client.println("</HTML>");
 
          delay(1);
          //stopping client
          client.stop();
 
          ///////////////////// control arduino pin
          if(readString.indexOf("?on") >0)//checks for on
          {
            digitalWrite(Relay_1, LOW);    // set pin 8 high
            Serial.println("Led On");
            Serial.println(readString.indexOf("?on"));
          }

          if(readString.indexOf("?off") >0)//checks for off
          {
            digitalWrite(Relay_1, HIGH);    // set pin 8 low
            Serial.println("Led Off");
          }
          //clearing string for next read
          readString="";
 
        }
      }
    }
  }
}
