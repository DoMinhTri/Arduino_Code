

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//   Port  --- Tiny13 Pin   ---  Arduino UNO ISP
//     0       5            -->   13
//     1       6            -->   12
//     2       7            -->   11
//     3       2            -->   
//     4       3            -->   
//             1 ( Reset)   -->   10    ---|
//             4 ( GND )    -->   GND   ---|--> ( >= 10uF )
//             8 ( VCC )    -->   VCC
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// Nguyên lý hoạt động :
// khi phát hiện có sự chuyển trạng thái thì tăng giá trị thời gian
// mỗi giá trị được tính là 1 giây.

#define AutoReset     0
#define LedTH         4
#define Led           5

int ThoiGianReset   = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  pinMode(Led,        OUTPUT);          // Led
  pinMode(LedTH,      OUTPUT);          // LedTH
  pinMode(AutoReset,  OUTPUT);          // AutoReset
  
  digitalWrite(LedTH, HIGH);
  digitalWrite(Led, LOW);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  ThoiGianReset = millis()/1000; 
  if(ThoiGianReset > 10 ){ digitalWrite(AutoReset, HIGH); } // Reset bộ nhớ
  //////////////////////////////////////////////////
}


