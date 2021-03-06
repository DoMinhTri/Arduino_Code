#include <AH_24Cxx.h>
#include <Wire.h> 

#define AT24C32  5
//Initialisation
#define BUSADDRESS  0x00      
#define EEPROMSIZE  2048     //AT24C32 2048byte
AH_24Cxx ic_eeprom = AH_24Cxx(AT24C32, BUSADDRESS);
/////////////////////////////////////////////////////////////////////////
void EEPROM_WS(int DiaChi, String ChuoiLuu)
{
  char* ChuoiTam = String2Char( ChuoiLuu );
  int DoDai =  ChuoiLuu.length();
  for (int i = 0; i < DoDai ; i++)
  {
      EEPROM_WC( DiaChi + i, ChuoiTam[i] );
      Serial.print(DiaChi + i);
      Serial.print(" : ");
      Serial.println(ChuoiTam[i]);
  }
   Serial.println("-------------------");
   delete ChuoiTam;
}
/////////////////////////////////////////////////////////////////////////
void EEPROM_WC(int DiaChi, char KyTu){  ic_eeprom.write_byte( DiaChi,  KyTu );  delay(20); }
/////////////////////////////////////////////////////////////////////////
void EEPROM_WN(int DiaChi, int SoLuu){  ic_eeprom.write_byte( DiaChi, SoLuu );  delay(20); }
/////////////////////////////////////////////////////////////////////////
String EEPROM_RS(int BatDau, int KetThuc)
{
    String ChuoiDoc = "";
    for(int i = BatDau; i< (BatDau + KetThuc); i++) { ChuoiDoc = ChuoiDoc + EEPROM_RC(i); }
    return ChuoiDoc;
}
/////////////////////////////////////////////////////////////////////////
char EEPROM_RC(int DiaChi){ return (char)ic_eeprom.read_byte(DiaChi); }
/////////////////////////////////////////////////////////////////////////
int EEPROM_RN(int DiaChi){ return ic_eeprom.read_byte(DiaChi); }
/////////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////////////   01675.183210 - le truc - 200m - tay phai - uon toc lanh - rao Hoa Duc
#include <LiquidCrystal.h> 
LiquidCrystal lcd(9, 8, 6, 5, 4, 3);
////////////////////////////////////////////////////////////////////////////////////// Key board 
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

// u(Up)  - d(Down) - e(Delete) - c(Clear) - a(Add) - b(Back)
char MangPhimChu[ROWS][COLS] = {
  {'u','3','2','1'},
  {'d','6','5','4'},
  {'e','9','8','7'},
  {'c','a','0','b'}
};

byte rowPins[ROWS] = {12, A3, 7, 2}; 
byte colPins[COLS] = {A0, A1, A2, 11};

Keypad BanPhimChu = Keypad( makeKeymap(MangPhimChu), rowPins, colPins, ROWS, COLS); 
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

int ViTriXem     ;
char KyTu        ;
char MenuChon    ;
int DoDaiID  = 7 ;
int ViTriDau = 4 ;
String ChuoiLuu  ;
int ViTriLuu     ; // V??? tr?? ti???p t???c l??u
int SoLuongID    ; // S??? l?????ng ID
boolean ChinhSua = false;

