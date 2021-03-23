
#include <IPAddress.h>

//////////////////////////////////////////////////////////////////////////////////
String LayChuoiIP(IPAddress _IP){   String _ChuoiIP = (String)_IP[0] + "." + (String)_IP[1] + "." + (String)_IP[2] + "." + (String)_IP[3] ;   return _ChuoiIP; }
//////////////////////////////////////////////////////////////////////////////////
void WifiNameSearch(String sName)
{
   ListSSID = "";
   TimWifi  = false;
   uint8_t n = WiFi.scanNetworks();
   ///////////////
   for (uint8_t i = 0; i < n; i++)
   { 
      if(sName == WiFi.SSID(i)){ TimWifi = true; } 
      ListSSID += "<input type='radio' class='radio' name='wname' value='"+   WiFi.SSID(i) +"' onclick='getName(this.value)'><font size=2>" +  WiFi.SSID(i) + "</font><br>";
      delay(100); 
   }
}
//////////////////////////////////////////////////////////////////////////////////
void SuLyGiaTriChuoiNhan(String sData)
{
  String sDDau = sData.substring(0, 5);
  String sDSau = sData.substring(6, sData.length());
  if(sDDau == "WNAME"){ Wifi_LuuName(sDSau); }
  if(sDDau == "WPASS"){ Wifi_LuuPass(sDSau); }
  if(sDDau == "RESET"){ Restart(); }
  Serial.println(sDSau);
}
//////////////////////////////////////////////////////////////////////////////////
void WifiConnect(String sName, String sPass)
{
     WiFi.mode(WIFI_STA);  
     WiFi.begin(String2Char(sName), String2Char(sPass));
}
//////////////////////////////////////////////////////////////////////////////////
void Accesspoint(String sName)
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(String2Char(sName) , "dmt@xxx@123");
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
