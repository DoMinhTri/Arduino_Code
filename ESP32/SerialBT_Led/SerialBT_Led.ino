
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


#define Led 2
String TTCongTac = "";

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); 
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(Led, OUTPUT);
}

void loop()
{
  //////////////////////////////////////////////////////////////
  if (Serial.available()){   SerialBT.write(Serial.read());  }
  //////////////////////////////////////////////////////////////
  if (SerialBT.available()) 
  { 
     char KyTu    = SerialBT.read();  
     uint8_t stas = (uint8_t)KyTu;
     Serial.println(KyTu);
  }
  //////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////
  delay(20);
}
