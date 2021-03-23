// 0xE5,0x2E,0x56,0x9D,0x01,0xB0,0x88,0xF9,0x12,0x63,0x4E,0xD8,0x32,0x51,0xA9,0xBC,0x32,0x4C,0x0C,0x0E   // fingerprint : https/nfc2cloud.com/ssl.php (SHA1)
//  Tag4yu_Guest - 62605606
//********** CHANGE PIN FUNCTION  TO GPIO **********
//GPIO 1 (TX) swap the pin to a GPIO.
//GPIO 3 (RX) swap the pin to a GPIO.
//**************************************************

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
ESP8266WiFiMulti WiFiMulti;

const uint8_t fingerprint[20] = { 0xE5,0x2E,0x56,0x9D,0x01,0xB0,0x88,0xF9,0x12,0x63,0x4E,0xD8,0x32,0x51,0xA9,0xBC,0x32,0x4C,0x0C,0x0E };
String myMac = WiFi.softAPmacAddress();
/////////////////////////////////////////////////////////////////////////
uint8_t LedStas  = 2  ;
String sUrl      = "" ;
String WifiName  = "" ;
String WifiPass  = "" ;
String myOnIP    = "" ;
uint8_t cGio = 0, cPhut = 0, cGiay = 0;
/////////////////////////////////////////////////////////////////////////
#include <Wire.h>
#define I2C_SDA D4         
#define I2C_SCL D3           
#define ADDRESS_DYN5 0xA6
uint8_t data_buff[64] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
/////////////////////////////////////////////////////////////////////////
#define GPIO2  D2
void Led_On()      {  LedStas = 1; digitalWrite(GPIO2, LOW);  }
void Led_Off()     {  LedStas = 2; digitalWrite(GPIO2, HIGH); }
void Restart()     {  Serial.println("MCU_RESTART"); delay(1000); ESP.restart(); }
/////////////////////////////////////////////////////////////////////////
int setupIIC (uint8_t slave_addr)
{
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.beginTransmission (slave_addr / 2);
    return  Wire.endTransmission();
}
/////////////////////////////////////////////////////////////////////////
int readIIC_data (uint8_t slave_addr, uint8_t data_addr, uint8_t* data_buff, uint16_t data_size)
{
    Wire.beginTransmission (slave_addr / 2);
    Wire.write ( (byte) 0x00);
    Wire.write ( (byte) data_addr);
    Wire.endTransmission();
    Wire.requestFrom ( (int) slave_addr / 2, (int) data_size);
    
    for (int i = 0; i < data_size; i++) // slave may send less than requested
    {
        data_buff[i] = Wire.read();
    }
    Wire.endTransmission (true);
}
/////////////////////////////////////////////////////////////////////////
boolean ASCIIOK(uint8_t nNum)
{
  boolean KiemTra = false;
  if(( nNum < 127) && (nNum > 31)){ KiemTra = true; }
  return KiemTra;
}
/////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
}
//////////////////////////////////////////////////////////////////////////////////
String LayChuoiIP(IPAddress _IP)
{
    String _ChuoiIP = (String)_IP[0] + "." + (String)_IP[1] + "." + (String)_IP[2] + "." + (String)_IP[3] ;
    return _ChuoiIP;
}
/////////////////////////////////////////////////////////////////////////
void GetWifiNamePass()
{
  boolean DauWN  = true;
  boolean CuoiWN = false;
  boolean DauWP  = false;
  boolean CuoiWP = false;
  uint8_t nDauWN = 0, nCuoiWN = 0, nDauWP = 0, nCuoiWP = 0;
  /////////////////////////////////////////////////////////////////
  for(uint8_t i=0; i < sizeof(data_buff); i++)
  {
    //////////////////////
    if(DauWN)
    {
      if((data_buff[i] == 69) && (data_buff[i+1] == 0))
      {
       
          DauWN  = false; 
          CuoiWN = true; 
          nDauWN = i + 3 ; 
          i      = nDauWN;  
      }
    }
    //////////////////////
    if(CuoiWN)
    {
      if(ASCIIOK(data_buff[i]) == false)
      {
          CuoiWN  = false;
          DauWP   = true;
          nCuoiWN = i - 1;
          i       = i + 1;
      }
    }
    //////////////////////
    if(DauWP)
    {
      if((data_buff[i] == 16) && (data_buff[i+1] == 39) && (data_buff[i+2] == 0))
      {
          DauWP  = false;
          CuoiWP = true;
          nDauWP = i + 4;
          i      = nDauWP;
      }
    }
    //////////////////////
    if(CuoiWP)
    {
      if(ASCIIOK(data_buff[i]) == false)
      {
          CuoiWP  = false;
          nCuoiWP = i - 1;
      }
    }
    //////////////////////
  }
  /////////////////////////////////////////////////////////////////
  // uint8_t nDauWN = 0, nCuoiWN = 0, nDauWP = 0, nCuoiWP = 0;
  WifiName = ""; WifiPass = "";
  for(uint8_t j = nDauWN; j <= nCuoiWN ; j++){  WifiName = WifiName + char(data_buff[j]);  }
  for(uint8_t k = nDauWP; k <= nCuoiWP ; k++){  WifiPass = WifiPass + char(data_buff[k]);  }
  //////////////////
}
/////////////////////////////////////////////////////////////////////////
void WifiSetup()
{
   WiFi.mode(WIFI_AP_STA);
   uint8_t SoLanConn = 0;
   boolean TimWifi   = true ;
   uint8_t nWifi     = WiFi.scanNetworks();
   if(nWifi > 0)
   {
       for(uint8_t i=0; i < nWifi; i++ )
       {
          if(WifiName == WiFi.SSID(i))
          {
              TimWifi = true ;
              WiFiMulti.addAP( String2Char(WifiName) , String2Char(WifiPass) ); 
              while( WiFiMulti.run() != WL_CONNECTED ) 
              {  
                 delay(500); SoLanConn++; 
                 if(SoLanConn > 12) break; 
              }
           }
       }
       ///////////
       //if(TimWifi){  if( WiFiMulti.run() != WL_CONNECTED ) { WiFi.disconnect(); } }
   }
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void setup()
{
   Serial.begin(9600);
   myMac.replace(":", "");
   pinMode(GPIO2,  OUTPUT);
   Led_On();
   /////////////////////////////////// Access point
   String ssid      = "TAG4YU_SPW1_" + myMac;
   const char *pass = "0918180212";
   WiFi.softAP(String2Char(ssid), pass);
   server.begin();
   /////////////////////////////////// Load NFC NDEF
   if (setupIIC (ADDRESS_DYN5) == 0)
   {
        readIIC_data (ADDRESS_DYN5, 35, data_buff, 64);   
        GetWifiNamePass();
        Serial.println(); 
        Serial.println(WifiName + " - " + WifiPass);
        delay(1000);
        WifiSetup();
    }
    else{ Serial.println ("Load error"); }
    //////////////
}
//////////////////////////////////////////////////////////////////////////////////
String UartData   = "";
uint8_t ConnCount = 0;

void loop()
{
  server.handleClient();
  sUrl = "https://nfc2cloud.com/spw1/?id=" + myMac + "&stas=" + LedStas + "&dvip=" + myOnIP;
  ///////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ UartData+= KyTu; } delay(5);
  }
  if(UartData == "restart")   { Restart();         }
  if(UartData == "disconnect"){ WiFi.disconnect(); }
  ///////////////////////
  if( WiFiMulti.run() == WL_CONNECTED ) 
  {
       ConnCount = 0;
       myOnIP    = LayChuoiIP(WiFi.localIP());
       uint8_t Denled = GetHTTPSData(sUrl).substring(0,1).toInt();
       if(Denled == 1) { Led_On();  }
       if(Denled == 2) { Led_Off(); }
       if(Denled == 3) { Restart(); }
       Serial.println( (String)Denled + " - " + sUrl ); 
  } 
  else 
  { 
    ConnCount++;
    if(ConnCount > 240 ){ ConnCount = 0; Restart();  }
  }
  ///////////////////////
  delay(1000);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void TinhThoiGianReset()
{
  cGiay++;
  if(cGiay == 60){ cGiay = 0; cPhut++; }
  if(cPhut == 60){ cPhut = 0; cGio++;  }
  if( cGio > 23) { Restart(); }
}
//////////////////////////////////////////////////////////////////////////////////
String GetHTTPSData(String sUrl)
{
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;
    String payload = "";
    if (https.begin(*client, sUrl)) 
    { 
      int httpCode = https.GET();
      if (httpCode > 0)
      {
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {  payload = https.getString();   }
      }
      //else{  payload = https.errorToString(httpCode).c_str();  }
      https.end();
    }
    //else {  payload = "[HTTPS] Unable to connect\n";   }
    return payload;
}
