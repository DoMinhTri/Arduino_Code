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




int Led_1  = 0 ;
int Led_2  = 1 ;
int Led_3  = 2 ;
int Led_4  = 4 ;
int Led_5  = 3 ;



void setup()
{
  pinMode(Led_1, OUTPUT);   
  pinMode(Led_2, OUTPUT);   
  pinMode(Led_3, OUTPUT);   
  pinMode(Led_4, OUTPUT);   
  pinMode(Led_5, OUTPUT);   
}
 
void loop()
{
  ///  1
  DenXuoiNguoc();
  DenXuoiNguoc();
  
  ///  2
  Den2Huong();
  Den2Huong();
  
  ///  3
  DenLacLu();
  
  ///  2
  Den2Huong();
  Den2Huong();
    
  ///  1
  DenXuoiNguoc();
  DenXuoiNguoc();
  
  ///  4
  Bat5Den();
  delay(2000000);
}


void Bat5Den()
{
    digitalWrite( Led_1, HIGH) ; 
    digitalWrite( Led_2, HIGH) ;
    digitalWrite( Led_3, HIGH) ;  
    digitalWrite( Led_4, HIGH) ;
    digitalWrite( Led_5, HIGH) ;
}

void DenXuoiNguoc()
{
    digitalWrite( Led_1, HIGH); delay(250);
    digitalWrite( Led_1, LOW) ; delay(250);
    
    digitalWrite( Led_2, HIGH); delay(250);
    digitalWrite( Led_2, LOW) ; delay(250);
    
    digitalWrite( Led_3, HIGH); delay(250);
    digitalWrite( Led_3, LOW) ; delay(250);    

    digitalWrite( Led_4, HIGH); delay(250);
    digitalWrite( Led_4, LOW) ; delay(250);

    digitalWrite( Led_5, HIGH); delay(250);
    digitalWrite( Led_5, LOW) ; delay(250);    
    
    digitalWrite( Led_4, HIGH); delay(250);
    digitalWrite( Led_4, LOW) ; delay(250);    

    digitalWrite( Led_3, HIGH); delay(250);
    digitalWrite( Led_3, LOW) ; delay(250);

    digitalWrite( Led_2, HIGH); delay(250);
    digitalWrite( Led_2, LOW) ; delay(250);     
}

void Den2Huong()
{
    digitalWrite( Led_1, HIGH); digitalWrite( Led_5, HIGH) ; delay(250);
    digitalWrite( Led_1, LOW);  digitalWrite( Led_5, LOW)  ; delay(250);
    
    digitalWrite( Led_2, HIGH); digitalWrite( Led_4, HIGH) ; delay(250);
    digitalWrite( Led_2, LOW);  digitalWrite( Led_4, LOW)  ; delay(250);
    
    digitalWrite( Led_3, HIGH); delay(250);
    digitalWrite( Led_3, LOW);  delay(250);
    
    digitalWrite( Led_2, HIGH); digitalWrite( Led_4, HIGH) ; delay(250);
    digitalWrite( Led_2, LOW);  digitalWrite( Led_4, LOW)  ; delay(250);
}

void DenLacLu()
{
   for( int i=0;i<=5;i++)
   {
      digitalWrite( Led_1, HIGH); digitalWrite( Led_4, HIGH) ; delay(250);
      digitalWrite( Led_1, LOW);  digitalWrite( Led_4, LOW)  ; delay(250);
    
      digitalWrite( Led_2, HIGH); digitalWrite( Led_5, HIGH) ; delay(250);
      digitalWrite( Led_2, LOW);  digitalWrite( Led_5, LOW)  ; delay(250);
   }
}
