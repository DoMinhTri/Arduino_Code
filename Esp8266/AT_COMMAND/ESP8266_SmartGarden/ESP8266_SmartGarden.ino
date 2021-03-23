
#include <EEPROM.h>
////////////////////////////////////

#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial Esp8266(3,4); // cai dat chan thu phat du lieu cho Esp8266, 10-RX, 11-TX                           
void setup()
{
  Serial.begin(9600);
  Esp8266.begin(9600); // chu y phai cung toc do voi ESP 8266
  
  pinMode(6,OUTPUT);// cai dat chan 11 la chan ra
  digitalWrite(6,HIGH);// tich cuc muc thap
  
  ConnectInternet();

}
///////////////////////////////////////////////////////////////////////////////////////
String ChuoiDuLieu_Offline = "";
String GT_Off_1, GT_Off_2, GT_Off_3;


String ChuoiDuLieu_Online  = "";
boolean TraLoi = false;

void loop(void)
{
  
  //ConnectInternet();
//delay(5000);
  if (Serial.available() > 0 ) 
  {
       char KyTu = Serial.read();
       if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu_Offline+= KyTu; }
       delay(5);
   }
   else
   { 
       if(ChuoiDuLieu_Offline !="")
       {
          ChuoiDuLieu_Offline.trim();
          String ATCom = ChuoiDuLieu_Offline.substring(0, 3);
          if((ATCom == "AT+") || (ATCom == "AT"))
          {
            Serial.print("[" + ChuoiDuLieu_Offline + "]=> "); 
            String TraLoi = sendData(ChuoiDuLieu_Offline,1000,DEBUG);
          }   
          if(ATCom == "DMT"){ ConnectInternet(); }  
          ChuoiDuLieu_Offline = ""; 
       }
   }
  ///////////////////////////
  if(Esp8266.available()) 
   {
         char KyTu = Esp8266.read();
         if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu_Online+= KyTu; }
         delay(5);
   }
  else
  { 
        if(ChuoiDuLieu_Online != "")
        {
           String strData = LayChuoiDuLieu(ChuoiDuLieu_Online);
           if(strData == "led=ok") { digitalWrite(6, !digitalRead(6)); }
           ChuoiDuLieu_Online = "";
           TraLoi = true;
        }
        else
        {
            if(TraLoi)
            {
              WebPage((String)digitalRead(6));
              delay(5000);
              TraLoi = false;
            }
        } 
   }
   /////////////////////////
}
///////////////////////////////////////////////////////////////////////////////////////
String TrangThaiLed()
{
  String LedStas = "On";
  if(digitalRead(6)) LedStas = "Off";
  return LedStas;
}
///////////////////////////////////////////////////////////////////////////////////////
String LayChuoiDuLieu(String strDuLieu)
{
  int nTong  = strDuLieu.length();
  int nIndexDau = 0, nIndexCuoi = 0;
  String strChuoi_1, strChuoi_2, strChuoi_3;
  for(int j=0; j<nTong; j++)
  {
    String kytu = strDuLieu.substring(j, j+1);
    if(kytu == "(" )
    {
      if(nIndexDau == 0 && nIndexCuoi == 0)
      {  
        nIndexDau = j; 
      }
      else {  nIndexCuoi = j; break; }
    }
  }
  strChuoi_1 = strDuLieu.substring(nIndexDau, nIndexDau+5);
  strChuoi_2 = strDuLieu.substring(nIndexDau + 5, nIndexCuoi);
  strChuoi_3 = strDuLieu.substring(nIndexCuoi, nIndexCuoi+5);
  Serial.print(strChuoi_1);
  Serial.print("-");
  Serial.print(strChuoi_2);
  Serial.print("-");
  Serial.println(strChuoi_3);
  return strChuoi_2;
}
///////////////////////////////////////////////////////////////////////////////////////
void WebPage(String html) {
  String Header;
  Header  = "HTTP/1.1 200 OK\r\n";
  Header += "Content-Type: text/html\r\n";
  Header += "Connection: close\r\n";  
  Header += "Refresh: 5\r\n";
  Header += "Content-Length: ";
  Header += (int)(html.length());
  Header += "\r\n\r\n";
  Esp8266.print("AT+CIPSEND=0,");
  Esp8266.println(Header.length()+html.length());
  delay(10);
  if (Esp8266.find(">")) {
      Esp8266.print(Header);
      Esp8266.print(html);
      delay(10);
   }
}
///////////////////////////////////////////////////////////////////////////////////////
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    Esp8266.print(command); // send the read character to the esp8266
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(Esp8266.available())
      {
        char c = Esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    if(debug){ Serial.print(response); }
    return response;
}

void ConnectInternet()
{
 //========================gui tap lenh AT cho ESP 8266 de cai dat cac tham so cho WiFi================================
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CWSAP=\"MEXUAN_network\",\"lichvannien\",1,4\r\n",1000,DEBUG);// { Connectify-MrT --  dmt@1234 } { MEXUAN_network  ----  lichvannien  }  
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////  EEPROM    //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){ EEPROM.write(DiaChi, KyTu); delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){ EEPROM.write( DiaChi, SoLuu ); delay(20); }
///////////////////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)EEPROM.read(DiaChi); }
///////////////////////////////////////////////////////////////////////////////////////
int EEPROM_RN(int DiaChi) { return  EEPROM.read(DiaChi);  }
///////////////////////////////////////////////////////////////////////////////////////
