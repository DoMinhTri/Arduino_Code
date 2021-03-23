

String ChuoiTinNhan = "";

///////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
}
///////////////////////////////////////////
void loop()
{
   while (Serial.available() > 0 ) 
  {
       char KyTu = Serial.read();
       if(KyTu != '\n' && KyTu != ' '){ ChuoiTinNhan+= KyTu; }
       delay(5);
   }
 
   if( ChuoiTinNhan != "") 
   { 
     Serial.println(ChuoiTinNhan);   
     ChuoiTinNhan = ""; 
    }
    
}
