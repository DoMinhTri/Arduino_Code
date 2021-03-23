
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
const char* ssid     = "Fablab_CanTho";
const char* password = "29082016";
WiFiServer server(80);
int ledPin_1 = 2; // GPIO2
int ledPin_2 = 0; // GPIO0
boolean KetNoiThanhCongWifi = false;
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
String TaoNutNhan(String ID, int TrangThai)
{
  String TTLink  = "on";
  String TTDen   = "off";
  String TTColor = "#FF0000";
  if(TrangThai == HIGH) 
  { 
    TTLink  = "off";
    TTDen   = "on";
    TTColor = "#00AA00";
  }
   String TTTatMo ="<font size=5 color="+ TTColor +">"+ TTDen +"</font>";
   String html = "<a href=led"+ ID + "=" + TTLink + "><font size=10 class='button white' style='width:20%;height=200px'>"+ TTTatMo +"</font></a> ";
   return html;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////      Text function      ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}

int String2Int(String sSo)
{
  return sSo.toInt();
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void CaiDatWifiName(String sName){  ssid = String2Char(sName); }
//////////////////////////////////////////////////////////////////////////////////
void CaiDatFiFiPass(String sPass){  password = String2Char(sPass); }
//////////////////////////////////////////////////////////////////////////////////
void SuLyGiaTriChuoiNhan(String ChuoiNhan)
{
  String ChuoiGiaTri   = "";
  String ChuoiNhanDang = "";
  int8_t nKyTu = ChuoiNhan.length();
  for(int8_t i = 0; i< nKyTu; i++)
  {
    String sKyTu = ChuoiNhan.substring(i, i+1);
    if(sKyTu == ":")
    {
       ChuoiNhanDang = ChuoiNhan.substring(0, i);
       ChuoiGiaTri   = ChuoiNhan.substring(i+1, nKyTu);
       break;
    }
  }
  if(ChuoiNhanDang      == "MG_DATE"){ CaiDatWifiName(ChuoiGiaTri);    }
  else if(ChuoiNhanDang == "MG_WIFI"){ CaiDatFiFiPass(ChuoiGiaTri);   }
}

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
    
    Serial.println("WiFi connected");
    server.begin();
    
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  KetNoiThanhCongWifi = true;
  Serial.println(  WiFi.localIP() );
  ///////////////////////////////////////////////
  delay(5000);
  Serial.println("..........");
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
//  Offline_server.on("/admin", handleRoot);
//  Offline_server.begin();
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
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Green Leap</title>");
  client.println("<link type='text/css' rel='stylesheet' href='http://khicongnghiepvenmer.com/css/style.css' />");
  client.println("</head>");
  client.println("<body>");
  client.println("<style> body{ background: url( http://wallpoper.com/images/00/45/05/47/green-background-2_00450547.jpg ) fixed; }</style>");
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
String ChuoiTinNhan = "";

void loop() {

  while (Serial.available() > 0 ) 
  {
       char KyTu = Serial.read();
       if(KyTu != '\n' && KyTu != ' '){ ChuoiTinNhan+= KyTu; } delay(5);
   }
 
   if( ChuoiTinNhan != "") 
   { 
      //Serial.println(ChuoiTinNhan);   
      SuLyGiaTriChuoiNhan(ChuoiTinNhan); 
      ChuoiTinNhan = ""; 
    }
  
//  server.handleClient();
  if(KetNoiThanhCongWifi)
  {
  /////////
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  //Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  //Serial.println(request);
  client.flush();
 
  // Match the request

  if (request.indexOf("/led1=on")  != -1)  {  LuuEEPROM(DiaChiDen_1, HIGH); KichHoatLed = true; Serial.println("MG_RELAY:ON"); }
  if (request.indexOf("/led1=off") != -1)  {  LuuEEPROM(DiaChiDen_1, LOW);  KichHoatLed = true; Serial.println("MG_RELAY:OFF"); }

  if(KichHoatLed)
  {
      value_1 = LoadEEPROM(DiaChiDen_1);
      value_2 = LoadEEPROM(DiaChiDen_2);
      BatTatLed(ledPin_1, value_1);
      KichHoatLed = false;
  }

  //HTMLHeader(client);

//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-Type: text/html");
//  client.println(""); //  do not forget this one
//  client.println("<!DOCTYPE HTML>");
//  client.println("<html>");
//  client.println("<head>");
//  client.println("<title>Green Leap</title>");
//  client.println("<link type='text/css' rel='stylesheet' href='http://khicongnghiepvenmer.com/css/style.css' />");
//  client.println("</head>");
//  client.println("<body>");
//  client.println("<style> body{ background: url( http://wallpoper.com/images/00/45/05/47/green-background-2_00450547.jpg ) fixed; }</style>");
//  
//  client.print("<div align=center><font color=white size=16>Green Leap</font><br><br>");
//  client.println(TaoNutNhan("1",value_1));
//  client.println("</div></html>");
// 
//  delay(1);
//  Serial.println("Client disonnected");
  }
}


