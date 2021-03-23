

#include <EEPROM.h>
/////////////////////////////////////////////////////////////////////////////   01675.183210 - le truc - 200m - tay phai - uon toc lanh - rao Hoa Duc
const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;

boolean eeprom_is_addr_ok(int addr) 
{
    return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}
/////////////////////////////////////////////////////////////////////////////
boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes) 
{
  int i;
  if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) return false;

  for (i = 0; i < numBytes; i++) 
  {
      EEPROM.write(startAddr + i, array[i]);
  }
  return true;
}
/////////////////////////////////////////////////////////////////////////////
boolean eeprom_write_string(int addr, const char* string) 
{
  int numBytes; 
  numBytes = strlen(string) + 1;
  return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}
/////////////////////////////////////////////////////////////////////////////
boolean eeprom_read_string(int addr, char* buffer, int bufSize) 
{
  byte ch; 
  int bytesRead; 

  if (!eeprom_is_addr_ok(addr)) { return false; }
  if (bufSize == 0) { return false; }
  if (bufSize == 1) { buffer[0] = 0; return true; }

  bytesRead = 0; 
  ch = EEPROM.read(addr + bytesRead); 
  buffer[bytesRead] = ch; 
  bytesRead++; 

  while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) ) 
  {
      ch = EEPROM.read(addr + bytesRead);
      buffer[bytesRead] = ch;
      bytesRead++; 
   }

  if ((ch != 0x00) && (bytesRead >= 1)) { buffer[bytesRead - 1] = 0; }
  return true;
}
/////////////////////////////////////////////////////////////////////////////
const int BUFSIZE = 503;
char      buf[BUFSIZE];
char      myStringChar[BUFSIZE];

char MangChar[BUFSIZE];
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include <LiquidCrystal.h> 
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
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

byte rowPins[ROWS] = { 3, 12, 11, 10}; 
byte colPins[COLS] = {A3, A2, A1, A0}; 

Keypad BanPhimChu = Keypad( makeKeymap(MangPhimChu), rowPins, colPins, ROWS, COLS); 
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

