////////////////////////////////////////////////////////////////////
// Nguyên lý hoạt động :
// khi phát hiện có sự chuyển trạng thái thì tăng giá trị thời gian
// mỗi giá trị được tính là 1 giây.



const int CamBien  = 7;
const int ledPin   = 13;

int TTT = 0, TTS = 0;     // Trạng thái phát hiện khi có sự thay đổi giá trị sang dương - có người chuyển động
boolean HoatDong = false; // khi trạng thái là đang hoạt động thì tính thời gian
int TGHienTai = 0, TGTruoc = 0, TongTG = 0 ; 


void setup() 
{
  pinMode(ledPin, OUTPUT);      
  pinMode(CamBien, INPUT);      
}

void loop()
{
  TTT = digitalRead(CamBien);
  if(TTT > 0) { if(TongTG <= 90) TongTG++; }
  //////////////////////////////////////////////////
  if(TTT != TTS)
  {
    if(TTT > 0) 
    {
      if(HoatDong == false)
      {
        HoatDong  = true;
        TGHienTai = millis()/1000;
        TGTruoc   = TGHienTai;
      }
    }
  }
  //////////////////////////////////////////////////  
  TTS = TTT;
  TGHienTai = millis()/1000;
  //////////////////////////////////////////////////
  if(HoatDong)
  {
      digitalWrite(ledPin, HIGH);  
      if((TGHienTai - TGTruoc) == 1 ) 
      {
          TongTG--;
          TGTruoc = TGHienTai;
          if(TongTG <=0 )HoatDong = false;
      }
  }
  else
  {
     digitalWrite(ledPin, LOW);  
  }
  //////////////////////////////////////////////////
  delay(50);
}
