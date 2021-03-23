#include <WiFiClientSecure.h>
WiFiClientSecure client;
//////////////////////////////////////////////////////////////////////////////////
String HTTPS_Data(char* Host, String sURL)
{
   String GT2  = "", GT3 = "";
   ////////////////////////////
   if (!client.connect(Host, 443)) {   }   
   client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: " + Host + "\r\nConnection: close\r\n\r\n");
   ////////////////////////////
   while (client.connected()){ sURL = client.readStringUntil('\n');  break; }
   while (client.available()){ GT3  = GT2; GT2  = sURL; sURL = client.readStringUntil('\n'); }
   ////////////////////////////
   client.stop();  sURL.trim();
   if(sURL == ""){ if( GT2.toInt() == 0 ){  GT3.trim(); sURL = GT3; }}
   ////////////////////////////
   return sURL;
}
//////////////////////////////////////////////////////////////////////////////////
String GoogleSheet_Data( String sURL)
{
   if (!client.connect("script.google.com", 443)) {   }   
   client.print(String("GET ") + sURL + " HTTP/1.1\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
   ////////////////////////////
   while (client.connected()) 
   {
     String lines = client.readStringUntil('\n');
     int8_t ind   = lines.indexOf("https://");
     if(ind > 0){ sURL = lines.substring(ind); break;  }
   }
   client.stop();
   ////////////////////////////
   return HTTPS_Data("script.google.com", sURL);;
}
//////////////////////////////////////////////////////////////////////////////////
