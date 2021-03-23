#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
WiFiClientSecure client;
///////////////////////////////////////
const char* ssid     = "Robo-T";     
const char* password = "dmt@123abc";
//////////////////////////////////////////////////// "nfc2cloud.com"
String HTTPS_Data(char* Host, String sURL)
{
  String GT2  = "", GT3 = "";
  ////////////////////////////
  if (!client.connect(Host, 443)) {   }   
  client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: " + Host + "\r\nConnection: close\r\n\r\n");
  ////////////////////////////
  while (client.connected()){ sURL = client.readStringUntil('\n');  break; }
  while (client.available()){ GT3  = GT2; GT2 = sURL; sURL = client.readStringUntil('\n'); }
  sURL.trim();
  if(sURL != ""){ GT3 = sURL ; }
  ////////////////////////////
  return GT3;
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
  String sURL  = "https://nfc2cloud.com/iot/demo.php";
  String aData = HTTPS_Data("nfc2cloud.com", sURL);
  Serial.println(aData);
  ///////////////
  delay(500);
}
///////////////////////////////////////
