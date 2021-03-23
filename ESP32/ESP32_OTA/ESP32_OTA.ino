
#include "OTA_Update.h"

const char* ssid     = "TDO tang 1_EXT";
const char* password = "tdo@vietnam";
/////////////////////////////////////////////////////////////
void setup(void) 
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");
  ///////////////////////////////
  while (WiFi.status() != WL_CONNECTED){ delay(500); Serial.print(".");  }
  Serial.println(WiFi.localIP());
  OTA_Function();
  server.begin();
}
/////////////////////////////////////////////////////////////
void loop(void) {
  server.handleClient();
  delay(1);
}
/////////////////////////////////////////////////////////////
