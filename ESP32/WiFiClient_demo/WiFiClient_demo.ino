

#include <WiFi.h>

const char* ssid     = "Robo-MT";
const char* password = "dmt@123abc";
const char* host     = "192.168.1.135";
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
  String sURL = "/cttm/test.txt";
  sURL = GetData(sURL);
  Serial.println(sURL);
  delay(500);
}
///////////////////////////////////////////////////////
String GetData(String sURL)
{
  WiFiClient client;
  ////////////////////////////
  if (!client.connect("192.168.1.135", 80)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: 192.168.1.135\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()){  sURL = client.readStringUntil('\n');  break; }
  while (client.available()){  sURL = client.readStringUntil('\n');  }
  ////////////////////////////
  sURL.trim();
  return sURL;
}
