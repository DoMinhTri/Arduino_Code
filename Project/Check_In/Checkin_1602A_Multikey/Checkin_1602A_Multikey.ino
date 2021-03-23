/// -- LCD 1602A -- /////////////////////////////////////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(3, 6, 7, 8, 9, 10);


#include <Wire.h>    

int pin=0;
void i2cexeepromwrite(int slaveaddress,int whichaddress,int data)
{
  Wire.beginTransmission(slaveaddress);
  Wire.send(whichaddress>>8);
  Wire.send(whichaddress & 0xFF);
  Wire.send(data);
  Wire.endTransmission();
  delay(100);
}
byte i2cexeepromread(int slaveaddress,int whichaddress)
{
  int res;
  Wire.beginTransmission(slaveaddress);
  Wire.send((byte)whichaddress>>8);
  Wire.send((byte)whichaddress & 0xFF);
  Wire.endTransmission();
  Wire.requestFrom(slaveaddress,1);
  if(Wire.available())
  res=Wire.receive();
  return res;  
}



void setup() {
    Serial.begin(9600);
    lcd.begin(1, 2);
    //lcd.print(readEEPROM(disk1, address));
    Wire.begin();
    ///////////////////////////

    
    
}


void loop() {
  
int val,a;
float temp;
val=analogRead(pin);
temp=(5*val*100)/1024;
Serial.println(temp);
for(a=0;a<10;a++)
{
  Serial.println("Writing now");
  i2cexeepromwrite(0x50,a,temp);
  Serial.println("Now Read");
  i2cexeepromread(0x50,a);
}


}




