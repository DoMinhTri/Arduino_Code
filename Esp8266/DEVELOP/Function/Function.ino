
void setup()
{
  Serial.begin(9600);

}
///////////////////////////////////////////////////////////////////////////////////////
String DATA = "";
void loop(void)
{
  if(Serial.available()) 
   {
         char KyTu = Serial.read();
         if(KyTu != '\n' && KyTu != ' '){ DATA+= KyTu; }
         delay(5);
   }
   else
   {
     if(DATA !="") 
     {
         //Serial.println(String2IP(DATA));
         //String2Int(DATA);
         //Serial.println((String)((char)1));
         String2IP(DATA);
         DATA = "";
     }
   }
   /////////////////////////
}

void String2IP(String _ChuoiIP)
{
       _ChuoiIP    = _ChuoiIP + ".";
  String _strChuoi = "";
  uint8_t nThuTu = 0;
  uint8_t _MangIP[4] = {0,0,0,0};
    
  for(uint8_t i=0; i< _ChuoiIP.length(); i++)
  {
    String sKyTu = _ChuoiIP.substring(i, i+1);
    if(sKyTu == ".")
    {
      _MangIP[nThuTu] = _strChuoi.toInt();
      _strChuoi = "";
      nThuTu++;
    }
    else{ _strChuoi+= sKyTu; }
  }
  Serial.println(_MangIP[0]);
  Serial.println(_MangIP[1]);
  Serial.println(_MangIP[2]);
  Serial.println(_MangIP[3]);
}

/////////////////////////////////////////////////////////////////////////
char* String2Char(String Chuoi)
{
  char* ChuoiTam = new char[Chuoi.length() + 1];
  strcpy(ChuoiTam, Chuoi.c_str());
  return ChuoiTam;
}
//////////////////////////////////////////////////////////////////////////////////////  
String Int2String(int So)
{
  if(So == 0) { return "0"; }
  int SoLe;
  String Chuoi;
  while(So >0)
  {
    SoLe  = So%10;
    Chuoi = Int2Char(SoLe) + Chuoi;
    So    = So/10;
  }
  return Chuoi;
}
//////////////////////////////////////////////////////////////////////////////////////  
char Int2Char(int _So)
{
    return (char)(_So + 48);;
}

//////////////////////////////////////////////////////////////////////////////////////  
