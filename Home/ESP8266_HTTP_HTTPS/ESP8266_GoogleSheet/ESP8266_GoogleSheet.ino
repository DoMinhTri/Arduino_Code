#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClientSecure client;
///////////////////////////////////////
const char* ssid     = "MCK0";     
const char* password = "congtymck";
/////////////////////////////////////////////////////
String GoogleSheet_Data(String sURL)
{
  String GT2  = "", GT3 = "";
  ////////////////////////////
  if (!client.connect("script.google.com", 443)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()) 
  {
    String lines = client.readStringUntil('\n');
    int8_t ind   = lines.indexOf("https://");
    if(ind > 0){ sURL = lines.substring(ind); break;  }
  }
  ////////////////////////////
  if (!client.connect("script.google.com", 443)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()){  sURL = client.readStringUntil('\n');  break; }
  while (client.available())
  { 
    GT3  = GT2; 
    GT2  = sURL; 
    sURL = client.readStringUntil('\n');
  }
  Serial.print(GT3 + "-" + GT2 + "-"); Serial.println(sURL);
  sURL.trim();
  if(sURL == ""){ if( GT2.toInt() == 0 ){ sURL = GT3; }}
  ////////////////////////////
  return sURL;
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
  String sURL  = "https://script.google.com/macros/s/AKfycbyelkSD5MvibS_jjmRZ7-RD9e1OS3cKCp7g__cNW1omhim-DSE/exec?id=ABCD";
  String aData = GoogleSheet_Data(sURL);
  ///////////////
  Serial.println(aData);
  delay(500);
}
/////////////////////////////////////////