int DenManHinh = 13;
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  pinMode(DenManHinh, OUTPUT);
  digitalWrite(DenManHinh, HIGH);
  ////////////////////////////  
  Wire.begin();
  ////////////////////////////
  lcd.begin(0, 2);
  MainMenu();  
  LoadSoLuongID();
  ViTriXem = 1;
}
//////////////////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    ////////////////////////    
    KyTu = BanPhimChu.getKey();

    ////////////////////////  
    if (KyTu)
    {
      if(KyTu == 'b') MainMenu();
      if(KyTu == '5') KK();

      //////////////////     
       if(ChinhSua == false)
       {
         if (KyTu != NO_KEY && (KyTu=='1'||KyTu=='2'||KyTu=='3'||KyTu=='4')) MenuChon = KyTu;
       }
       //////////////////
       switch (MenuChon) 
       {
          case '1': // 1.ID + 
                  lcd.begin(0, 2);
                  lcd.print("1.ID+");
                  if(ChinhSua) NhapSo(KyTu) ;
                  ////////////////////////
                  if(KyTu == '1') ChinhSua = true; 
            break;
          /////////////////////////////////      
          case '2': // 2.View 
                  if(ChinhSua == false)
                  {
                    LCD2Dong("Up/Down", "Light:1=On 0=Off");
                    delay(1000);
                  }
                  
                  if(KyTu == '1') digitalWrite(DenManHinh, HIGH);
                  if(KyTu == '0') digitalWrite(DenManHinh, LOW);
                    
                  XemTongID(KyTu);
                  ////////////////////////
                  if(KyTu == '2') ChinhSua = true;    
          break;
          /////////////////////////////////                       
          case '3': // 3.Export
                  lcd.clear();
                  lcd.begin(0, 2);
                  lcd.print("Total: ");
                  lcd.print(SoLuongID);
                  if(SoLuongID > 0)
                  {
                    lcd.setCursor(0, 1);
                    lcd.print("  1.Yes   2.No");
                  }
                  if(KyTu == '1') XuatGiaTriLuu();
                  if(KyTu == '2') MainMenu();
                  ////////////////////////
                  if(KyTu == '3') ChinhSua = true;    
          break;
          /////////////////////////////////                       
          case '4': // 4.Setting
                  LCD2Dong("4.Clear", "  1.Yes   2.No");
                  if(KyTu == '1') XoaAllEEPROM();
                  if(KyTu == '2') MainMenu();
                  ////////////////////////
                  if(KyTu == '4') ChinhSua = true;                  
          break;
          /////////////////////////////////                
          case 'b': // 
                  MainMenu();         
          break;
        }
       //////////////////
    }
    ////////////////////////  
}
//////////////////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////////////////  
void MainMenu()
{
   ChuoiLuu = ""; 
   MenuChon = 'b';
   ChinhSua = false;
   
   LCD2Dong("1.ID+   3.Export", "2.View  4.Clear");
}
//////////////////////////////////////////////////////////////////////////////////////  
void NhapSo(char key)
{
    if (key != NO_KEY && (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'||key=='e'||key=='c'))
    {
      if(key == 'e') 
      { 
        ChuoiLuu = ChuoiLuu.substring(0, ChuoiLuu.length() -1 );  
      }
      else if (key != 'c')
          { ChuoiLuu  = ChuoiLuu + key; }
      else
          { ChuoiLuu = "" ;}
    }
    
    int DoDai = ChuoiLuu.length();
    lcd.setCursor(16 - DoDai, 1);
    lcd.print(ChuoiLuu);
    
    if(key == 'a') 
    {
        boolean Loi = false;
        String Msg  = "ID7-Err";
        if(ChuoiLuu.length() != 7) Loi = true;
        if(Loi == false) { Loi = KiemTraTrung(ChuoiLuu); if(Loi) { Msg = "IDx2-Err"; } }
        if(Loi)
        {
          lcd.setCursor(0, 2);
          lcd.print(Msg);
        }
        else
        {
            EEPROM_WS(ViTriLuu, ChuoiLuu );
            ChuoiLuu = "";
            SoLuongID++;
            ViTriLuu = ViTriLuu + DoDai ;

            LuuSoLuongID();
            XoaDong_2();
            lcd.setCursor(0, 2);
            lcd.print("-> Ok");
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////  
void XoaDong_1(){ lcd.setCursor(0, 2); lcd.print("                "); }
//////////////////////////////////////////////////////////////////////////////////////  
void XoaDong_2(){ lcd.setCursor(0, 1); lcd.print("                "); }
//////////////////////////////////////////////////////////////////////////////////////  
void XemTongID(char KyTuClick)
{
    String Mang1 = "", Mang2 = "";
    ////////////////////////
    if(SoLuongID >= 3)
    {
      if(KyTuClick == 'u')           ViTriXem = ViTriXem - 1;
      if(KyTuClick == 'd')           ViTriXem = ViTriXem + 1;
      if( ViTriXem  < 1  )           ViTriXem = 1 ;
      if( ViTriXem  > (SoLuongID-1)) ViTriXem = SoLuongID - 1 ;
    }
    ////////////////////////
    for(int i=1; i<= SoLuongID; i++)
    { 
      if(i == ViTriXem    ) { Mang1 = LoadID(i); }
      if(i == ViTriXem + 1) { Mang2 = LoadID(i); break ;}
    }
    
    if(Mang1 != "") { Mang1 = Mang1 + "-" + Int2String(ViTriXem)  ; }
    if(Mang2 != "") { Mang2 = Mang2 + "-" + Int2String(ViTriXem+1); }
    Mang1 = ThemKhoangCach(Mang1,"ID");
    Mang2 = ThemKhoangCach(Mang2, Int2String(SoLuongID));
    LCD2Dong(Mang1,Mang2);
    ////////////////////////
}
//////////////////////////////////////////////////////////////////////////////////////  
String ThemKhoangCach(String Chuoi1, String Chuoi2)
{
  int nDoDai      = Chuoi1.length() + Chuoi2.length();
  String tmpChuoi = "";
  for(int i=1; i <= 16 - nDoDai; i++)
  {
    tmpChuoi = tmpChuoi + " ";
  }
  return (Chuoi1 + tmpChuoi + Chuoi2);
}
//////////////////////////////////////////////////////////////////////////////////////  
void XuatGiaTriLuu()
{
    String TongChuoi = "";
    for(int i=1; i<= SoLuongID; i++)
    { 
      TongChuoi = TongChuoi + LoadID(i) + ",";
    }
    Serial.println(TongChuoi);
}
//////////////////////////////////////////////////////////////////////////////////////  
boolean KiemTraTrung(String ChuoiSS)
{
    for(int i = 1; i<= SoLuongID ; i++)
    {
       if(ChuoiSS == LoadID(i)) return true; 
    }
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////  
void XoaAllEEPROM()
{
  lcd.clear();
  lcd.begin(0, 2);
  lcd.print(" Clearning  ...");
  delay(500);
  for (int i = 0; i < ViTriLuu; i++) { EEPROM_WN(i, 0); }
  lcd.setCursor(0, 1); 
  lcd.print(" Clear complete");
  delay(1000);
  /////////////
  ViTriLuu  = ViTriDau;
  SoLuongID = 0;
  ChuoiLuu  = "";
  ViTriXem  = 1;
  LuuSoLuongID();
  /////////////  
  MainMenu();
}
//////////////////////////////////////////////////////////////////////////////////////  
String LoadID(int nThuTu)
{
  int ntpSo = ViTriDau + (nThuTu -1)*DoDaiID ;
  return EEPROM_RS(ntpSo, DoDaiID);
}
//////////////////////////////////////////////////////////////////////////////////////  
// ?????a ch??? 0, n??i l??u v??? tr?? EEPROM ?????n ?????a ch??? th??? N
void LoadSoLuongID()
{
    SoLuongID = (EEPROM_RS(0,4)).toInt();
    ViTriLuu  = ViTriDau + SoLuongID*DoDaiID;
}
void LCD2Dong(String Dong_1, String Dong_2)
{
   lcd.clear();
   lcd.begin(0, 2);
   lcd.print(Dong_1);
   lcd.setCursor(0, 1);
   lcd.print(Dong_2);
}
/////////////////////////////////////////////////////////////////////////
void LuuSoLuongID()
{
  String tmSL = ThemSoKhong(Int2String(SoLuongID)) ;
  EEPROM_WS(0,tmSL);
}
/////////////////////////////////////////////////////////////////////////
String ThemSoKhong(String Chuoi)
{
  for(int i= Chuoi.length(); i< 4; i++) { Chuoi = "0" + Chuoi; }
  return Chuoi;
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
    return return (char)(_So + 48);;
}

//////////////////////////////////////////////////////////////////////////////////////  
void KK()
{

}


