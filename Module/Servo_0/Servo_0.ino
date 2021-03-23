#include <Servo.h> 
 
Servo C_1A,C_1B,C_2A,C_2B,C_3A,C_3B,C_4A,C_4B,C_5A,C_5B,C_6A,C_6B;  
           
void setup() 
{ 
  C_1A.attach(2); 
  C_2A.attach(3); 
  C_3A.attach(4); 
  C_4A.attach(5); 
  //C_5A.attach(6);
  //C_6A.attach(7); 
  
  C_1B.attach(8); 
  C_2B.attach(9); 
  C_3B.attach(10); 
  C_4B.attach(11); 
  //C_5B.attach(12); 
  //C_6B.attach(13);
} 
 
 
void loop() 
{ 
  C_1A.write(0); 
  C_2A.write(0); 
  C_3A.write(0); 
  C_4A.write(0); 
  //C_5A.attach(6);
  //C_6A.attach(7); 
  
  C_1B.write(0); 
  C_2B.write(0); 
  C_3B.write(0); 
  C_4B.write(0); 
  //C_5B.attach(12); 
  //C_6B.attach(13);
  
  /*
  for(int i=0;i<=180;i++)
  { C_1A.write(i);
     delay(10);    
  }
  
  for(int j=180;j<=0;j--)
  { C_1A.write(j);
     delay(10);    
  }
  */
   //C_3A.write(0);
   //delay(1000);       
   //C_5A.write(0);
   //delay(1000);    
   //C_2A.write(0);              
   //delay(1000);    
   //C_4A.write(0);           
   //delay(1000);    
   //C_6A.write(0);                 
   delay(1000); 
} 
