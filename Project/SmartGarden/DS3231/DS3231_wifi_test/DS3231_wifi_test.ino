
#include <EEPROM.h>
////////////////////////////////////

#include <SoftwareSerial.h>
#define DEBUG true
SoftwareSerial Esp8266(3,4); // cai dat chan thu phat du lieu cho Esp8266, 10-RX, 11-TX                           
void setup()
{
  Serial.begin(9600);
  Esp8266.begin(9600); // chu y phai cung toc do voi ESP 8266


}
///////////////////////////////////////////////////////////////////////////////////////
String ChuoiDuLieu_Offline = "";
String ChuoiDuLieu_Online  = "";


void loop(void)
{
  
  if (Serial.available() > 0 ) 
  {
       char KyTu = Serial.read();
       if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu_Offline+= KyTu; }
       delay(5);
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
           Serial.println(ChuoiDuLieu_Online);
           ChuoiDuLieu_Online = "";
        }
   }
   /////////////////////////
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



