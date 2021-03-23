
void HanhDong_11()
{
      Serial.println("Action 11");      
      BoxDungLow(); 
      handServo.write(26);
      delay(2000);
      for (int i = 0; i < 3; i++)
      {
        handServo.write(65);
        delay(200);
        handServo.write(40);
        delay(200);
      }
      delay(1500);
      handServo.write(180);
      delay(400);
      boxServo.write(70);
      delay(1500);
      for (int i = 70; i <= 90; i++){
        boxServo.write(i);
        delay(6);
      }
      handServo.write(26);
      delay(3000);
      handServo.write(180);
      delay(400);
      boxServo.write(70);
      ////////////
      delay(2000);
      DongCongTac();
      delay(2000);
      boxServo.write(0);
}
///////////////////////////////////////
void HanhDong_12()
{
      Serial.println("Action 12");      
      BoxDungLow(); 
      delay(1000);
      for (int i = 180; i >= 26; i--)
      {
        handServo.write(i);
        delay(50);
      }
      delay(2000);
      handServo.write(26);
      delay(200);
      handServo.write(180);
      delay(400);
      boxServo.write(70);
      ////////////
      delay(2000);
      DongCongTac();
      delay(2000);
      boxServo.write(0);
}
///////////////////////////////////////
void HanhDong_13()
{
      Serial.println("Action 13");      
      BoxDungLow(); 
      delay(2000);
      for (int i = 180; i >= 60; i--)
      {
        handServo.write(i);
        delay(40);
      }
      for (int i = 0; i < 3; i++)
      {
        handServo.write(60);
        delay(200);
        handServo.write(40);
        delay(800);
      }
      handServo.write(26);
      delay(2000);
      handServo.write(180);
      delay(500);
      boxServo.write(70);
      ////////////
      delay(2000);
      DongCongTac();
      delay(2000);
      boxServo.write(0);
}  
///////////////////////////////////////
void HanhDong_14()
{
      Serial.println("Action 14");     
      BoxDungLow(); 
      delay(4000);
      boxServo.write(90);
      handServo.write(26);
      delay(650);
      handServo.write(180);
      delay(500);
      boxServo.write(90);
      delay(4000);
      boxServo.write(70);
      ////////////
      delay(2000);
      DongCongTac();
      delay(2000);
      boxServo.write(0);
}    
///////////////////////////////////////
void HanhDong_15()
{
      Serial.println("Action 15");    
      BoxDungLow(); 
      delay(1000);
      for (int i = 0; i < 3; i++)
      {      
        for (int j = 40; j <= 90; j++){
          boxServo.write(j);
          delay(20);
        }
        for (int j = 90; j >= 40; j--)
        {
          boxServo.write(j);
          delay(20);
        }
      }
      for (int j = 70; j <= 90; j++){
        boxServo.write(j);
        delay(50);
      }
      for (int i = 180; i >= 35; i--)
      {
        handServo.write(i);
        delay(40);
      }
      delay(1000);
      handServo.write(26);
      delay(400);
      handServo.write(180);
      delay(400);
      boxServo.write(70);
      ////////////
      delay(2000);
      DongCongTac();
      delay(2000);
      boxServo.write(0);
}
