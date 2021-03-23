
#include <EEPROM.h>
#define DiaChiDen_1 1
#define DiaChiDen_2 2
int value_1 = LOW;
int value_2 = LOW;
boolean KichHoatLed = true;
//////////////////////////////////////////////////////////////////////////////////
void LuuEEPROM(int nDiaChi, int nGiaTri)
{
   EEPROM.write(nDiaChi, nGiaTri);
   EEPROM.commit();
   delay(10);
}
//////////////////////////////////////////////////////////////////////////////////
int LoadEEPROM(int nDiaChi)
{
   delay(10);
   return EEPROM.read(nDiaChi);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
//////////////////////////////////////////////////////////////////////////////////
const char* ssid     = "Chua Long An";
const char* password = "muongihoithaykia";
WiFiServer Online_Server(80);
int ledPin_1 = 2; // GPIO2
int ledPin_2 = 0; // GPIO0
boolean KetNoiThanhCongWifi = false;
//////////////////////////////////////////////////////////////////////////////////
const char *Off_ssid     = "DMT Co.,Ltd";
const char *Off_password = "12345678";
ESP8266WebServer Offline_server(80);
void handleRoot() {
  String KK = (String)LoadEEPROM(1);
  Offline_server.send(200, "text/html", "<h1>Trang admin</h1>" + KK );
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String TaoNutNhan(String ID, int TrangThai)
{
  String TTLink  = "on";
  String TTDen   = "on";
  String TTColor = "#00AA00";
  if(TrangThai == HIGH) 
  { 
    TTLink  = "off";
    TTDen   = "off";
    TTColor = "#FF0000";
  }
   String TTTatMo ="<font size=5 color="+ TTColor +">"+ TTDen +"</font> </br>";
   String html    ="<a href=led"+ ID + "=" + TTLink + "><font size=6 class='button blue' style='width:20%'>Led "+ ID +"</font></a> " + TTTatMo;
   return html;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void setup() {
  EEPROM.begin(512);
  
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin_1, OUTPUT);
  digitalWrite(ledPin_1, LOW);
  
  pinMode(ledPin_2, OUTPUT);
  digitalWrite(ledPin_2, LOW);
  
  // Connect to WiFi network
  Serial.print("Connecting to :");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  delay(5000);
  Serial.println("");
  if(WiFi.status() == WL_CONNECTED) 
  {
    KetNoiThanhCongWifi = true;
    Serial.println("WiFi connected");
    Online_Server.begin();
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
  }
  else
  {
    Serial.println("WiFi not connected");
  }
  ///////////////////////////////////////////////
  delay(5000);
  Serial.println("..........");
  Serial.print("Configuring access point...");
  WiFi.softAP(Off_ssid, Off_password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Offline_server.on("/", handleRoot);
  Offline_server.begin();
  Serial.println("HTTP server started");
 }
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*
void HTMLHeader(WiFiClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<link type='text/css' rel='stylesheet' href='http://khicongnghiepvenmer.com/css/style.css' />");
  client.println("<html>");
}
*/
void BatTatLed(int nPin, boolean nTrangThai)
{
   digitalWrite(nPin, nTrangThai);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////// 
void loop() {


  Offline_server.handleClient();
  //if(KetNoiThanhCongWifi)
  {
  /////////
  WiFiClient client = Online_Server.available();
  if (!client) { return;  }
 
  // Wait until the client sends some data
  //Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request

  if (request.indexOf("/led1=on")  != -1)  {  LuuEEPROM(DiaChiDen_1, HIGH); KichHoatLed = true;}
  if (request.indexOf("/led1=off") != -1)  {  LuuEEPROM(DiaChiDen_1, LOW);  KichHoatLed = true;}

  if (request.indexOf("/led2=on")  != -1)  {  LuuEEPROM(DiaChiDen_2, HIGH); KichHoatLed = true;}
  if (request.indexOf("/led2=off") != -1)  {  LuuEEPROM(DiaChiDen_2, LOW);  KichHoatLed = true;}

  if(KichHoatLed)
  {
      value_1 = LoadEEPROM(DiaChiDen_1);
      value_2 = LoadEEPROM(DiaChiDen_2);
      BatTatLed(ledPin_1, value_1);
      BatTatLed(ledPin_2, value_2);
      KichHoatLed = false;
  }

  //HTMLHeader(client);
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<link type='text/css' rel='stylesheet' href='http://khicongnghiepvenmer.com/css/style.css' />");
  client.println("<html>");
  
  client.print("<div align=center><font color=blue size=16>Green Leap</font><br><br>");
  client.println(TaoNutNhan("1",value_1));
  client.println(TaoNutNhan("2",value_2));
  client.println("</div></html>");
 
  delay(1);
  //Serial.println("Client disonnected");
  }
}



