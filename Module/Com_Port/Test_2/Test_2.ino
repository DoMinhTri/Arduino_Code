

int i=0;
const int ledPin = 2; 

void setup() 
{
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
}

String ChuoiDuLieu;
void loop() 
{
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
   
   if(ChuoiDuLieu != "")
   {
        Serial.print(ChuoiDuLieu);
        if(ChuoiDuLieu == "1") Serial.println("-1");
        if(ChuoiDuLieu == "2") Serial.println("-2");
        if(ChuoiDuLieu == "3") Serial.println("-3");
        if(ChuoiDuLieu == "4") Serial.println("-4");
        if(ChuoiDuLieu == "5") Serial.println("-5");
        if(ChuoiDuLieu == "6") Serial.println("-6");
        if(ChuoiDuLieu == "7") Serial.println("-7");
        if(ChuoiDuLieu == "8") Serial.println("-8");
        if(ChuoiDuLieu == "9") Serial.println("-9");
        ChuoiDuLieu = "";
   }
    delay(500);
}

void BatDen(){
  digitalWrite(ledPin, HIGH); 
  Serial.println("- On"); 
}

void TatDen(){
  digitalWrite(ledPin, LOW); 
  Serial.println("+ Off"); 
}


