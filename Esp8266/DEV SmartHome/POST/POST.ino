#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <IPAddress.h>
#include <PubSubClient.h>

const char* ssid     = "Caphe sang";  
const char* password = "khongbiet";

const char* host     = "192.168.1.105"; // Your domain  
String path          = "/led/light.json";  
const int pin        = 2;

void setup() {  
  pinMode(pin, OUTPUT); 
  pinMode(pin, HIGH);
  Serial.begin(115200);

  delay(10);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.println("---------------------------------");
}

void loop() {  

    WiFiClient client;
    const char* host="http://jsonplaceholder.typicode.com/";
    String PostData = "title=foo&body=bar&userId=1";

    if (client.connect(host, 80)) {

    client.println("POST /posts HTTP/1.1");
    client.println("Host: jsonplaceholder.typicode.com");
    client.println("Cache-Control: no-cache");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    
    long interval = 2000;
    unsigned long currentMillis = millis(), previousMillis = millis();
    ////////////////////
    while(!client.available()){

      if( (currentMillis - previousMillis) > interval ){
  
        Serial.println("Timeout");
        //blinkLed.detach();
        digitalWrite(2, LOW);
        client.stop();     
        return;
      }
      currentMillis = millis();
    }
    ////////////////////
    while (client.connected())
    {
      if ( client.available() )
      {
        char str=client.read();
        Serial.println(str);
      }      
    }
    ////////////////////
  }
}
