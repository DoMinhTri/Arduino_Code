#define Led 13
String ChuoiData = "";

void setup() {
  Serial.begin(9600);
  pinMode(Led, OUTPUT);
}




void loop() {

  ////////////
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiData+= KyTu; } delay(5);
  }

  if(ChuoiData != "") 
  { 
    
       if(ChuoiData == "a")
       {
           digitalWrite(Led, HIGH);   delay(1000);
       }
       else
       {
          digitalWrite(Led, LOW);    delay(1000);
       }
       Serial.println(ChuoiData);
       ChuoiData = ""; 
   }
}
