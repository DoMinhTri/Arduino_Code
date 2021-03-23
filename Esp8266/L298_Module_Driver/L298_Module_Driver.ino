
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid     = "XeDieuKhien";
const char *password = "12345678";
ESP8266WebServer server(80);
//////////////////////////////////////
void PageIndex() {
  ///////////////////////
  if (server.hasArg("chieu"))
  {    
    String Chieu = server.arg("chieu").c_str(); 
    if(Chieu =="toi") Xe_Toi(); 
    if(Chieu =="lui") Xe_Lui(); 
    if(Chieu =="ngung") Xe_Ngung(); 
  }
  ///////////////////////
  if (server.hasArg("huong"))
  {    
    String Huong = server.arg("huong").c_str(); 
    if(Huong =="trai") Xe_Trai();
    if(Huong =="phai") Xe_Phai();
  }
  ///////////////////////
  server.send(200, "text/html", "<div align=center><h1>Hello</h1></div>");
}
//////////////////////////////////////
//////////////////////////////////////
//////////////////////////////////////
#define DC_A1 5 //  motor A direction
#define DC_A2 4 //  motor A speed

#define DC_B1 2 //  motor B direction
#define DC_B2 15 //  motor B speed

#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0
int Tocdo = 150;
/////////////////////////////////////////////////////////////////////////////
void setup() {
  
  delay(1000);
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  server.on("/", PageIndex);
  server.begin();
  /////////////
  pinMode(DC_A1, OUTPUT); 
  pinMode(DC_A2, OUTPUT); 
  pinMode(DC_B1, OUTPUT); 
  pinMode(DC_B2, OUTPUT); 
}
/////////////////////////////////////////////////////////////////////////////
String ChuoiDuLieu = "";

void loop() { 
   server.handleClient();
   //DongCoA_Toi();
   ////////////////////////////////////
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
   ////////////////////////////////////
   if(ChuoiDuLieu != "")
   {
      String  Huong = ChuoiDuLieu.substring(0, 2);
  
      if(Huong == "CT") { Xe_Toi();   }
      if(Huong == "CL") { Xe_Lui();   }
      if(Huong == "XN") { Xe_Ngung(); }

      if(Huong == "QT") { Xe_Trai();  }
      if(Huong == "QP") { Xe_Phai();  }
      ChuoiDuLieu = "";
   }
   
   delay(25);
}
/////////////////////////////////////////////////////////////////////////////
void Xe_Toi()  { DongCoA_Toi();   }
void Xe_Lui()  { DongCoA_Lui();   }
void Xe_Ngung(){ DongCoA_Ngung(); }

void Xe_Trai() {  DongCoB_Toi(); delay(300);  DongCoB_Ngung(); }
void Xe_Phai() {  DongCoB_Lui(); delay(300);  DongCoB_Ngung(); }

void Xoay_Phai(){  }
void Xoay_Trai(){  }
/////////////////////////////////////////////////////////////////////////////
void DongCoA_Toi()
{
  digitalWrite(DC_A1, LOW); 
  digitalWrite(DC_A2, HIGH);
  //Tocdo = constrain(Tocdo, MIN_SPEED, MAX_SPEED);
  //analogWrite(DC_A2, 255 - Tocdo);
}

void DongCoA_Lui(){
  digitalWrite(DC_A1, HIGH); 
  digitalWrite(DC_A2, LOW);
  //Tocdo = constrain(Tocdo, MIN_SPEED, MAX_SPEED);
  //analogWrite(DC_A2, Tocdo);
}

void DongCoA_Ngung(){
  digitalWrite(DC_A1, LOW); 
  digitalWrite(DC_A2, LOW); 
}
/////////////////////////
void DongCoB_Toi()
{
  digitalWrite(DC_B1, LOW); 
  digitalWrite(DC_B2, HIGH); 
}

void DongCoB_Lui(){
  digitalWrite(DC_B1, HIGH); 
  digitalWrite(DC_B2, LOW); 
}

void DongCoB_Ngung()
{
  digitalWrite(DC_B1, LOW); 
  digitalWrite(DC_B2, LOW); 
}
