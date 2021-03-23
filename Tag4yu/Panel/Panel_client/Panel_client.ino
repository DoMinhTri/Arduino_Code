#include <ESP8266WiFi.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
///////////////////////////////////////
const char* ssid     = "Tag4yu_Panel";     
const char* password = "0908221404";
///////////////////////////////////////
void Index() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}
///////////////////////////////////////
void setup() 
{
  Serial.begin(9600); 
  ///////////////////
  WiFi.mode(WIFI_STA);
  IPAddress wip     = IPAddress(192,168,10,11);
  IPAddress Subnet  = IPAddress(255,255,255,0);
  IPAddress wgetway = IPAddress(192,168,10,1);  
  WiFi.config( wip, wgetway, Subnet);
  ///////////////////
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500);  }
  ///////////////////
  server.on("/", Index);
  server.begin();
}
///////////////////////////////////////
void loop() 
{
  server.handleClient();
  delay(500);
}
/////////////////////////////////////////
