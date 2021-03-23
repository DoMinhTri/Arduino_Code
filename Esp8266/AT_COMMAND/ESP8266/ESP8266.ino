#include <SoftwareSerial.h>
SoftwareSerial esp8266(6, 7); // RX | TX

#define DEBUG true

int ERROR_PIN = 7;
int OK_PIN = 6;

char serialbuffer[400];//serial buffer for request url
const String ssid = "MEXUAN_network";
const String pw = "lichvannien";

int state = 0;

void setup() 
{
    delay(1000);
    /**
    // init leds
    pinMode(ERROR_PIN, OUTPUT);
    pinMode(OK_PIN, OUTPUT);

    state = 0;

    digitalWrite(ERROR_PIN, HIGH);
    digitalWrite(OK_PIN, LOW);
    /**/
    // init ports
    Serial.begin(9600);
    Serial.println("initializing esp8266 port...");
    esp8266.begin(115200);
    delay(400);
    // init WIFI
    /**/
    while(!esp8266.available())
    {
        Serial.print("...");
        delay(300);
    }
    Serial.println();
    Serial.println("FINISH esp8266 initializing!");
    //
    /**
    digitalWrite(ERROR_PIN, LOW);
    digitalWrite(OK_PIN, HIGH);
    state = 1;
    /**/
    /**/
    // Setup connection
    sendData("AT+RST\r\n",2000,DEBUG);
    sendData("AT+CWMODE?\r\n",1000,DEBUG);
    //sendData("AT+CWMODE=1\r\n",2000,DEBUG);
    //sendData("AT+RST\r\n",3000,DEBUG);
    //sendData("AT+CWLAP\r\n",6000,DEBUG);
    sendData("AT+CWJAP=\"" + ssid + "\",\""+ pw +"\"\r\n",12000,DEBUG);
    sendData("AT+CIFSR\r\n",8000,DEBUG);
    sendData("AT+CIPMUX=1\r\n", 6000, DEBUG);
    webRequest("");
    /**/
    /**/
}

void loop() 
{
    if (esp8266.available())
    {
        char c = esp8266.read() ;
        Serial.print(c);
        /**
        if(state == 0)
        {
            state = 1;
            digitalWrite(ERROR_PIN, LOW);
            digitalWrite(OK_PIN, HIGH);
        }
        /**/
    }
    else
    {
        /**
        if(state > 0)
        {
            state = 0;
            digitalWrite(ERROR_PIN, HIGH);
            digitalWrite(OK_PIN, LOW);
        }
        /**/
    }   
    if (Serial.available())
    {  
        char c = Serial.read();
        esp8266.print(c);
    }
}

//////////////////////////////////////////////////////////////////////////////
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    esp8266.print(command); // send the read character to the esp8266
    long int time = millis();
    while( (time+timeout) > millis())
    {
        while(esp8266.available())
        {
            // The esp has data so display its output to the serial window
            char c = esp8266.read(); // read the next character.
            response+=c;
        }
    }
    if(debug)
    {
        Serial.print(response);
    }
    return response;
}
//////////////////////////////////////////////////////////////////////////////////
String webRequest(String url)
{
    String response = "";
    url = "www.google.es";
    //String tmpCommand = "AT+CIPSTART=4," + "\"TCP\",\"" + url + "\",80";
    String tmpSTARTCommmand = "AT+CIPSTART=0,\"TCP\",\"retro.hackaday.com\",80\r\n\r\n";
    String tmpGETCommand = "GET / HTTP/1.1\r\nHost: "; 
    tmpGETCommand += "retro.hackaday.com";
    tmpGETCommand += ":80\r\n\r\n";
    String tmpSENDCommand = "AT+CIPSEND=0," + String(tmpGETCommand.length()) + "\r\n";
    sendData(tmpSTARTCommmand, 8000, DEBUG);
    sendData(tmpSENDCommand, 8000, DEBUG);
    sendData(tmpGETCommand, 15000, DEBUG);
}
