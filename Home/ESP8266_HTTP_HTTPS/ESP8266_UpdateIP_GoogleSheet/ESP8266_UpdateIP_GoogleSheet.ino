#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClientSecure client;
///////////////////////////////////////
const char* ssid     = "Robo-MT";     
const char* password = "dmt@123abc";
/////////////////////////////////////////////////////
void GoogleSheet_LocalIP_UPdate(String IP)
{
  String sURL = "https://script.google.com/macros/s/AKfycbzYJcsyFP6x5LgLZgzikit39UNde1AcBO8t_pmsszFYrp9-Xok/exec?uts_ip=" + IP;
  ////////////////////////////
  if (!client.connect("script.google.com", 443)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()){ String lines = client.readStringUntil('\n');  break;  }
}
/////////////////////////////////////////////////////
void NoIP_UPdate(String IP)
{
  String sURL = "http://dominhtri@ymail.com:dmt@1234@dynupdate.no-ip.com/nic/update?hostname=uptosmart.ddns.net/&myip=" + IP;
  ////////////////////////////
  if (!client.connect("dynupdate.no-ip.com", 80)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: dynupdate.no-ip.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()){ String lines = client.readStringUntil('\n');  break;  }
}
/////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500);  }
  client.setInsecure();
}
///////////////////////////////////////
void loop() 
{
  String NetIP = getIp();
  GoogleSheet_LocalIP_UPdate(NetIP);
  NoIP_UPdate(NetIP);
  Serial.println(NetIP);
  delay(5000);
}
/////////////////////////////////////////

String getIp()
{
  WiFiClient client;
  if (client.connect("api.ipify.org", 80)) 
  {
      client.println("GET / HTTP/1.0");
      client.println("Host: api.ipify.org");
      client.println();
  } else {
      return String();
  }
  delay(5000);
  String line;
  while(client.available())
  {
    line = client.readStringUntil('\n');
  }
  return line;
}
