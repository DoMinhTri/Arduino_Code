////////////////////////////////////////////
#include <LiquidCrystal.h>
LiquidCrystal lcd(A2, 13, 12, 11, 10, 9);


#define Relay        3
#define LCDLed       A0
#define DenTTRelay   A3

#define RF315_1      A1
#define RF315_2      5

String  textSMS     = "Canh Bao";
String  phoneNumber = "+84908221404";
 
boolean HoatDong = true;

void setup()
{
  pinMode(LCDLed    ,OUTPUT);
  pinMode(Relay     ,OUTPUT);
  pinMode(DenTTRelay,OUTPUT);
  
  pinMode(RF315_1 ,INPUT);
  pinMode(RF315_2 ,INPUT);
  
  Serial.begin(9600);
  SIM900power();  
  ////////////////////
  lcd.begin(16, 2);
  digitalWrite(LCDLed, HIGH);
}

void loop()
{
   int TH_1 = digitalRead(RF315_1);
   int TH_2 = digitalRead(RF315_2);
   if (TH_1 == 1){ HoatDong = true ;  }
   if (TH_2 == 1){ HoatDong = false ; }
   if(HoatDong){ digitalWrite(LCDLed, HIGH); } else { digitalWrite(LCDLed, LOW); }

   
   
       //digitalWrite(DenTTRelay, HIGH);
       //sendSMS(textSMS,phoneNumber); 
       //delay(1000);   
       //Serial.println("SMS1");   
    
        
       //digitalWrite(DenTTRelay, HIGH);
       //CallSomeone(phoneNumber); 
       //delay(1000); 
      // Serial.println("SMS2");     

   //////////////
   delay(1000);
}




void Nhayled(){ digitalWrite(DenTTRelay, HIGH); delay(100); digitalWrite(DenTTRelay, LOW); }

void NhayRelay(){  digitalWrite(Relay, HIGH);   delay(200); digitalWrite(Relay, LOW); }
//--------- Hàm Gọi Điện thoại ---------------------
void CallSomeone(String phonenumber)
{
  Serial.print("ATD"); 
  Serial.print(phonenumber);
  Serial.print(";");
  Serial.print("\r\n");
  // wait for 3 seconds...
  delay(10000);            
  Serial.print("ATH\r\n");   // hang up
}


//---------------  Hàm gửi SMS -------------------
void sendSMS(String message,String phonenumber)
{
  // Message format English
  // Message IN TEXTMODE
  Serial.print("AT+CMGF=1\r\n");
  delay(500);
  // syntax SMS :
  // AT+CMGS="0909xxxxxx"<CR>noi dung tin nhan<ctrl+z>
  //  <CR> là '\r'(ma ASCII la 13)     
  //  <ctrl+z> là '^Z' (ma ASCII la 26)
  Serial.print("AT+CMGS=\""); 
  // send phone number to SIM900
  Serial.print(phonenumber);
  Serial.print("\"\r");
  delay(500);
  // At this time, SIM900 will return '>'
  //SIM900_response(500);
  Serial.print(message); // message to send
  // End AT command with a ^Z, ASCII code 26
  Serial.print((char)26); // Ctrl+Z      
  Serial.flush();  // clear buffer 
  // ket thuc qua trinh nhan tin
  delay(5000); 
}

//---------------  Hàm Khởi động SIM  ------------------
void  SIM900power(){
  digitalWrite(DenTTRelay, HIGH);
  delay(1000);
  digitalWrite(DenTTRelay, LOW);
  delay(2000);
  digitalWrite(DenTTRelay, HIGH);
  delay(9000); // waiting for Finding network
  digitalWrite(DenTTRelay, LOW);
}
 

