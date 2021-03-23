#include <EEPROM.h>
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void LuuEEPROM(uint8_t nDiaChi, uint8_t nGiaTri){ EEPROM.write(nDiaChi, nGiaTri); EEPROM.commit(); delay(20); }
//////////////////////////////////////////////////////////////////////////////////
uint8_t LoadEEPROM_(uint8_t DiaChi){ return EEPROM.read(DiaChi); }
//////////////////////////////////////////////////////////////////////////////////
void EEPROM_WS(uint8_t DiaChi, String ChuoiLuu)
{
  char* ChuoiTam = String2Char( ChuoiLuu );
  uint8_t DoDai  = ChuoiLuu.length();
  for (uint8_t i = 0; i < DoDai ; i++){ EEPROM.write(DiaChi + i,ChuoiTam[i]); }
  EEPROM.write(DiaChi + DoDai,255);
  delete ChuoiTam;
  EEPROM.commit();
}
//////////////////////////////////////////////////////////////////////////////////
String EEPROM_RS(uint8_t BatDau, uint8_t DoDai)
{
    String ChuoiDoc = "";
    for(uint8_t i = BatDau; i< (BatDau + DoDai); i++) 
    { 
      char tmpChar = char(EEPROM.read(i));
      if((tmpChar > 31) && (tmpChar < 127)){ ChuoiDoc+= String(tmpChar); } else { break; }
    }
    return ChuoiDoc;
}
////////////////////////////////////////////////////////////////////////////////// Name - 30
void    Wifi_LuuName(String _WName){ EEPROM_WS(21, _WName); }
String  Wifi_LoadName(){ return EEPROM_RS(21, 30); }
////////////////////////////////////////////////////////////////////////////////// Pass - 20
void Wifi_LuuPass(String _WPass){ EEPROM_WS(51, _WPass); }
String  Wifi_LoadPass(){ return EEPROM_RS(51, 20); }
//////////////////////////////////////////////////////////////////////////////////
