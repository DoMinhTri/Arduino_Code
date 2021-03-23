

#include <WiFiClientSecure.h>
WiFiClientSecure client;

const char* ssid     = "Robo-MT";
const char* password = "dmt@123abc";
///////////////////////////////////////////////////////
void setup()
{
    Serial.begin(9600);
    Serial.println("");
    ////////////////////
    delay(10);
    WiFi.begin(ssid, password);
}
///////////////////////////////////////////////////////
void loop()
{
  char* host  = "nfc2cloud.com";
  String sURL = "/sw/test.txt";
  sURL = HTTPS_Data(host, sURL);
  //Serial.println(sURL);
  delay(500);
}
///////////////////////////////////////////////////////
String GetData(String sURL)
{
  WiFiClient client;
  ////////////////////////////
  if (!client.connect("nfc2cloud.com", 443)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: nfc2cloud.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()){  sURL = client.readStringUntil('\n');  break; }
  while (client.available()){  sURL = client.readStringUntil('\n');  }
  ////////////////////////////
  sURL.trim();
  return sURL;
}
///////////////////////////////////////////////////////
String HTTPS_Data(char* Host, String sURL)
{
   WiFiClient client;
   String GT2  = "", GT3 = "";
   ////////////////////////////
   if (!client.connect(Host, 443)) {   }   
   client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: " + Host + "\r\nConnection: close\r\n\r\n");
   ////////////////////////////
   while (client.connected()){ sURL = client.readStringUntil('\n');  break; }
   while (client.available()){ GT3  = GT2; GT2  = sURL; sURL = client.readStringUntil('\n'); Serial.println(sURL); }
   ////////////////////////////
   client.stop();  sURL.trim();
   if(sURL == ""){ if( GT2.toInt() == 0 ){  GT3.trim(); sURL = GT3; }}
   ////////////////////////////
   return sURL;
}
