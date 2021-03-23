// CS     10
// MOSI	  11
// MISO	  12
// SCK	  13

#include <SPI.h>
#include <Ethernet.h>
 
 
byte mac[]     = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // physical mac address
byte ip[]      = { 192, 168, 1, 15 };                   // ip in lan
byte gateway[] = { 192, 168, 1, 1 };                     // internet access via router
byte subnet[]  = { 255, 255, 255, 0 };                   // subnet mask
EthernetServer server(80);                               // server port
 
String readString;
 int nData = 2;
//////////////////////
 
void setup(){
 
  pinMode(nData, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //the pin for the servo co
  //enable serial data print
  Serial.begin(9600);
  Serial.println("server LED test 1.0"); // so I can keep track of what is loaded
}
 
void loop(){
  // Create a client connection
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
            digitalWrite(nData, LOW);    // set pin 8 high
            Serial.println("Led On");
            Serial.println(readString.indexOf("?on"));
          }

          if(readString.indexOf("?off") >0)//checks for off
          {
            digitalWrite(nData, HIGH);    // set pin 8 low
            Serial.println("Led Off");
          }
          //clearing string for next read
          readString="";
 
        }
      }
    }
  }
}
