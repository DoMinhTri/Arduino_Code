
#include "OTA_Update.h"
#include "Legs_Pin.h"
#include "Functions.h"
#include "Get_API.h"


const char* ssid     = "TDO tang 1_EXT";
const char* password = "tdo@vietnam";
/////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");
  ///////////////////////////////
  //  1B  1A  -  4A 4B
  //  2B  2A  -  3A 3B
  C_1B.attach(26); // chân left trên trước  - D2
  C_1A.attach(25); // chân left trên sau    - D3

  C_2B.attach(17); // chân left dưới trước  - D4
  C_2A.attach(16); // chân left dưới sau    - D5

  C_3B.attach(27); // chân right dưới trước - D6
  C_3A.attach(14); // chân right dưới sau   - D7

  C_4B.attach(12); // chân right trên trước - D8
  C_4A.attach(13); // chân right trên sau   - D9
  ///////////////////////////////
  while (WiFi.status() != WL_CONNECTED){ delay(500); Serial.print(".");  }
  Serial.println(WiFi.localIP());
  OTA_Function();
  server.on("/", PageIndex);  // Get_API.h
  server.begin();
  ///////////////////////////////
  Nhen_Dung();
  delay(2000);
}
/////////////////////////////////////////////////////////////
void loop() 
{
  server.handleClient();
  NhenAction();
  delay(500);
}
/////////////////////////////////////////////////////////////
void NhenAction()
{
   switch (KieuHoatDong) 
   {
      case 0:
            Nhen_Dung();
            break;  
      case 1:
            //Nhen_DiToi();
            break; 
      case 2:
            //Nhen_QuaTrai();
            break; 
      case 3:
            //Nhen_DiToi();
            break;  
   }
}
/////////////////////////////////////////////////////////////
