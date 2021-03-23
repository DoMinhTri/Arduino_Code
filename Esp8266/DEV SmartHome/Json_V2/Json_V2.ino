#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid     = "Caphe sang";  
const char* password = "khongbiet";

const char* host  = "uptosmart.vn"; // Your domain  
String path       = "/homegreen/data/?utsid=6c30046100320032&utsvalue=abc";  
const int pin     = 2;

void setup() {  
  Serial.begin(115200);
  Serial.println("");  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");  
  Serial.println("IP address: " + WiFi.localIP());
}

void loop() {  

  WiFiClient client;
  if (!client.connect(host, 80)){  return;  }
  
  client.print(String("GET ") + path + "\r\n" );
  
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }
  Serial.println("---------------------------------------------");
  delay(500); 
}
