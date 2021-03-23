


#define DC_A1 0
#define DC_A2 5

#define DC_B1 2
#define DC_B2 4

/////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  
  pinMode(DC_A1, OUTPUT); //  motor A direction
  pinMode(DC_A2, OUTPUT); //  motor A speed
  
  pinMode(DC_B1, OUTPUT); //  motor B direction
  pinMode(DC_B2, OUTPUT); //  motor B speed
}
/////////////////////////////////////////////////////////////////////////////
String ChuoiDuLieu = "";

void loop() { 

   ////////////////////////////////////
   while (Serial.available() > 0 ) 
   {
        char KyTu = Serial.read();  uint8_t nbKyTu = (int)KyTu;  
        if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiDuLieu+= KyTu; } delay(5);
   }
   ////////////////////////////////////
   if(ChuoiDuLieu != "")
   {
      String  Huong = ChuoiDuLieu.substring(0, 1);
  
      if(Huong == "T") { Xe_Toi(); }
      if(Huong == "t") { Xe_Lui(); }
      if(Huong == "P") { Xe_Trai();}
      if(Huong == "p") { Xe_Phai();}

      if(Huong == "1") { Xe_Toi();}
      if(Huong == "2") { Cung_Phan_Tu_1();}
      if(Huong == "3") { Xe_Phai();}
      if(Huong == "4") { Cung_Phan_Tu_4();}
      if(Huong == "5") { Xe_Lui();}
      if(Huong == "6") { Cung_Phan_Tu_3();}
      if(Huong == "7") { Xe_Trai();}
      if(Huong == "8") { Cung_Phan_Tu_2();}
      if(Huong == "9") { Xe_Ngung();}

      ChuoiDuLieu = "";
   }
   
   delay(25);
}

/////////////////////////////////////////////////////////////////////////////

void Xe_Ngung()
{
  digitalWrite(DC_A1, LOW); 
  digitalWrite(DC_A2, LOW); 

  digitalWrite(DC_B1, LOW); 
  digitalWrite(DC_B2, LOW);
}
//////////////////////////////////////////
void Xe_Toi() { BanhPhai_Toi(); BanhTrai_Toi(); }
void Xe_Lui() { BanhPhai_Lui(); BanhTrai_Lui(); }

void Xe_Trai(){ Xoay_Trai(); }
void Xe_Phai(){ Xoay_Phai(); }

void Xoay_Phai(){ BanhTrai_Toi(); BanhPhai_Lui(); }
void Xoay_Trai(){ BanhPhai_Toi(); BanhTrai_Lui(); }
//////////////////////////////////////////
void BanhPhai_Toi()
{
  digitalWrite(DC_A1, LOW); 
  digitalWrite(DC_A2, HIGH); 
}

void BanhPhai_Lui(){
  digitalWrite(DC_A1, HIGH); 
  digitalWrite(DC_A2, HIGH);
}

void BanhPhai_Ngung(){
  digitalWrite(DC_A1, LOW); 
  digitalWrite(DC_A2, LOW); 
}
/////////////////////////
void BanhTrai_Toi()
{
  digitalWrite(DC_B1, LOW); 
  digitalWrite(DC_B2, HIGH); 
}

void BanhTrai_Lui(){
  digitalWrite(DC_B1, HIGH); 
  digitalWrite(DC_B2, HIGH); 
}

void BanhTrai_Ngung()
{
  digitalWrite(DC_B1, LOW); 
  digitalWrite(DC_B2, LOW); 
}
