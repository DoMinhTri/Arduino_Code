
boolean BatDen = false;
int i=0;
unsigned long ThoiDiemHienTai = 0;
const int ledPin = A3; 
const int Relay  = 10; 
const int Sensor = A2; 

void setup() 
{
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(Relay,  OUTPUT);
    pinMode(Sensor, INPUT);
}

String ChuoiDuLieu;
void loop() 
{
  ThoiDiemHienTai = millis()/1000;
  uint8_t SS = analogRead(Sensor);
  
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
   
   if(ChuoiDuLieu != "")
   {
        Serial.print(ChuoiDuLieu);
        SuLyGiaTriChuoiNhan(ChuoiDuLieu);
        ChuoiDuLieu = "";
   }
   String sms = "UTS_MCU:" + (String)SS;
   Serial.println(sms);
   delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////
void SuLyGiaTriChuoiNhan(String ChuoiNhan)
{
  bool timthay         = false;
  String ChuoiGiaTri   = "";
  String ChuoiNhanDang = "";
  uint8_t nKyTu = ChuoiNhan.length(), i;
  for( i = 0; i< nKyTu; i++)
  {
    if(ChuoiNhan.substring(i, i+1) == ":"){ timthay = true; break; }
  }
  if(timthay)
  {
      ChuoiNhanDang = ChuoiNhan.substring(0, i);
      ChuoiGiaTri   = ChuoiNhan.substring(i+1, nKyTu);
  } else { ChuoiNhanDang = ChuoiNhan; }
  
  //if(ChuoiNhanDang == "UTS_DATA:")   { Wifi_LuuPass(ChuoiGiaTri); }
}

void BatDen(){  digitalWrite(ledPin, HIGH); }
void TatDen(){  digitalWrite(ledPin, LOW);  }

void BatRelay(){  digitalWrite(Relay, HIGH); }
void TatRelay(){  digitalWrite(Relay, LOW);  }



