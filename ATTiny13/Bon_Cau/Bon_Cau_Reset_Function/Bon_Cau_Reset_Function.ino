

///////////////////////////////////////////
///////////////////////////////////////////
//   Port  --- Tiny13 Pin   ---  Arduino UN
//     2       7            -->   13
//     1       6            -->   12
//     0       5            -->   11
//     3       2            -->   
//     4       3            -->   
//             1 ( Reset)   -->   10    ---
//             4 ( GND )    -->   GND   ---
//             8 ( VCC )    -->   VCC
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
// Nguyên lý hoạt động :
// khi phát hiện có sự chuyển trạng thái th
// mỗi giá trị được tính là 1 giây.

#define DenLed   0

void(* RESET) (void)  = 0;
//void RESTART(){ asm volatile (" jmp 0"); }

///////////////////////////////////////////
void setup() 
{
  pinMode(DenLed,  OUTPUT); 
  digitalWrite(DenLed, LOW);  
  delay(1000);  
  digitalWrite(DenLed, HIGH);
}
///////////////////////////////////////////
void loop()
{
  delay(1000);  
  RESET();
}


