

#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial Esp8266(6,5); // cai dat chan thu phat du lieu cho Esp8266, 10-RX, 11-TX                           
void setup()
{
  Serial.begin(9600);
  Esp8266.begin(115200); // chu y phai cung toc do voi ESP 8266
  
  pinMode(6,OUTPUT);// cai dat chan 11 la chan ra
  digitalWrite(6,HIGH);// tich cuc muc thap
  
  pinMode(13,OUTPUT);// tuong tu chan 12
  digitalWrite(13,LOW);
  
 //========================gui tap lenh AT cho ESP 8266 de cai dat cac tham so cho WiFi================================
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CWSAP=\"Quang Cao Thuan Thanh\",\"longthanh\",1,4\r\n",1000,DEBUG);// setup your SSID and Password
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}
 //====================================================================================================================
void loop()
{
  if(Esp8266.available()) // kiem tra xem ESP 8266 co gui data ko
  {
    if(Esp8266.find("+IPD,"))
    {
     delay(1000); // wait for the serial buffer to fill up (read all the serial data)
     // get the connection id so that we can then disconnect
     int connectionId = Esp8266.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
          
     Esp8266.find("pin="); // advance cursor to "pin="
     
     int pinNumber = (Esp8266.read()-48)*10; // get first number i.e. if the pin 13 then the 1st number is 1, then multiply to get 10
     pinNumber += (Esp8266.read()-48); // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number
     Serial.println( "Pin fist stas: " + digitalRead(pinNumber));
     digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin    
     Serial.println( "Pin sendcon stas: " + digitalRead(pinNumber));
     // make close command
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,1000,DEBUG); // close connection
    }
  }
}
 
/*
* Name: sendData
* Description: Function used to send data to Esp8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    Esp8266.print(command); // send the read character to the esp8266
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(Esp8266.available())
      {
        char c = Esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    if(debug){ Serial.print(response); }
    return response;
}
   
