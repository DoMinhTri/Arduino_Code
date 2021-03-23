
#include <SoftwareSerial.h>
SoftwareSerial Esp8266(3, 4); // RX, TX 
////////////////////////////////////////////////////////////////////////////////////  
void setup() { 
      
  Serial.begin(9600);   
  Esp8266.begin(9600);   
}
////////////////////////////////////////////////////////////////////////////////////  
String ChuoiDuLieu = "";
void loop() 
{
  if (Serial.available() > 0 ) 
  {
       char KyTu = Serial.read();
       if(KyTu != '\n' && KyTu != ' '){ ChuoiDuLieu+= KyTu; }
       delay(5);
   }
   else
   {
     if(ChuoiDuLieu != "")
     {
       //Serial.println(ChuoiDuLieu);
       //CaiDatWifiData(ChuoiDuLieu);
       //Serial.println("");
       //Serial.println(ThuGonChuoiData( ChuoiDuLieu));
       String2IP(ChuoiDuLieu);
       ChuoiDuLieu = "";
     }
   }
   delay(50);
}

////////////////////////////////////////////////////////////////////////////////////
String ThuGonChuoiData(String _ChuoiData)
{
  uint8_t i; 
  _ChuoiData.replace("_", "");
  for(i=0; i < _ChuoiData.length(); i++)
  {
      String sKyTu = _ChuoiData.substring(i, i+1);
      if(sKyTu == "X"){ break; }
  } 
  String tmpData = _ChuoiData.substring(i+1, _ChuoiData.length());
  return tmpData;
}
////////////////////////////////////////////////////////////////////////////////////
void CaiDatWifiData(String _ChuoiData)
{
  String tmpSTR      = ThuGonChuoiData(_ChuoiData);
  String _strChuoi   = "";
  uint8_t nThuTu     = 0;
  String MangData[8] = {"","","","","","","",""};
  
  for(uint8_t i=0; i< tmpSTR.length(); i++)
  {
    String sKyTu = tmpSTR.substring(i, i+1);
    if( sKyTu == "," )
    {
      MangData[nThuTu] = _strChuoi;
      _strChuoi = "";
      nThuTu++;
    }
    else{ _strChuoi+= sKyTu; }
  }
  for(uint8_t i=0; i< 8; i++)
  {
     Serial.print("["+ MangData[i] + "]-");
   }
}

//////////////////////////////////////////////////////////////////////////////////
void String2IP(String _ChuoiIP)
{
  _ChuoiIP+= ".";
  uint8_t nThuTu  = 0;
  String strChuoi = "";
  uint8_t nIP[4]  = {0,0,0,0};   
  for(uint8_t i=0; i< _ChuoiIP.length(); i++)
  {
    String sKyTu = _ChuoiIP.substring(i, i+1);
    if(sKyTu == ".")
    {
       String tpStr = strChuoi;
       nIP[nThuTu]  = tpStr.toInt();
       strChuoi     = "";
       nThuTu++;
    }else{strChuoi+= sKyTu; }
  }
  Serial.println(nIP[0]);
  Serial.println(nIP[1]);
  Serial.println(nIP[2]);
  Serial.println(nIP[3]);
}
