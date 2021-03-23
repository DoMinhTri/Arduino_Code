
#include <IPAddress.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
const char* ssid     = "NVH";
const char* password = "khongbiet";
////////////////////////////////////////////
boolean AutoOn    = true;

uint8_t onTimer   = 0;
uint8_t offTimer  = 0;
uint8_t GocQuay   = 0;
uint8_t FistDelay = 0;

#include <Servo.h> 
Servo SERVO; 
void PowerOn() { SERVO.write(GocQuay);  delay(onTimer*1000);  SERVO.write(0); }
void PowerOff(){ SERVO.write(GocQuay);  delay(offTimer*1000); SERVO.write(0); }
void ServoDefault(){ SERVO.write(0); }
////////////////////////////////////////////
void Desktop() 
{
  String sData = "";
  /////////////////
  if (server.hasArg("fi"))
  {    
      sData = server.arg("fi").c_str(); 
      EEPROM_WN(4, sData.toInt());
      sData = "";
  }
  /////////////////
  if (server.hasArg("on"))
  {    
      sData = server.arg("on").c_str(); 
      EEPROM_WN(5, sData.toInt());
      sData = "";
  }
  /////////////////
  if (server.hasArg("of"))
  {    
      sData = server.arg("of").c_str(); 
      EEPROM_WN(6, sData.toInt());
      sData = "";
  }
  /////////////////
  if (server.hasArg("de"))
  {    
      sData = server.arg("de").c_str(); 
      EEPROM_WN(7, sData.toInt());
      sData = "";
  }
  /////////////////
  if (server.hasArg("rs"))
  {    
      sData = server.arg("rs").c_str(); 
      if( sData == "ok"){ Restart(); }
  }
  /////////////////
  if (server.hasArg("ao"))
  {    
      sData = server.arg("ao").c_str(); 
      if( sData == "1"){ AutoOn = true; } else{  AutoOn = false; } 
  }
  /////////////////
  /////////////////
  if (server.hasArg("pw")) {
    sData = server.arg("pw").c_str(); 
    if(sData == "0") { PowerOff(); }  
    if(sData == "1") { PowerOn();  } 
    if(sData == "2") { ServoDefault(); }   
    sData = "";    
  }
  /////////////////
  sData  = "<div align='center'><table><tr><td align=center><h1><font color='red'>Mini PC Power </font></h1></td></tr><tr><td align=left> Fist delay (fi) : " + String(FistDelay) + " s<br> On timer (on) : " + String(onTimer) + " s<br> Off timer (of) : " + String(offTimer) + " s<br> Degree (de) : " + String(GocQuay) + "&#186;" + "<br>Reset (rs) : ok <br> Auto On (ao): 1 = true, 0 = false <br> Power (pw) : 0 = off , 1 = on , 2 = default</td></tr></table></div>";
  server.send(200, "text/html", sData);
  sData = "";   
}
////////////////////////////////////////////
#include <EEPROM.h>
void LuuEEPROM(uint8_t nDiaChi, uint8_t nGiaTri){  EEPROM.write(nDiaChi, nGiaTri);  EEPROM.commit();  delay(20);}
void EEPROM_WN(uint8_t nDiaChi, uint8_t nSoLuu){ LuuEEPROM(nDiaChi, nSoLuu); }
uint8_t EEPROM_RN(uint8_t nDiaChi){ return EEPROM.read(nDiaChi); }
////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  Serial.println();
  EEPROM.begin(32);  delay(100);
  FistDelay = EEPROM_RN(4);
  onTimer   = EEPROM_RN(5);
  offTimer  = EEPROM_RN(6);
  GocQuay   = EEPROM_RN(7);
  //////////////////////////
  IPAddress wip     = IPAddress(192,168,1,99);
  IPAddress wgetway = IPAddress(192,168,1,1);
  IPAddress Subnet  = IPAddress(255,255,255,0);
  WiFi.config( wip, wgetway, Subnet);
  //////////////////////////
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(FistDelay*1000);
  Serial.println(WiFi.localIP());
  ////////////////////////// 
  server.on("/", Desktop);
  server.begin();
  ////////////////////////// 
  SERVO.attach(2);
  PowerOn();
}
////////////////////////////////////////////
void loop() {
  server.handleClient();
  //if(AutoOn){ delay(120000); PowerOn(); }
}
