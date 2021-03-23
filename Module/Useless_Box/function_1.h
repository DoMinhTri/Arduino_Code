

////////////////////////////////////////////////////
void BoxDungFast(){  boxServo.write(BoxMax);  }
void BoxDungLow() {  for (int i = 0; i <= BoxMax; i++){ boxServo.write(i); delay(20); } }
////////////////////////////////////////////////////
void BoxNamLow() {  for (int i = BoxMax; i >=0; i--) { boxServo.write(i); delay(20); } }
void BoxNamFast(){  boxServo.write(0); }
////////////////////////////////////////////////////
void HandBamLow() {  for (int i = HandMax; i >= 0; i--){ handServo.write(i); delay(20); } }
void HandBamFast(){  handServo.write(0); }
////////////////////////////////////////////////////
void HandBuongLow() {  for (int i = 26; i <=HandMax; i++){ handServo.write(i); delay(20); } }
void HandBuongFast(){  handServo.write(HandMax); }
///////////////////////////////////////
void DongCongTac(){ delay(1500); HandBamFast();  delay(1000); HandBuongFast(); }
///////////////////////////////////////
///////////////////////////////////////
void HanhDong_1()
{
   Serial.println("Action 1");   
   BoxDungLow();        
   delay(2000);
   HandBamFast();
   delay(2000);
   HandBuongLow();
   delay(1000);
   BoxNamFast();
   
}
///////////////////////////////////////
void HanhDong_2()
{
     Serial.println("Action 2");         
     BoxDungLow();        
     delay(1500);
     handServo.write(26);
     delay(550);
     HandBuongFast();
     delay(550);
     BoxDungFast();
     delay(1500);
     BoxDungLow();   
     delay(2000);
     BoxNamFast();
     ////////////
     DongCongTac();
     
}
///////////////////////////////////////
void HanhDong_3()
{
      Serial.println("Action 3");         
      BoxDungLow();   
      delay(1000);
      BoxNamFast();
      delay(2000);
      BoxDungLow(); 
      handServo.write(26);
      delay(550);
      handServo.write(180);
      delay(500);
      BoxNamFast();
      ////////////
      DongCongTac();
      
}
///////////////////////////////////////
void HanhDong_4()
{
      Serial.println("Action 4");         
      BoxDungLow();   
      HandBamLow();
      delay(2000);
      handServo.write(26);
      delay(200);
      DongCongTac();
      delay(400);
      boxServo.write(0);
}
///////////////////////////////////////
void HanhDong_5()
{
     Serial.println("Action 5");       
      BoxDungLow(); 
      delay(2000);
      handServo.write(65);
      delay(550);
      handServo.write(40);
      delay(200);
      handServo.write(65);
      delay(200);
      handServo.write(40);
      delay(200);
      handServo.write(65);
      delay(200);
      handServo.write(40);
      delay(200);
      handServo.write(65);
      delay(200);
      handServo.write(40);
      delay(200);
      handServo.write(65);
      delay(200);
      handServo.write(40);
      delay(200);
      handServo.write(65);
      delay(2000);
      handServo.write(26);
      delay(400);
      handServo.write(180);
      delay(400);
      boxServo.write(0);
      ////////////
      DongCongTac();
}
