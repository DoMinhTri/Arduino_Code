#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <IPAddress.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
void Restart(){ ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
#include <Servo.h>
Servo myServo;
//////////////////////////////////////////////////////////////////////////////////
#define IN1 5  //  motor A direction
#define IN2 4  //  motor A speed

#define IN3 2  //  motor B direction
#define IN4 15 //  motor B direction
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void PageIndex() {
  ///////////////////////
  if (server.hasArg("rbtr"))
  {    
    String getVal = server.arg("rbtr").c_str(); 
    if(getVal =="toi")  { Xe_Toi();        }
    if(getVal =="lui")  { Xe_Lui();        }
    if(getVal =="ngung"){ Xe_Ngung();      }
    if(getVal =="trai") { Xe_Trai();       }
    if(getVal =="phai") { Xe_Phai();       }
    if(getVal =="dong") { ThungRac_Dong(); }
    if(getVal =="mo")   { ThungRac_Mo();   }
}
  ///////////////////////
  server.send(200, "text/html", "<div align=center>RBTC20</div>" );
}
//////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  delay(100);
  String myMac = WiFi.softAPmacAddress();
  myMac.replace(":", "");
  /////////////
  WiFi.mode(WIFI_AP);
  WiFi.softAP("RBTRC20_" + myMac, "dmt@zzz@123");
  /////////////
  server.on("/",      PageIndex);
  server.begin();
  /////////////
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  Xe_Ngung();
  /////////////
  myServo.attach(13);
  ThungRac_Mo();
  delay(3000);
  ThungRac_Dong();
}
/////////////////////////////////////////////////////////////////////////////
void loop() { 
   server.handleClient();
   ////////////////////////////////////
   delay(25);
}

/////////////////////////////////////////////////////////////////////////////
void ThungRac_Mo()  {  myServo.write(180); }
void ThungRac_Dong(){  myServo.write(0); }

void Xe_Toi()  { DongCoA_Toi();   DongCoB_Toi();   }
void Xe_Ngung(){ DongCoA_Ngung(); DongCoB_Ngung(); }
void Xe_Lui()  { Xe_Toi(); DongCoA_Lui();  DongCoB_Lui(); }
void Xe_Trai() { Xe_Toi(); DongCoA_Lui();  DongCoB_Toi(); }
void Xe_Phai() { Xe_Toi(); DongCoA_Toi();  DongCoB_Lui(); }
/////////////////////////////////////////////////////////////////////////////
void DongCoA_Lui()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}
/////////////////////////
void DongCoA_Toi()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}
/////////////////////////
void DongCoA_Ngung(){ digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW); }
/////////////////////////////////////////////////////////////////////////////
void DongCoB_Lui()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
/////////////////////////
void DongCoB_Toi()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
/////////////////////////
void DongCoB_Ngung(){  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW); }
/////////////////////////////////////////////////////////////////////////////
