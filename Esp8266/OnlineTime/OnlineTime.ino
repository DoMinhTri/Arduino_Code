#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
///////////////////////////////////////////////
const char *ssid          = "Robo-T";
const char *password      = "dmt@123abc";
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
///////////////////////////////////////////////
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 3600);
///////////////////////////////////////////////
uint8_t tpVNTime, VNTime, VNDay;


void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  timeClient.begin();
}

void loop() {
  timeClient.update();
  
  tpVNTime = timeClient.getHours() + 6 ;
  VNTime   = tpVNTime%24  ;
  
  if( tpVNTime < 24)
  {
      VNDay = timeClient.getDay();
  }
  else
  {
     VNDay = (timeClient.getDay() + 1)%7 ;
  }
  ////////////////////
  Serial.print(timeClient.getHours());
  Serial.print(" , ");
  Serial.print(VNTime);
  Serial.print(" - ");
  Serial.print(timeClient.getDay());
  Serial.print(" , ");
  Serial.print(VNDay);
  Serial.print(" *** ");
  ////////////////////
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
