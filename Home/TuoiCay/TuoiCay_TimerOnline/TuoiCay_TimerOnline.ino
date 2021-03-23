#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 3600);

const char *ssid     = "Robo-T";
const char *password = "dmt@123abc";
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <IPAddress.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);

#define GPIO2  2
String stas = "0";
/////////////////////  
void Restart(){ ESP.restart(); }
void CallRestart(){ if (server.hasArg("restart")){ String  sStas = server.arg("restart").c_str();  if( sStas == "ok"){ Serial.println(""); Serial.print("Restart"); Restart(); }}}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Desktop() 
{
  CallRestart();
  if (server.hasArg("TrangThai"))
  { 
    stas = server.arg("TrangThai") ;
    if (stas == "1")
    { 
      digitalWrite(GPIO2, LOW);
      Serial.println("Led on");
    } 
    else
    { 
      digitalWrite(GPIO2, HIGH);  
      Serial.println("Led off");
    }
  }
  server.send(200, "text/html", stas);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String LayChuoiIP(IPAddress _IP)
{
    String _ChuoiIP = (String)_IP[0] + "." + (String)_IP[1] + "." + (String)_IP[2] + "." + (String)_IP[3] ;
    return _ChuoiIP;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(115200);
  ///////////////////////////
  pinMode(GPIO2,  OUTPUT);
  digitalWrite(GPIO2, HIGH);
  ///////////////////////////
  timeClient.begin();
  ///////////////////////////
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  IPAddress wip     = IPAddress(192,168,1,100);
  IPAddress wgetway = IPAddress(192,168,1,1);
  IPAddress Subnet  = IPAddress(255,255,255,0);
  WiFi.config( wip, wgetway, Subnet);
  delay(3000);
  ////////////////////////// Access point setup
  //Serial.println("Configuring access point...");
  //WiFi.softAP(ap_ssid, ap_pass);  
  //delay(1000);
  ////////////////////////// Các trang mặc định 
  server.on("/", Desktop);
  server.begin();
  //////////////////////////
  
}

void loop() {
  
  server.handleClient();
  
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  delay(1000);
}
