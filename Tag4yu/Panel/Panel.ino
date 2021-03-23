
#include <ESP8266WiFi.h>
#include <IPAddress.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
/////////////////////////////

const char *ssid     = "Tag4yu_Panel";
const char *password = "0908221404";



void Index() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  ///////////////////
  IPAddress Subnet      = IPAddress(255,255,255,0);
  IPAddress APlocalIP   = IPAddress(192,168,10,1);
  ///////////////////
  WiFi.softAPConfig( APlocalIP, APlocalIP, Subnet);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/", Index);
  server.begin();

}

void loop() {
  server.handleClient();
}
