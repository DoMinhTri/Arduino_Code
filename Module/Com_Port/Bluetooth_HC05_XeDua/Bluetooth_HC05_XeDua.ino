
#include <SoftwareSerial.h>
SoftwareSerial HC05(A1, A2); // RX | TX

#define Pin_1 3
#define Pin_2 4

#define Pin_3 5
#define Pin_4 2


void setup() 
{
  Serial.begin(9600);
  HC05.begin(9600);
  pinMode(Pin_1, OUTPUT);
  pinMode(Pin_2, OUTPUT);
  pinMode(Pin_3, OUTPUT);
  pinMode(Pin_4, OUTPUT);
  
}
 
void loop() {

  while (HC05.available() > 0 ) 
  {
      char KyTu = HC05.read();  

      if(KyTu == 'I'){ BanhTraiToi();  Serial.println("Banh Trai Toi - Goc 1");         }
      if(KyTu == 'J'){ BanhTraiLui();  Serial.println("Banh Trai Lui - Goc 2");         }
      if(KyTu == 'H'){ BanhPhaiLui();  Serial.println("Banh Phai Lui - Goc 3");         }
      if(KyTu == 'G'){ BanhPhaiToi();  Serial.println("Banh Phai Toi - Goc 4");         }
      
      if(KyTu == 'S'){ XeNgung();      Serial.println("STOP");          }
      
      if(KyTu == 'L'){ XeQuayTrai();   Serial.println("Xe Quay Trai");  }
      if(KyTu == 'R'){ XeQuayPhai();   Serial.println("Xe Quay Phai");  }
      if(KyTu == 'F'){ XeToi();        Serial.println("Xe Toi");        }
      if(KyTu == 'B'){ XeLui();        Serial.println("Xe Lui");        }

     //if(KyTu == 'F'){ BanhTraiToi();    }
     //if(KyTu == 'B'){ BanhTraiLui();    }
      Serial.println(KyTu);
  }
  /////////////
  delay(100);
}
///////////////////////
void XeQuayTrai()
{
  BanhTraiLui();
  BanhPhaiToi();
}
///////////////////////
void XeQuayPhai()
{
  BanhPhaiLui();
  BanhTraiToi();
}

///////////////////////
void XeToi()
{
  BanhPhaiToi();
  BanhTraiToi();
}
///////////////////////
void XeLui()
{
  BanhPhaiLui();
  BanhTraiLui();
}
///////////////////////
void XeNgung()
{
  digitalWrite(Pin_1, LOW);
  digitalWrite(Pin_2, LOW);
  digitalWrite(Pin_3, LOW);
  digitalWrite(Pin_4, LOW);
}
///////////////////////
void BanhPhaiToi()
{
  digitalWrite(Pin_3, LOW);
  digitalWrite(Pin_4, HIGH);
}
///////////////////////
void BanhPhaiLui()
{
  digitalWrite(Pin_3, HIGH);
  digitalWrite(Pin_4, LOW);
}
///////////////////////
void BanhTraiToi()
{
  digitalWrite(Pin_1, LOW);
  digitalWrite(Pin_2, HIGH);
}
///////////////////////
void BanhTraiLui()
{
  digitalWrite(Pin_1, HIGH);
  digitalWrite(Pin_2, LOW);
}

