
void HanhDong_6()
{
      Serial.println("Action 6");      
      BoxDungLow(); 
      delay(1000);
      for (int i = 0; i < 12; i++)
      {
        for (int j = 70; j <= 90; j++){
          boxServo.write(j);
          delay(6);
        }
        delay(200);
      }
      for (int i = 180; i >= 35; i--)
      {
        handServo.write(i);
        delay(30);
      }
      handServo.write(26);
      delay(400);
      handServo.write(180);
      delay(400);
      boxServo.write(70);
      delay(1500);
      for (int i = 70; i <= 90; i++){
        boxServo.write(i);
        delay(6);
      }
      delay(2000);
      boxServo.write(80);
      ////////////
      delay(2000);
      DongCongTac();
      delay(2000);
      boxServo.write(0);
}
///////////////////////////////////////
void HanhDong_7()
{
      Serial.println("Action 7");
      BoxDungLow(); 
      delay(400);
      handServo.write(26);
      delay(1000);
      for (int i = 0; i < 6; i++)
      {
        BoxDungLow();         
        delay(200);
      }
      delay(500);
      handServo.write(180);
      delay(400);
      boxServo.write(0);
      ////////////
      delay(2000);
      DongCongTac();
}
///////////////////////////////////////
void HanhDong_8()
{
      Serial.println("Action 8");      
      BoxDungLow(); 
      handServo.write(26);
      delay(2000);
      for (int i = 0; i < 7; i++)
      {
        handServo.write(75);
        delay(100);
        handServo.write(40);
        delay(100);
      }
      delay(500);
      handServo.write(180);
      delay(1000);
      boxServo.write(0);
      ////////////
      delay(2000);
      DongCongTac();
}  
///////////////////////////////////////
void HanhDong_9()
{
      Serial.println("Action 9");      
      BoxDungLow(); 
      delay(2000);
      boxServo.write(90);
      for (int i = 180; i >=35; i--)
      {
        handServo.write(i);
        delay(40);
      }
      delay(500);
      handServo.write(26);
      delay(200);
      for (int i = 26; i <=180; i++)
      {
        handServo.write(i);
        delay(40);
      }
      boxServo.write(70);
      delay(2000);
      boxServo.write(90);
      delay(3000);
      boxServo.write(0);
      ////////////
      delay(2000);
      DongCongTac();
}    
///////////////////////////////////////
void HanhDong_10()
{
      Serial.println("Action 10");      
      BoxDungLow(); 
      delay(1000);
      for (int i = 0; i < 3; i++)
      {
        BoxDungLow(); 
        delay(100);
      }
      boxServo.write(90);
      delay(100);
      handServo.write(26);
      delay(650);
      handServo.write(180);
      delay(400);
      boxServo.write(70);
      ////////////
      delay(2000);
      DongCongTac();
      delay(2000);
      boxServo.write(0);
}
