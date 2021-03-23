byte latchPin = 1; 
byte clockPin = 0;
byte dataPin  = 2;

void setup() {     
  
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
//  Serial.begin(9600);
  // cài đặt timer
  TCCR0B |=  _BV(CS02)  ; 
}

int timer = 150;

void _delay(word time = 1) { // Xây dựng lại hàm delay
  for (word i = 1; i<=time;i++) {
    delayMicroseconds(timer);
  }
}

void _shiftOut(byte dataPin, byte clockPin, byte ledStatus) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ledStatus);  
  digitalWrite(latchPin, HIGH); 
}
int k = 0;
void loop() {
  k++;
  //Serial.print(k);
  byte ledStatus = 0; //trạng thái Led
  _shiftOut(dataPin, clockPin, 0);
  _delay(2000);
  for (byte i = 0; i < 8; i++) {
    ledStatus = ledStatus << 1 | 1;
    _shiftOut(dataPin, clockPin, ledStatus);
    _delay(2000);
  }
}
