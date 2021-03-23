

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

void handleRoot() {
	server.send(200, "text/html", "<h1>You are connected</h1>");
}
//////////////////////////////////////////////////////////
boolean KetNoiWifi = false;

void setup() {
	delay(1000);
	Serial.begin(9600);
  ////////////////////////////////////////
  const char *ssid     = "Duy Khanh 7";
  const char *password = "passdk07";
  WiFi.begin(ssid, ssid);
  delay(5000);
  Serial.println(""); 
  ////////////////////////////////////////
  const char *assid     = "DMT Company";
  const char *apassword = "12345678";
  WiFi.softAP(assid, apassword);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  Serial.println(".....................");
  if(WiFi.status() == WL_CONNECTED)
  {  
        KetNoiWifi = true;   
        Serial.print("Connected IP: "); 
        Serial.println(WiFi.localIP()); 
  } 
  else 
  {  
    Serial.println("Not connected"); 
  }
}
