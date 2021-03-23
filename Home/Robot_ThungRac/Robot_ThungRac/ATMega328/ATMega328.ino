#include<Servo.h>
Servo SERVO; 

#include<SoftwareSerial.h>
SoftwareSerial HC05(5, 3); 

#define SerVoPin      9

#define Kenh_A        13
#define Chieu_Kenh_A  12

#define Kenh_B        11
#define Chieu_Kenh_B  10
//////////////////////////////////////////////////////////
void BanhPhaiNgung()
{ 
    digitalWrite(Chieu_Kenh_A, LOW); 
    digitalWrite(Kenh_A, LOW); 
}
void BanhPhaiToi()
{
    digitalWrite(Kenh_A, HIGH);
    digitalWrite(Chieu_Kenh_A, LOW);
}
void BanhPhaiLui()
{
    digitalWrite(Kenh_A, LOW);
    digitalWrite(Chieu_Kenh_A, HIGH);
}
//////////////////////////////////////////////////////////
void BanhTraiNgung()
{ 
    digitalWrite(Chieu_Kenh_B, LOW); 
    digitalWrite(Kenh_B, LOW); 
}
void BanhTraiToi()
{
    digitalWrite(Kenh_B, HIGH);
    digitalWrite(Chieu_Kenh_B, LOW);
}
void BanhTraiLui()
{
    digitalWrite(Kenh_B, LOW);
    digitalWrite(Chieu_Kenh_B, HIGH);
}
//////////////////////////////////////////////////////////
void XeNgung()
{
    BanhTraiNgung();
    BanhPhaiNgung();
}
void XeToi()
{
    BanhPhaiToi();
    BanhTraiToi(); 
}
void XeLui()
{
    BanhPhaiLui();
    BanhTraiLui();
}
void XeQuayTrai()
{
    BanhPhaiToi();
    BanhTraiLui();
}
void XeQuayPhai()
{
    BanhPhaiLui();
    BanhTraiToi();
}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void ThungMo(){   SERVO.write(180);}
void ThungDong(){ SERVO.write(0);  }
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
void setup()
{
  
  HC05.begin(9600);
  Serial.begin(9600);
  SERVO.attach(SerVoPin);

  pinMode(Kenh_A, OUTPUT);
  pinMode(Kenh_B, OUTPUT);
  pinMode(Chieu_Kenh_A, OUTPUT);
  pinMode(Chieu_Kenh_B, OUTPUT);
  XeNgung();
  ///////////////////////////
  ThungMo();
  delay(3000);
  ThungDong();
  XeNgung();
}

String Chuoi = "";
void loop()
{
  
  while (Serial.available() > 0 ) 
  {
      char KyTu = Serial.read(); 
      Chuoi = KyTu; delay(5);   

      if(Chuoi != "")
      {
        Serial.print(Chuoi);
        if(Chuoi == "a")      {   BanhPhaiToi();   }
        else if(Chuoi == "s") {   BanhPhaiLui();   }
        else if(Chuoi == "d") {   BanhPhaiNgung(); }
        
        else if(Chuoi == "f") {   BanhTraiToi();   }
        else if(Chuoi == "g") {   BanhTraiLui();   }
        else if(Chuoi == "h") {   BanhTraiNgung(); }
      
        else if(Chuoi == "z") {   ThungMo();       }
        else if(Chuoi == "x") {   ThungDong();     } 
        Chuoi = ""; 
      }
  }
  ////////////////
 

  ////////////////
  while (HC05.available() > 0 ) 
  {
      char KyTu = HC05.read();  
      if(KyTu == 'U')  
      {
        XeToi();
      }
      else if(KyTu == 'L')  
      {
         XeLui();
      }
      else if(KyTu == 'T')  
      {
         XeQuayTrai();
      }
      else if(KyTu == 'P')  
      {
         XeQuayPhai();
      }
      else if(KyTu == 'N')  
      {
         XeNgung();
      }
      else if(KyTu == 'M')  
      {
         ThungMo();
      }
      else if(KyTu == 'D')  
      {
         ThungDong();
      }
      else
      {
        XeNgung();
      }
      /////////////////
  }
  /////////////////
}
