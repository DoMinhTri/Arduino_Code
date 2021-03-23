
/*
airspectrum.cdnstream1.com:8114/1648_128 #  1 - Easy Hits Florida 128k Y
109.206.96.34:8100                       #  0 - NAXI LOVE RADIO, Belgrade, Serbia
us2.internet-radio.com:8050              #  2 - CLASSIC ROCK MIAMI 256k
airspectrum.cdnstream1.com:8000/1261_192 #  3 - Magic Oldies Florida
airspectrum.cdnstream1.com:8008/1604_128 #  4 - Magic 60s Florida 60s Classic Rock
us1.internet-radio.com:8105              #  5 - Classic Rock Florida - SHE Radio
icecast.omroep.nl:80/radio1-bb-mp3       #  6 - Radio 1, NL
205.164.62.15:10032                      #  7 - 1.FM - GAIA, 64k
skonto.ls.lv:8002/mp3                    #  8 - Skonto 128k
94.23.66.155:8106                        #  9 - *ILR CHILL and GROOVE

 "http://wbgo.streamguys.net/wbgo96",
  "http://wbgo.streamguys.net/thejazzstream",
  "http://stream.srg-ssr.ch/m/rsj/mp3_128",
  "http://37.187.79.93:8368/stream2",
  "http://icecast.omroep.nl/3fm-sb-mp3",
  "http://beatles.purestream.net/beatles.mp3",
  "http://listen.181fm.com/181-beatles_128k.mp3",
  "http:://stream.srg-ssr.ch/m/rsj/mp3_128",

VS1053 - connections detail
XRST = EN (D3)
 MISO = D19
 MOSI = D23
 SCLK = D18
 VCC = 5V / 3.3 V
 Gnd = Gnd 
  
*/


#include <VS1053.h>
#include "helloMp3.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wifi.h>
////////////////////////////////////////////////////////////// 
#define LED_BUILTIN 2
void BatDen(){  digitalWrite(LED_BUILTIN, HIGH);}
void TatDen(){  digitalWrite(LED_BUILTIN, LOW); }
//////////////////////////////////////////////////////////////
char ssid[] = "Robo-T";       //  your network SSID (name) 
char pass[] = "dmt@123abc";   // your network password
//////////////////////////////////////////////////////////////  37.187.79.93:8368/stream2
char *host[9]  = {"listen.181fm.com"     ,"radiostreaming.ert.gr","realfm.live24.gr","149.255.59.162", "wbgo.streamguys.net","37.187.79.93","us4.internet-radio.com","192.168.1.4","uptosmart.ddns.net"};
char *path[9]  = {"/181-beatles_128k.mp3","/ert-kosmos"          ,"/realfm"         ,"/1"            , "/wbgo96"            ,"/stream2"    , "/stream"              ,"/radio"     ,"/radio"};
int   port[9]  = {80                     ,80                     ,80                ,8062            , 80                   , 8368         , 8266                   ,8000         , 92     };
//////////////////////////////////////////////////////////////
WiFiClient  client;
int nChanel = 8;
uint8_t mp3buff[32];   // vs1053 likes 32 bytes at a time
//////////////////////////////////////////////////////////////
// Wiring of VS1053 board (SPI connected in a standard way)
#define VS1053_CS    27  //32
#define VS1053_DCS   14  //33
#define VS1053_DREQ  26  //35 // 26 // 10
#define VOLUME       90  // volume level 0-100
VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);
//////////////////////////////////////////////////////////////
void station_connect (int station_no ) {
    //String sUrl = "Connected : " + host[station_no] + ":" + port[station_no] + path[station_no] ;
    client.stop();
    if (client.connect(host[station_no],port[station_no]) ) {  Serial.println(host[station_no]); BatDen();  }
    client.print(String("GET ") + path[station_no] + " HTTP/1.1\r\n" + "Host: " + host[station_no] + "\r\n" + "Connection: close\r\n\r\n");     
}
//////////////////////////////////////////////////////////////
void setup () 
{
  Serial.begin(115200);
  delay(500);
  SPI.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  TatDen();
  ////////////////////////////////////////
  player.begin();
  player.setVolume(VOLUME);
  ////////////////////////////////////////
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print(".");  }
  Serial.println(WiFi.localIP());
  delay(100);
  ////////////////////////////////////////
  player.playChunk(hello3, sizeof(hello3)); //VS1053 is wake up & running
  delay(1000); 
  player.softReset(); 
  station_connect(nChanel); 
  delay(5000); 
}
//////////////////////////////////////////////////////////////
String cChanel = "";
void loop() 
{
   ////////////////////////
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ cChanel+= KyTu; } delay(5);
   }
   ////////////////////////
   if(cChanel != "")
   {
     TatDen();
     nChanel = cChanel.toInt();
     player.softReset();
     station_connect(nChanel);
     delay(1000); 
     cChanel = "";
   }
   ////////////////////////
   // if (!client.connect(host[nChanel],port[nChanel])){ TatDen(); station_connect(nChanel); }
   ////////////////////////
   if (client.available() > 0) {
      uint8_t bytesread = client.read(mp3buff, 32);
      player.playChunk(mp3buff, bytesread);
   }
   ////////////////////////
}

//////////////////////////////////////////////////////////////
