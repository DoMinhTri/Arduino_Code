
#include <SoftwareSerial.h>
SoftwareSerial Esp8266(6, 5); // RX, TX 


////////////////////////////////////////////////////////////////////////////////////  
void setup() { 
      
  Serial.begin(9600);    // Serial monitor
  Esp8266.begin(115200);   // ESP8266
  
  Serial.println("ESP8266 AT COMMAND");
  connectWiFi("Quang Cao Thuan Thanh", "longthanh");
  Serial.print("Waiting ... ");
  delay(2000);
  Serial.println("Begin");

}
////////////////////////////////////////////////////////////////////////////////////  

String ChuoiDuLieu = "";
void loop() 
{
  while (Serial.available() > 0 ) 
  {
       char KyTu = Serial.read();
       if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu+= KyTu; }
       delay(5);
   }
   
   if(ChuoiDuLieu != "")
   {
       String TraLoi = sendData(ChuoiDuLieu,2000, true);
       ChuoiDuLieu = "";
    }
}

///////////////////////////////////////////////////////////////////////////////////////
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
    response.trim();
    if(debug){ Serial.print(response); }
    return response;
}
///////////////////////////////////////////////////////////////////////////////////////
boolean connectWiFi(String NetworkSSID,String NetworkPASS) {
  String cmd = "AT+CWJAP=\"";
  cmd += NetworkSSID;
  cmd += "\",\"";
  cmd += NetworkPASS;
  cmd += "\"";
  
  Serial.println(cmd); 
  Serial.println(GetResponse(cmd,10));
}

void WebPage()
{
  Esp8266.println("HTTP/1.1 200 OK");
  Esp8266.println("Content-Type: text/html");
  Esp8266.println(""); //  do not forget this one
  Esp8266.println("<!DOCTYPE HTML>");
  Esp8266.println("<html>");

  Esp8266.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");
  Esp8266.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
  Esp8266.println("</html>");
}
////////////////////////////////////////////////////////////////////////////////////  
String GetResponse(String AT_Command, int wait){
  String tmpData;
  
  Esp8266.println(AT_Command);
  delay(wait);
  while (Esp8266.available() >0 )  {
    char c = Esp8266.read();
    tmpData += c;
    
    if ( tmpData.indexOf(AT_Command) > -1 )         
      tmpData = "";
    else
      tmpData.trim();  // xóa khoảng trắng 2 bên   
   }
   return tmpData;
}
