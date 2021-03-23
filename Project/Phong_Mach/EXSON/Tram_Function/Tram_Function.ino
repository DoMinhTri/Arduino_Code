
#define ViTriTram 3


/////////////////////////////////////////////////////////////////////////////////////////
void KiemTraTinHieuNhan()
{
  char TangPhat   = MangLoad[3];
  char PhongPhat  = MangLoad[4];
  char TangNhan   = MangLoad[5];
  char PhongNhan  = MangLoad[6];
  char TramTruyen = MangLoad[7];
  //Serial.print(TangPhat); Serial.print("-"); Serial.println(TangNhan); 
  ///////////
  if((int)TangPhat > (int)TangNhan)
  {
      if( (int)ViTriTram <= (int)TramTruyen )
      {
        CapNhatChuoiGoi();   Serial.println("Down"); 
      }
  }///////////
  if((int)TangPhat < (int)TangNhan)
  {
      if( (int)ViTriTram <= (int)TramTruyen )
      {
        CapNhatChuoiGoi(); Serial.println("Up"); 
      }
  }
  ///////////
}

/////////////////////////////////////////////////////////////////////////////////////////
void CapNhatChuoiGoi()
{
  char TangPhat   = MangLoad[3];
  char PhongPhat  = MangLoad[4];
  char TangNhan   = MangLoad[5];
  char PhongNhan  = MangLoad[6];
  char TramTruyen = MangLoad[7];
  
  MangTam[3] = TangPhat  ;
  MangTam[4] = PhongPhat ;
  MangTam[5] = TangNhan  ;
  MangTam[6] = PhongNhan ;
  MangTam[7] = ViTriTram ;
  SendGiaTri();
}
