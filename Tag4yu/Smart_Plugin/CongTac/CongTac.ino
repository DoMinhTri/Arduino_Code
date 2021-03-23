
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <IPAddress.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);

#define GPIO2  2
/////////////////////  
void Restart(){ ESP.restart(); }
void CallRestart(){ if (server.hasArg("restart")){ String  sStas = server.arg("restart").c_str();  if( sStas == "ok"){ Serial.println(""); Serial.print("Restart"); Restart(); }}}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void Desktop() 
{
  CallRestart();
  if (server.hasArg("TrangThai"))
  { 
    String stas = server.arg("TrangThai") ;
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
  server.send(200, "text/html", "");
}
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

const char *wi_ssid  = "Tag4yu #iot_nfc";    // Wifi 
const char *wi_pass  = "0918180212";

const char *ap_ssid  = "Tag4yu SPV1"; // Access point
const char *ap_pass  = "0918180212";
boolean  DaKetNoi    = false;

void setup() 
{

  /////////////////////////////
  Serial.begin(115200);
  Serial.println(" ");
  
  pinMode(GPIO2,  OUTPUT);
  digitalWrite(GPIO2, HIGH);
  WiFi.mode(WIFI_AP_STA);
  /////////////////////////// Wifi connect
  Serial.println("Configuring wifi ...");
  WiFi.begin(wi_ssid, wi_pass);
  IPAddress wip     = IPAddress(192,168,43,123);
  IPAddress wgetway = IPAddress(192,168,43,1);
  IPAddress Subnet  = IPAddress(255,255,255,0);
  WiFi.config( wip, wgetway, Subnet);
  delay(3000);
  ////////////////////////// Access point setup
  Serial.println("Configuring access point...");
  WiFi.softAP(ap_ssid, ap_pass);  
  delay(1000);
  ////////////////////////// Các trang mặc định 
  server.on("/", Desktop);
  server.begin();
  Serial.println("HTTP server started");
}
//////////////////////////////////////////////////////////////////////////////////

void loop() 
{
   server.handleClient();
   if(DaKetNoi == false)
   {
      if(WiFi.status() == WL_CONNECTED) 
      {
          String IP_On  = LayChuoiIP(WiFi.localIP());
          String IP_Off = LayChuoiIP(WiFi.softAPIP());
          Serial.println("WF_IP: " + IP_On);
          Serial.println("AP_IP: " + IP_Off);
          DaKetNoi = true;
      }
   }
   delay(1000);
}

////////////////////////////////////////////////////////////////////////////////////
String LayChuoiJson()
{
        HTTPClient http;
        String payload = "";
        String SVLink   = "http://fablabs.store/cttm/data/12345678abcdefgh.txt" ;//+ SVID ;
        http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){  payload = http.getString();  }
        } 
        http.end();
        //////////
        return payload;
}
