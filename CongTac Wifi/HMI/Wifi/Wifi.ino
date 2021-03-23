
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiServer server(80);
String WifiName, WifiPass, WifiIP, WifiPort, SVID, DVNUM, AdmPass;

void Restart(){  delay(1000); ESP.restart(); }
//////////////////////////////////////////////////////////////////////////////////
String LayChuoiIP(IPAddress _IP)
{
    String _ChuoiIP = (String)_IP[0] + "." + (String)_IP[1] + "." + (String)_IP[2] + "." + (String)_IP[3] ;
    return _ChuoiIP;
}
//////////////////////////////////////////////////////////////////////////////////
IPAddress String2IP(String _ChuoiIP)
{
  _ChuoiIP+= ".";
  String _strChuoi = "";
  uint8_t nThuTu   = 0;
  uint8_t nIP[4]   = {0,0,0,0};
  for(int i=0; i< _ChuoiIP.length(); i++)
  {
    String sKyTu = _ChuoiIP.substring(i, i+1);
    if(sKyTu == ".")
    {
      String tmpSTR = _strChuoi;
      nIP[nThuTu] = tmpSTR.toInt();
      _strChuoi = "";
      nThuTu++;
    }
    else{ _strChuoi+= sKyTu; }
  }
  if(nThuTu != 4){ nIP[0]=0; nIP[1]=0; nIP[2]=0; nIP[3]=0; }
  IPAddress tmpIP(nIP[0], nIP[1], nIP[2], nIP[3]); 
  return tmpIP;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

const char* ssid     = "Duy Khanh 7";
const char* password = "passdk07";
//////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  delay(10);


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  server.begin();
  
}

void loop() {
  LayChuoiJson();
}

String LayChuoiJson()
{
        HTTPClient http;
        String WebCode = "";
        //String SVLink   = "http://fablabs.store/cttmm/inc/onoff/?UTSID=" + SVID + "&utstype=utsdevice&utsdvnum=" + DVNUM + "&utsvalue=" + TTCongTac + "&utsdvupdate=" + DeviceUpdate;
        String SVLink = "http://fablabs.store/cttmm/inc/onoff/?UTSID=11223344AABBCCDD&utsvalue=0,1,0,1&utstype=utsdevice&utsdvnum=1&utsdvupdate=0" ;
        http.begin(SVLink); //HTTP
        uint8_t httpCode = http.GET();
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK){ WebCode = http.getString(); }
        } 
        http.end();
        Serial.println(WebCode); 
        return WebCode;
        //////////
}
