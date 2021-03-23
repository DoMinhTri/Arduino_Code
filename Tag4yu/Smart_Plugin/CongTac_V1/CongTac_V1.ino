
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
/////////////////
ESP8266WebServer server(80);
const char *ssid     = "Tag4yu SPV1";
const char *password = "12345678";
/////////////////
void Desktop() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}
/////////////////
void setup() {
  delay(1000);
  Serial.begin(115200);
  //////////////////
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: " + myIP);
  server.on("/", Desktop);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
