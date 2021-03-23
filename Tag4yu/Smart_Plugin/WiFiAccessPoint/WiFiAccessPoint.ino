

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>



String myMac         = WiFi.softAPmacAddress();
const char *ssid     = "TAG4YU_SPW1_";
const char *password = "0918180212";

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  myMac.replace(":", "");
  WiFi.softAP(ssid + myMac, password);
  server.on("/", handleRoot);
  server.begin();
  ////////////
  Serial.println("");
  Serial.println(myMac);
}

void loop() {
  server.handleClient();
}
