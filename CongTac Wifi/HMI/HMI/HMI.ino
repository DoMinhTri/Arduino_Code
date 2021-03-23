/////////////////////  Wifi 
uint8_t TTOnline      = 0;
String WifiName, WifiPass, WifiIP, WifiPort, SVID, DVNUM, AdmPass;

///////////////////// HMI
String UartData       = "";
bool Load_1_Lan       = false;
uint8_t HMIAction     = 0;
uint8_t HMITextDislay = 0;
String  TenManHinh[5] = {"pHMIMAIN","pHMIWIFI","pHMISETT","pHMIDATE","pHMIHELP"};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////       Function        //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////       Hàm kiểm tra dữ liệu online có động bộ với dữ liệu gốc không
void KiemTraDataOnline(uint8_t TT1, uint8_t TT2, uint8_t TT3, uint8_t TT4)
{
    if(TT1 == MangTTDen[0])
    {
         if(TT2 == MangTTDen[1])
         {
               if(TT3 == MangTTDen[2])
               {
                    if(TT4 == MangTTDen[3]) { DeviceUpdate = 0;  }
               }
         }
    }
}
///////////////////////////////////////////////////////////////////////////
void UartDataCheck(String UartDataX)
{
    if(UartDataX == "pESPRESTART"){ Restart(); }
    if(UartDataX == "ESPRESTART") { Restart(); }
    String SDau = UartDataX.substring(0, 6);
    String SSau = UartDataX.substring(6, UartDataX.length());
    if(SDau == "WName:"){ Wifi_LuuName( SSau ); UartDataX = "";  }
    if(SDau == "WPass:"){ Wifi_LuuPass( SSau ); UartDataX = "";  }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////       HMI         //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void LayChiSoManHinh(String UartDataX)
{
    for(uint8_t i=0; i < sizeof(TenManHinh); i++)
    {
      if(UartDataX == TenManHinh[i]) { HMIAction = i; HMITextDislay = i; }
    }
}
//////////////////////////////////////////////////////////////////////////////////
void HMIDataCheck(String ChuoiData)
{
    UartDataCheck(ChuoiData);
    LayChiSoManHinh(ChuoiData);
    String _KyTuDau = ChuoiData.substring(0, 3);
    String _HMIData = ChuoiData.substring(3, ChuoiData.length());
    if(ChuoiData == "pOneLoad") { Load_1_Lan = true; }
    ///////////
    switch (HMIAction) {
    case 1:
          if(_KyTuDau == "pWN"){ WifiName = _HMIData ;  Wifi_LuuName( WifiName );  }
          if(_KyTuDau == "pWP"){ WifiPass = _HMIData ;  Wifi_LuuPass( WifiPass );  Load_1_Lan = true;}
      break;
    case 2:
          if(_KyTuDau == "pPW"){ AdmPass  = _HMIData ;  Admin_LuuPass( _HMIData ); }
          if(_KyTuDau == "pID"){ SVID     = _HMIData ;  SV_LuuSVID(    _HMIData ); }
          if(_KyTuDau == "pOD"){ DVNUM    = _HMIData ;  SV_LuuDVNUM(   _HMIData ); }
          if(_KyTuDau == "pIP"){ WifiIP   = _HMIData ;  Wifi_LuuIP(    _HMIData ); }
          if(_KyTuDau == "pPO"){ WifiPort = _HMIData ;  Wifi_LuuPort(  _HMIData ); }
      break;
    default:
          if(_KyTuDau == "pN1") { MangTTDen[0] = (MangTTDen[0] + 1)%2; DeviceUpdate = 1; }
          if(_KyTuDau == "pN2") { MangTTDen[1] = (MangTTDen[1] + 1)%2; DeviceUpdate = 1; }
          if(_KyTuDau == "pN3") { MangTTDen[2] = (MangTTDen[2] + 1)%2; DeviceUpdate = 1; }
          if(_KyTuDau == "pN4") { MangTTDen[3] = (MangTTDen[3] + 1)%2; DeviceUpdate = 1; }
    break;
  }
}
//////////////////////////////////////////////////////////////////////////////////
void HMIDislay()
{
  switch (HMITextDislay) {
    case 1:
          HMISendInt("tpWifiVal", TTOnline);
          if(Load_1_Lan)
          {
              HMISendTxT("tWName", WifiName);
              HMISendTxT("tWPass", WifiPass);
              //////////////////////
              uint8_t ap_count = WiFi.scanNetworks();
              if(ap_count > 6) ap_count = 6;
              if(ap_count > 0)
              {
                  for (uint8_t ap_idx = 0; ap_idx < ap_count; ap_idx++)
                  {
                      //String TenWifi = "Duy Khanh 7"; 
                      String TenWifi = String(WiFi.SSID(ap_idx));
                      HMISendTxT("tW" + String(ap_idx) , String(TenWifi));
                  }
              }
              Load_1_Lan = false;
          }
      break;
    case 2:
          HMISendInt("tpWifiVal" , TTOnline );
          if(Load_1_Lan)
          {
            HMISendTxT("tPass"  ,AdmPass);
            HMISendTxT("tDVID"  ,SVID);
            HMISendTxT("tOrder" ,DVNUM);
            HMISendTxT("tIP"    ,WifiIP);
            HMISendTxT("tPort"  ,WifiPort);
            ///////////////
            Load_1_Lan = false;
          }
      break;
    default:
          String Date = String(Ngay) + "/" + String(Thang);
          String Time = String(Gio)  + ":" + String(Phut) + ":" + String(Giay);
          /////////////
          HMISendTxT("tDate" ,Date);
          HMISendTxT("tTimer",Time);
          HMISendTxT("tNhietDo", String(NhietDo) + "*C");
          HMISendTxT("tDoAm"   , String(DoAm)    + "%" );
          /////////////
          HMISendPic("bD1"   , MangTTDen[0] );
          HMISendPic("bD2"   , MangTTDen[1] );
          HMISendPic("bD3"   , MangTTDen[2] );
          HMISendPic("bD4"   , MangTTDen[3] );
          /////////////
          HMISendInt("tpWifiVal" , TTOnline );
          HMISendInt("vMua"      , TroiMua );
          HMISendTxT("tNam"      , String(Nam));
    break;
  }
}
//////////////////////////////////////////////////////////////////////////////////
void HMISendInt(String DoiTuong, int GiaTri)
{
  Serial.print(DoiTuong + ".val=");
  Serial.print(GiaTri);
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
////////////////////////////////////////
void HMISendTxT(String DoiTuong, String GiaTri)
{
  Serial.print(DoiTuong + ".txt=");
  Serial.print("\"" + GiaTri + "\"");
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
////////////////////////////////////////
void HMISendPic(String DoiTuong, uint8_t GiaTri)
{
  Serial.print(DoiTuong + ".picc=");
  Serial.print(GiaTri);
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////       Main        //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void setup() {
  EEPROM.begin(512);   delay(100);
  Serial.begin(9600);  delay(100);
}
////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  ////////////////////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ UartData+= KyTu; } delay(5);
  }
  ////////////
  if(UartData != "") { HMIDataCheck(UartData); UartData = ""; }
  //////////////////////////////
  HMIDislay();
  delay(500);
  //////////////////////////////
}


