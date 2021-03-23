
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
///////////////////////////////////////////////////////////////////////////////////////
char message[7] = "abcdef";
File myFile;
///////////////////////////////////////////////////////////////////////////////////////
#define REQ_BUF_SZ   20
///////////////////////////////////////////////////////////////////////////////////////
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 11, 29);  // IP address, may need to change depending on network
EthernetServer server(80);       // create a server at port 80
File webFile;
char req_index            = 0;   // index into HTTP_req buffer
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
///////////////////////////////////////////////////////////////////////////////////////

void setup()
{
    // disable Ethernet chip
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    ////////////////////////////
    Serial.begin(9600);       // for debugging
    Serial.print(message[0]); 
    Serial.println(message[5]); 
    ////////////////////////////    
    // initialize SD card
    if (SD.begin(4)) 
    {
      Serial.println("SUCCESS - SD card install."); 
      PassMacDinh();
    }
    else
    {
        Serial.println("ERROR - SD card install."); 
        return;    // init failed
    }
    ////////////////////////////    
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
}
///////////////////////////////////////////////////////////////////////////////////////
void loop()
{
    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // buffer first part of HTTP request in HTTP_req array (string)
                // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                // print HTTP request character to serial monitor
                Serial.print(c);
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // open requested web page file
                    if (StrContains(HTTP_req, "GET / ")  || StrContains(HTTP_req, "GET /index.htm")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("index.htm");        // open web page file
                    }
                    
                     if (StrContains(HTTP_req, "GET / ")  || StrContains(HTTP_req, "GET /home.htm")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("home.htm");        // open web page file
                    }
                    //////-----------------------------------------------
                    else if (StrContains(HTTP_req, "GET /bg.jpg")) {
                        webFile = SD.open("bg.jpg");
                        if (webFile) {
                            client.println("HTTP/1.1 200 OK");
                            client.println();
                        }
                    }
                    
                    else if (StrContains(HTTP_req, "GET /bg_home.jpg")) {
                        webFile = SD.open("bg_home.jpg");
                        if (webFile) {
                            client.println("HTTP/1.1 200 OK");
                            client.println();
                        }
                    }
                    
                    else if (StrContains(HTTP_req, "GET /loader.gif")) {
                        webFile = SD.open("loader.gif");
                        if (webFile) {
                            client.println("HTTP/1.1 200 OK");
                            client.println();
                        }
                    }
                    
                    else if (StrContains(HTTP_req, "GET /logo.png")) {
                        webFile = SD.open("logo.png");
                        if (webFile) {
                            client.println("HTTP/1.1 200 OK");
                            client.println();
                        }
                    }
                    
                    else if (StrContains(HTTP_req, "GET /phone.png")) {
                        webFile = SD.open("phone.png");
                        if (webFile) {
                            client.println("HTTP/1.1 200 OK");
                            client.println();
                        }
                    }
                    //////-----------------------------------------------
                    else if (StrContains(HTTP_req, "GET /style.css")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/css");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("style.css");        // open web page file
                    }                    

                    else if (StrContains(HTTP_req, "GET /style_home.css")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/css");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("style_home.css");        // open web page file
                    }
                    //////-----------------------------------------------
                    else if (StrContains(HTTP_req, "GET /jquery.1.6.js")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/javascript");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("jquery.1.6.js");        // open web page file
                    }                    

                    else if (StrContains(HTTP_req, "GET /function.js")) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/javascript");
                        client.println("Connnection: close");
                        client.println();
                        webFile = SD.open("function.js");        // open web page file
                    }
                    //////-----------------------------------------------
                    
                    if (webFile) {
                        while(webFile.available()) {
                            client.write(webFile.read()); // send web page to client
                        }
                        webFile.close();
                    }
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}
///////////////////////////////////////////////////////////////////////////////////////
void PassMacDinh()
{
  //SD.remove("ADMNPASS.TXT"); 
  if (SD.exists("ADMNPASS.TXT")) 
   {
      Serial.println("ADMNPASS.TXT - exists");
      myFile = SD.open("ADMNPASS.TXT");
      if (myFile) 
      {
        while (myFile.available()) 
        {
          Serial.write(myFile.read());
        }
      }
    }
    else
    {
      SD.remove("ADMNPASS.TXT");
      myFile = SD.open("ADMNPASS.TXT", FILE_WRITE);
      myFile.println("admin");
      myFile.print("password");
      myFile.close();
      Serial.println("SUCCESS - Admin password install");
    }
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////
