
boolean BatDen   = false;
boolean BatRelay = false;

int i=0;
unsigned long ThoiDiemHienTai = 0;
unsigned long ThoiGianRelay   = 0;

const int ledPin = A3; // A3
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
  /////////////////////
  if(BatDen){
    if(ThoiDiemHienTai %2==0){ BatDenLed(); }
    else{ TatDenLed(); }
  } else{  TatDenLed(); }
  /////////////////////
  if(BatRelay)
  {
      if(ThoiDiemHienTai < (ThoiGianRelay + 30))
      {
        ChoBatRelay();
      }
      else
      {
         ChoTatRelay();
      }
  }
  /////////////////////
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
   
   if(ChuoiDuLieu != "")
   {
        SuLy(ChuoiDuLieu);
        ChuoiDuLieu = "";
   }
   String sms = "UTS_MCU:" + (String)SS;
   Serial.println(sms);
   delay(1000);
}

//////////////////////////////////////////////////////////////////////////////////

void SuLy(String smsg)
{
    if(smsg == "UTSA"){ BatDen   = true;  }
    if(smsg == "UTSB"){ BatDen   = false; }
    if(smsg == "UTSC"){ BatRelay = true; ThoiGianRelay = ThoiDiemHienTai; }
}

void BatDenLed(){  digitalWrite(ledPin, HIGH); }
void TatDenLed(){  digitalWrite(ledPin, LOW);  }

void ChoBatRelay(){  digitalWrite(Relay, HIGH); }
void ChoTatRelay(){  digitalWrite(Relay, LOW);  }



