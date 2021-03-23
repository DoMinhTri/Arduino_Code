

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
/// -- LCD 5110 -- //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//      http://www.arduino-projekte.de/index.php?n=23
//      SCK  - PIN_SCLK  - Pin 2
//      MOSI - PIN_SDIN  - Pin 8
//      DC   - PIN_DC    - Pin 7
//      RST  - PIN_RESET - Pin 3
//      CS   - PIN_SCE   - Pin 6
/////////////////////////////////////////////////////////////////////////////
#include <LCD5110_Graph.h>
LCD5110 myGLCD(2,8,7,3,6);
extern uint8_t SmallFont[];
extern unsigned char TinyFont[];
extern uint8_t MediumNumbers[];
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////// Key board   /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
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
byte rowPins[ROWS] = { 12, 10, A0, 9}; 
byte colPins[COLS] = { A3, A2, A1, 11}; 
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



String mmm ;  
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(9600);
  //Serial.println("AAAA");
  ////////////////////////////
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  pinMode(13, OUTPUT); 
  digitalWrite(13, HIGH); 
  ////////////////////////////
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
    ////////////////////////  
    if (KyTu)
    {
      if(KyTu == 'b') MainMenu();
      if(KyTu == 'c') ChuoiLuu = "";
      Serial.println(KyTu);
      //////////////////     
       if(ChinhSua == false)
       {
         if (KyTu != NO_KEY && (KyTu=='1'||KyTu=='2'||KyTu=='3'||KyTu=='4')) MenuChon = KyTu;
       }
       //////////////////
       switch (MenuChon) 
       {
      
           case '1': // 1.ID Import
                  myGLCD.clrScr();
                  myGLCD.print("1.ID Import", LEFT, 8);
                  myGLCD.update();
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
                  eeprom_read_string(8, buf, BUFSIZE);
                  mmm = buf;
                  myGLCD.clrScr();
                  myGLCD.print("3.Export", LEFT, 8);
                  myGLCD.print(mmm, LEFT, 16);
                  myGLCD.update();
                  Serial.println(mmm);
                  ////////////////////////
                  if(KyTu == '3') ChinhSua = true;    
          break;
          /////////////////////////////////                       
          case '4': // 4.Setting
                  myGLCD.clrScr();
                  myGLCD.print("4.Clear", LEFT, 8);
                  myGLCD.print("7.Yes 5.No", CENTER, 24);
                  myGLCD.update();
                  if(KyTu == '7') XoaAllEEPROM();
                  if(KyTu == '5') MainMenu();
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
   
   myGLCD.clrScr();
   myGLCD.print("1.ID Import", LEFT, 8);
   myGLCD.print("2.View", LEFT, 16);
   myGLCD.print("3.Export", LEFT, 24);
   myGLCD.print("4.Clear", LEFT, 32);
   myGLCD.update();
}
//////////////////////////////////////////////////////////////////////////////////////  
void NhapSo(char key)
{
    if (key != NO_KEY && (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'))
    {
      ChuoiLuu  = ChuoiLuu + key;
      myGLCD.print(ChuoiLuu, RIGHT, 24);
      myGLCD.update();
    }
    
    int DoDai = ChuoiLuu.length();
    //Serial.println(ChuoiLuu);
    if(key == 'a') 
    {
        //LuuEEPROM(ViTriLuu, "_" + ChuoiLuu );
        ChuoiLuu = "";
        SoLuong++;
        ViTriLuu = ViTriLuu + DoDai + 1;
        //LuuEEPROM(0, Int2String(ViTriLuu) + "_" + Int2String(SoLuong));
    }
}
//////////////////////////////////////////////////////////////////////////////////////  
void XemTongID(char KyTuClick)
{
    eeprom_read_string(8, buf, BUFSIZE);
    String ChuoiLoad = buf , tmp , Mang1 = "", Mang2 = "";
    int SoLanThay = 0;
    boolean BatDauAdd = false ;
    ////////////////////////
    if(SoLuong >= 3)
    {
      if(KyTuClick == 'u')         ViTriXem = ViTriXem - 2;
      if(KyTuClick == 'd')         ViTriXem = ViTriXem + 2;
      if( ViTriXem  < 1  )         ViTriXem = 1 ;
      if( ViTriXem  > (SoLuong-2)) ViTriXem = SoLuong - 2 ;
    }
    ////////////////////////
    for(int i=0; i < ChuoiLoad.length(); i++)
    {
        String kk = ChuoiLoad.substring(i, i+1);
        if(kk == "_")  { SoLanThay++; kk = ""; BatDauAdd = true; }
        if(BatDauAdd) 
        {
          if(SoLanThay == ViTriXem + 0) {Mang1 = Mang1 + kk; }
          if(SoLanThay == ViTriXem + 1) {Mang2 = Mang2 + kk; }
          if(SoLanThay  > ViTriXem + 1) {break; }
        }
    }
    
    myGLCD.clrScr();
    myGLCD.print(Mang1, LEFT, 8);
    myGLCD.print(Mang2, LEFT, 16);
    myGLCD.update();
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
 
  myGLCD.clrScr();
  myGLCD.print("Clear ...", LEFT, 8);
  myGLCD.update();

  delay(1000);
  for (int i = 0; i < 512; i++) { EEPROM.write(i, 0); }
  
  myGLCD.print("Complete", CENTER, 24);
  myGLCD.update();
  delay(2000);
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
char* String2Char(String CString)
{
  char *cstr = new char[CString.length() + 1];
  strcpy(cstr, CString.c_str());
  return cstr;
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
