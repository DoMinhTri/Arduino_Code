/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"
#include <Wire.h>
#define LED_BUILTIN 2
#define I2C_SDA D4 //GPIO04
#define I2C_SCL D5 //GPIO14
#define ADDRESS_DYN5 0xA6

#define ledPin  LED_BUILTIN;
//////////////////////////////////////////////
void setup_wifi ()
{
    WiFi.mode (WIFI_STA);
    WiFi.disconnect();
    delay(1000);
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i)
    {
        Serial.println (WiFi.SSID (i));
    }
}
//////////////////////////////////////////////
int setupIIC (uint8_t slave_addr)
{
    Wire.begin (I2C_SDA, I2C_SCL);
    Wire.beginTransmission (slave_addr / 2);
    return  Wire.endTransmission();
}
//////////////////////////////////////////////
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
//////////////////////////////////////////////

void setup()
{
    Serial.begin (115200);
    //setup_wifi();
    
    if (setupIIC (ADDRESS_DYN5) == 0)
        Serial.println ("I2C setup OK");
    else
        Serial.println ("I2C setup error");
        
    Serial.println ("Setup done");
	  Serial.println ("We read DYN5 Tag each 8 bytes");
}

String ChuoiDuLieu = "";
void loop()
{
    
  ///////////////////////
  /*
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
    ///////////////////////  
   if(ChuoiDuLieu != "")
   {
      SuLyGiaTriChuoiNhan(ChuoiDuLieu);
      ChuoiDuLieu = "";
   }
   */
  ///////////////////////  
    
    while (Serial.available() <= 0) {}
    
    byte add = (Serial.read() - 0x30) * 10;
    
    while (Serial.available() <= 0) {}
    
    add += Serial.read() - 0x30;
    Serial.println();
    uint8_t data_buff[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    readIIC_data (ADDRESS_DYN5, add, data_buff, 8);
    
    for (int i = 0; i < 8; i++)
    {
        if(data_buff[i] < 0x0F) Serial.print("0");
        Serial.print (data_buff[i], HEX);
        Serial.print (":");
    }
    Serial.println();
}

String WifiName = "",  WifiPass = "";
///////////////////////  
void SuLyGiaTriChuoiNhan(String ChuoiNhan)
{
  uint8_t nKyTu =  ChuoiNhan.length();
  String CDau   = ChuoiNhan.substring(0, 2);
  String CCuoi  = ChuoiNhan.substring(3, nKyTu);
  
  if(CDau  == "WN") { WifiName = CCuoi;  }
  if(CDau  == "WP") { WifiPass = CCuoi;  }
  if((WifiName != "") && (WifiPass != ""))
  {  
      Serial.println("Connect wifi");
      WiFi.begin (WifiName, WifiPass); 
      WifiName = ""; WifiPass = "";
      while (WiFi.status() != WL_CONNECTED){ delay (1000);  Serial.print (".");  }
      if(WiFi.status() == WL_CONNECTED)  Serial.println (WiFi.localIP());
  }
}
