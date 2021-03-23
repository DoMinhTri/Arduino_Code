
#define Pin_AmThanh A2
uint8_t ThoiGianDelay    = 0;
uint16_t ThoiDiemHienTai = 0;
uint16_t ThoiDiemSauCung = 0;

void setup() {

  Serial.begin(9600);
  pinMode(9,  OUTPUT);
  pinMode(10, OUTPUT);
}
//////////////
void loop() {

   ThoiDiemHienTai = millis()/1000;
   int TinHieu     = analogRead(Pin_AmThanh);
   int Volume      = map(TinHieu - 300, 0, 256, 0, 10);
   if( Volume > 0) { ThoiGianDelay = 2; }
   
   if(ThoiGianDelay > 0 )
   {
       digitalWrite(9, HIGH);  
       digitalWrite(10, HIGH); 
       if(( ThoiDiemHienTai - ThoiDiemSauCung) > 1) 
       {
              ThoiGianDelay--;
              ThoiDiemSauCung = ThoiDiemHienTai;  
       }
   }
   else
   {
       digitalWrite(9, LOW);  
       digitalWrite(10, LOW); 
   }
   
    Serial.print(Volume);  Serial.print(" * ");  Serial.println(ThoiGianDelay); 
    delay(200);       
}


