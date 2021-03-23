
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
  myServo1.attach(25); // C_1A - 25
  myServo2.attach(26); // C_1B - 26
  myServo3.attach(16); // C_2A - 16
  myServo4.attach(17); // C_2B - 17
  myServo5.attach(14); // C_3A - 14
  myServo6.attach(27); // C_3B - 27
  myServo7.attach(13); // C_4A - 13
  myServo8.attach(12); // C_4B - 12
  
  //C_1B.attach(26); // chân left trên trước  - D2
  //C_1A.attach(25); // chân left trên sau    - D3
  //C_2B.attach(17); // chân left dưới trước  - D4
  //C_2A.attach(16); // chân left dưới sau    - D5
  //C_3B.attach(27); // chân right dưới trước - D6
  //C_3A.attach(14); // chân right dưới sau   - D7
  //C_4B.attach(12); // chân right trên trước - D8
  //C_4A.attach(13); // chân right trên sau   - D9
  ///////////////////////////////
  while (WiFi.status() != WL_CONNECTED){ delay(500); Serial.print(".");  }
  Serial.println(WiFi.localIP());
  OTA_Function();
  server.on("/", PageIndex);  // Get_API.h
  server.begin();
  ///////////////////////////////
//  Nhen_Dung();
  center_servos();
  delay(2000);
}
/////////////////////////////////////////////////////////////
void loop() 
{
  server.handleClient();
  //////////////
  KieuHoatDong = 1;
  //center_servos();  // Center all servos
  high = 15;        // Set hight to 15
  spd  = 3;         // Set speed to 3
  while (1 == 1)    // Loop forever
  {
      switch (KieuHoatDong)
      {
        case 1:
          forward();
          break;

        case 2:
          back();
          break;

        case 3:
          turn_right();
          break;

        case 4:
          turn_left();
          break;

        case 5:
          bow();
          break;

        case 6:
          wave();
          break;

        case 7:
          increase_speed();
          break;

        case 8:
          decrease_speed();
          break;

        case 9:
          dance();
          break;

        case 10:
          center_servos();
          break;

        case 11:
          trim_left();
          break;

        case 12:
          trim_right();
          break;

        default:
          break;
      }
      delay(50);  // Pause for 50ms before executing next movement


  }//while

}
/////////////////////////////////////////////////////////////
