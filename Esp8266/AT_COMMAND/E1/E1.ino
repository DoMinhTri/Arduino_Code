/*
 * ref: http://raysfiles.com/arduino/ESP8266a_arduino.ino
 * TEST on PC:
 * 1. start browser
 * 2. http://IpServer:8080 --> home page
 * 3. press 'ON' || 'OFF' link
 */

// constants
const byte PIN_LED = 7;
const short DELAY = 1;
void controlLED(byte pin, int cmd) {
  if (cmd == 0) {
    digitalWrite(pin, LOW); delay(DELAY); 
  } else if (cmd == 1) {
    digitalWrite(pin, HIGH); delay(DELAY); 
  }
}

/* ====== ESP8266 Demo ======
 * Print out analog values
 * (Updated Dec 14, 2014)
 * ==========================
 *
 * Change SSID and PASS to match your WiFi settings.
 * The IP address is displayed to soft serial upon successful connection.
 *
 * Ray Wang @ Rayshobby LLC
 * http://rayshobby.net/?p=9734
 */

// comment this part out if not using LCD debug

#define BUFFER_SIZE 512

#define SSID  "@PANDA"      // change this to match your WiFi SSID
#define PASS  "0904240111"  // change this to match your WiFi password
#define PORT  "8080"        // using port 8080 by default

// If using Software Serial for debug, use the definitions below
#include <SoftwareSerial.h>
SoftwareSerial dbg(2,3);  // use pins 2,3 for software serial 
#define esp Serial

char buffer[BUFFER_SIZE];

// By default we are looking for OK\r\n
char OKrn[] = "OK\r\n";
byte wait_for_esp_response(int timeout, char* term=OKrn) {
  unsigned long t=millis();
  bool found=false;
  int i=0;
  int len=strlen(term);
  // wait for at most timeout milliseconds
  // or if OK\r\n is found
  while(millis()<t+timeout) {
    if(esp.available()) {
      buffer[i++]=esp.read();
      if(i>=len) {
        if(strncmp(buffer+i-len, term, len)==0) {
          found=true;
          break;
        }
      }
    }
  }
  buffer[i]=0;
  dbg.print(buffer);
  return found;
}

// Arduino: setup to run only once
void setup() {
  pinMode(PIN_LED, OUTPUT);

  // assume esp8266 operates at 9600 baud rate, change if necessary to match your modules' baud rate
  esp.begin(9600);
  dbg.begin(9600);
    
  setupWiFi();
  // print device IP address
  dbg.print("device ip addr:");
  esp.println("AT+CIFSR");
  wait_for_esp_response(1000);
}

// Arduino: loop to run repeatedly
void loop() {
  boolean ON = false;
  int ch_id, packet_len;
  char *pb;  
  if(read_till_eol()) {
    if(strncmp(buffer, "+IPD,", 5)==0) {
      // request: +IPD,ch,len:data
      sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
      if (packet_len > 0) {
        // read serial until packet_len character received, start from :
        pb = buffer+5;
        while(*pb!=':') pb++;
        pb++;
        if (strncmp(pb, "GET /led=1", 10) == 0) {
          wait_for_esp_response(1000);
          controlLED(PIN_LED, 1); 
          serve_homepage(ch_id);
        } else if (strncmp(pb, "GET /led=0", 10) == 0) {
          wait_for_esp_response(1000);
          controlLED(PIN_LED, 0);
          serve_homepage(ch_id);
        } else if (strncmp(pb, "GET /", 5) == 0) {
          wait_for_esp_response(1000);
          serve_homepage(ch_id);
        }
      }
    }
  }
}

bool read_till_eol() {
  static int i=0;
  if(esp.available()) {
    buffer[i++]=esp.read();
    if(i==BUFFER_SIZE)  i=0;
    if(i>1 && buffer[i-2]==13 && buffer[i-1]==10) {
      buffer[i]=0;
      i=0;
      dbg.print(buffer);
      return true;
    }
  }
  return false;
}

void serve_homepage(int ch_id) {
  String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n";
  String content="<h1 align='center'><a href='./led=1'>ON</a> &nbsp;&nbsp; <a href='./led=0'>OFF</a></h1>\n";
  content += "<div align='center'><form action='led=1' method='GET'><input type='submit' value='ON'/></form></div>";
  content += "<div align='center'><form action='led=0' method='GET'><input type='submit' value='OFF'/></form></div>";

  header += "Content-Length:";
  header += (int)(content.length());
  header += "\r\n\r\n";
  
  esp.print("AT+CIPSEND=");
  esp.print(ch_id);
  esp.print(",");
  esp.println(header.length()+content.length());
  if(wait_for_esp_response(2000, "> ")) {
    esp.print(header);
    esp.print(content);
  } else {
    esp.print("AT+CIPCLOSE=");
    esp.println(ch_id);
  }
}

void setupWiFi() {
  // try empty AT command
  esp.println("AT");
  wait_for_esp_response(1000);

  // set mode 1 (client)
  esp.println("AT+CWMODE=1");
  wait_for_esp_response(1000);  

  // reset WiFi module
  esp.print("AT+RST\r\n");
  wait_for_esp_response(1500);
 
  // join AP
  esp.print("AT+CWJAP=\"");
  esp.print(SSID);
  esp.print("\",\"");
  esp.print(PASS);
  esp.println("\"");
  // this may take a while, so wait for 5 seconds
  wait_for_esp_response(5000);
  
  esp.println("AT+CIPSTO=30");  
  wait_for_esp_response(1000);

  // start server
  esp.println("AT+CIPMUX=1");
  wait_for_esp_response(1000);
  
  // turn on TCP service
  esp.print("AT+CIPSERVER=1,");
  esp.println(PORT);
  wait_for_esp_response(1000);
}