int ViTriXem    ;
char KyTu       ;
char MenuChon   ;
String ChuoiLuu ;
int ViTriLuu    ; // Vị trí tiếp tục lưu
int SoLuong     ; // Số lượng đã lưu, mỗi ID cách nhau dấu ","
boolean ChinhSua = false;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  ////////////////////////////
  lcd.begin(0, 2);
  LoadViTriLuu_SoLuong();
  MainMenu();  
  ViTriXem = 1;
}
//////////////////////////////////////////////////////////////////////////////////////  
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    
    KyTu = BanPhimChu.getKey();
    lcd.setCursor(0, 1);
    ////////////////////////  
    if (KyTu)
    {
      if(KyTu == 'b') MainMenu();
      if(KyTu == 'c') ChuoiLuu = "";
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
                  XemTongID(KyTu);
                  ////////////////////////
                  if(KyTu == '2') ChinhSua = true;    
          break;
          /////////////////////////////////                       
          case '3': // 3.Export
                  lcd.clear();
                  lcd.begin(0, 2);
                  //eeprom_read_string(0, buf, BUFSIZE);
                  lcd.print(ViTriXem);
                  lcd.print("_");
                  lcd.print(SoLuong);
                  
                  lcd.setCursor(0, 1);
                  eeprom_read_string(8, buf, BUFSIZE);
                  lcd.print(buf);
                  ////////////////////////
                  if(KyTu == '3') ChinhSua = true;    
          break;
          /////////////////////////////////                       
          case '4': // 4.Setting
                  lcd.clear();
                  lcd.begin(0, 2);
                  lcd.print("4.Clear");
                  lcd.setCursor(0, 1);
                  lcd.print("  1.Yes   2.No");
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
   
   lcd.clear();
   lcd.begin(0, 2);
   lcd.print("1.ID+   3.Export");
   lcd.setCursor(0, 1);
   lcd.print("2.View  4.Clear");
}
//////////////////////////////////////////////////////////////////////////////////////  
void NhapSo(char key)
{
    if (key != NO_KEY && (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'))
    {
      ChuoiLuu  = ChuoiLuu + key;
    }
    
    int DoDai = ChuoiLuu.length();
    lcd.setCursor(15 - DoDai, 1);
    lcd.print(ChuoiLuu);
    
    if(key == 'a') 
    {
        LuuEEPROM(ViTriLuu, "_" + ChuoiLuu );
        ChuoiLuu = "";
        SoLuong++;
        ViTriLuu = ViTriLuu + DoDai + 1;
        
        LuuEEPROM(0, Int2String(ViTriLuu) + "_" + Int2String(SoLuong));
        XoaDong_2();
    }
}
//////////////////////////////////////////////////////////////////////////////////////  
void XoaDong_1()
{
     lcd.setCursor(0, 2);
     lcd.print("                ");
}
//////////////////////////////////////////////////////////////////////////////////////  
void XoaDong_2()
{
     lcd.setCursor(0, 1);
     lcd.print("                ");
}
//////////////////////////////////////////////////////////////////////////////////////  
void XemTongID(char KyTuClick)
{
    eeprom_read_string(8, buf, BUFSIZE);
    String ChuoiLoad   = buf ;
    int Dau_1 = 0, Dau_2 = 0, Dau_3 = 0, SoLanThay = 0;
    String MangShow[2] = {" "," "};
    ////////////////////////
    if(SoLuong == 1) 
    {
      MangShow[0] = ChuoiLoad.substring(1, ChuoiLoad.length());
      goto EndView;
    }
    ////////////////////////
    if(SoLuong >= 2)
    {
      if(KyTuClick == 'u') { if( ViTriXem > 1 )          { ViTriXem--; }}
      if(KyTuClick == 'd') { if( ViTriXem < (SoLuong-1)) { ViTriXem++; }}
    }
    ////////////////////////
    for(int i=0; i < ChuoiLoad.length(); i++)
    {
        String kk = ChuoiLoad.substring(i, i+1);
        if(kk == "_") 
        {
          SoLanThay++;
          if(SoLanThay == ViTriXem + 0) { Dau_1 = i + 1; }
          if(SoLanThay == ViTriXem + 1) { Dau_2 = i + 1; }
          if(SoLanThay == ViTriXem + 2) 
          { 
              Dau_3 = i + 1; 
              if(ViTriXem == (SoLuong - 1)) Dau_3 = ChuoiLoad.length() + 1;
              break ;
          }
        }
    }
    MangShow[0] = ChuoiLoad.substring(Dau_1, Dau_2-1);// + ":[" + Int2String(Dau_1)+ "]-[" + Int2String(Dau_2)+ "]-[" + Int2String(Dau_3) + "]";
    MangShow[1] = ChuoiLoad.substring(Dau_2, Dau_3-1);// + ":[" + Int2String(SoLanThay) + "]-[" + Int2String(ViTriXem);
EndView:    
    lcd.clear();
    lcd.begin(0, 2);
    lcd.print(MangShow[0]);
    lcd.setCursor(0, 1);
    lcd.print(MangShow[1]);
    ////////////////////////
}
//////////////////////////////////////////////////////////////////////////////////////  
void LuuEEPROM(int addLuu, String GiaTriLuu)
{
    GiaTriLuu.toCharArray(myStringChar, BUFSIZE); 
    strcpy(buf, myStringChar);
    eeprom_write_string(addLuu, buf); 
}
//////////////////////////////////////////////////////////////////////////////////////  
void XoaAllEEPROM()
{
  ViTriLuu = 8;
  SoLuong  = 0;
  ChuoiLuu = "";
  ViTriXem = 1;
 
  lcd.clear();
  lcd.begin(0, 1);
  lcd.print(" Clearning ...");
  delay(500);
  for (int i = 0; i < 512; i++) { EEPROM.write(i, 0); }
  lcd.setCursor(1, 1); 
  lcd.print("Clear complete");
  delay(1000);
  MainMenu();
}
//////////////////////////////////////////////////////////////////////////////////////  
// Địa chỉ 0, nơi lưu vị trí EEPROM đến địa chỉ thứ N
void LoadViTriLuu_SoLuong()
{
    eeprom_read_string(0, buf, BUFSIZE);
    String ChuoiLoad = buf;
    int DauGach = 0;
    for(int i=0; i<8; i++)
    {
        if(ChuoiLoad.substring(i,i+1) == "_") DauGach = i;
    }
    ViTriLuu = (ChuoiLoad.substring(0, DauGach)).toInt();
    SoLuong  = (ChuoiLoad.substring(DauGach+1, 7)).toInt();
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
char Int2Char(int So)
{
    char Chu;
    switch (So)
    {
      case 0: Chu='0'; break;
      case 1: Chu='1'; break;
      case 2: Chu='2'; break;
      case 3: Chu='3'; break;
      case 4: Chu='4'; break;
      case 5: Chu='5'; break;
      case 6: Chu='6'; break;
      case 7: Chu='7'; break;
      case 8: Chu='8'; break;
      case 9: Chu='9'; break;  
    }
    return Chu;
}
//////////////////////////////////////////////////////////////////////////////////////  
