
#define Vien 0
#define Chu  1
int TGDelay = 5000;
void setup()
{
  pinMode(Vien, OUTPUT);
  pinMode(Chu, OUTPUT);
}

void loop() 
{
  if(TGDelay < 500) TGDelay = 5000;
  NhayAll_10(); 
  NhayVien_10(); 
  NhayChu_10(); 
  TGDelay = TGDelay/2; 
  NhayAll_10(); 
  NhayChu_10(); 
  NhayChu_10(); 
  TGDelay = TGDelay/2; 
  NhayVien_10(); 
  NhayVien_10(); 
  NhayAll_10(); 
  TGDelay = TGDelay/2; 
  NhayChu_10(); 
  NhayVien_10(); 
  NhayChu_10(); 
  TGDelay = TGDelay/2; 
}

void NhayAll_10()  { NhayAll();  NhayAll();  NhayAll();  NhayAll();  NhayAll();  NhayAll();  NhayAll();  NhayAll();  NhayAll();  NhayAll();  }
void NhayVien_10() { NhayVien(); NhayVien(); NhayVien(); NhayVien(); NhayVien(); NhayVien(); NhayVien(); NhayVien(); NhayVien(); NhayVien(); }
void NhayChu_10()  { NhayChu();  NhayChu();  NhayChu();  NhayChu();  NhayChu();  NhayChu();  NhayChu();  NhayChu();  NhayChu();  NhayChu();  }

void NhayVien()
{
  digitalWrite(Vien, HIGH);  delay(TGDelay); digitalWrite(Vien, LOW);   delay(TGDelay);             
}

void NhayAll()
{
  digitalWrite(Vien, HIGH); digitalWrite(Chu, HIGH);  delay(TGDelay);   
  digitalWrite(Vien, LOW);  digitalWrite(Chu, LOW);   delay(TGDelay);   
}


void NhayChu()
{
  digitalWrite(Chu, HIGH);  delay(TGDelay);             
  digitalWrite(Chu, LOW);   delay(TGDelay);             
}
