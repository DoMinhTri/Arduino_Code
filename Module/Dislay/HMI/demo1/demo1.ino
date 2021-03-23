

void setup() {
  Serial.begin(9600);
  
//  HMI.begin(9600);
//  HMI.print("baud=115200");  
//  HMI.write(0xff);
//  HMI.write(0xff);
//  HMI.write(0xff);
//  HMI.end();  
//  HMI.begin(115200);

}
int GT           = 0; 
String ChuoiData = "";

void loop() {

  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read();  uint8_t nbKyTu = (uint8_t)KyTu;  
      if( 31<nbKyTu && nbKyTu < 127 ){ ChuoiData+= KyTu; } delay(5);
  }
 
  if(ChuoiData != "") { SuLyTinHieu(ChuoiData); ChuoiData = ""; }

  SendGT();
  delay(200);
}
////////////////////////////////////////
void SuLyTinHieu(String Chuoi)
{
//  if(Chuoi == "p1Off")
//  {
//      HMISendInt("p0.pic=", 1);
//      HMISendTxT("t0.txt=", "1On");
//  }
//  if(Chuoi == "p1On")
//  {
//      HMISendInt("p0.pic=", 0);
//      HMISendTxT("t0.txt=", "1Off");
//  }
//  if(Chuoi == "p2Off")
//  {
//      HMISendInt("p1.pic=", 1);
//      HMISendTxT("t1.txt=", "2On");
//  }
//  if(Chuoi == "p2On")
//  {
//      HMISendInt("p1.pic=", 0);
//      HMISendTxT("t1.txt=", "2Off");
//  }
  Serial.println(Chuoi);
}
////////////////////////////////////////
void SendGT()
{
  GT = GT + 10;  
  if(GT > 360) GT = 10; 
  HMISendInt("z0.val=", GT);  
}
////////////////////////////////////////
void HMISendInt(String DoiTuong, int GiaTri)
{
  Serial.print(DoiTuong);
  Serial.print(GiaTri);
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
////////////////////////////////////////
void HMISendTxT(String DoiTuong, String GiaTri)
{
  Serial.print(DoiTuong);
  Serial.print("\"" + GiaTri + "\"");
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
