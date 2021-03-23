
#include <SoftwareSerial.h>
SoftwareSerial Port2(5,6); // TX - RX

////////////////////////////////////////////////////////////////////////////////////  
void setup() { 
      
  Serial.begin(115200);    // Serial monitor
  Port2.begin(9600);
  
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
 
   {
       if(ChuoiDuLieu != "")
       {
           Serial.print(ChuoiDuLieu);
           Port2.print(ChuoiDuLieu);
          ChuoiDuLieu = "";
      }
   }
}
///////////////////////////////////////////////////////////////////////////////////////
boolean connectWiFi(String NetworkSSID,String NetworkPASS) {
  String cmd = "AT+CWJAP=\"";
  cmd += NetworkSSID;
  cmd += "\",\"";
  cmd += NetworkPASS;
  cmd += "\"";
  
  Serial.println(cmd); 
  Port2.println(cmd);
}
///////////////////////////////////////////////////////////////////////////////////////
void WebPage()
{
  Serial.println("HTTP/1.1 200 OK");
  Serial.println("Content-Type: text/html");
  Serial.println(""); //  do not forget this one
  Serial.println("<!DOCTYPE HTML>");
  Serial.println("<html>");

  Serial.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");
  Serial.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
  Serial.println("</html>");
}

