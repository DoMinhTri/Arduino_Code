const int trig = 12;//chân trig của HC-SR04
const int echo = 11;//chân echo của HC-SR04

void setup()
{
  Serial.begin(9600);//giao tiếp Serial với baudrate 9600
  pinMode(trig,OUTPUT);//chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);//chân echo sẽ nhận tín hiệu
}

void loop()
{
  unsigned long duration;//biến đo thời gian
  int distance;//biến lưu khoảng cách

  /* phát xung từ chân trig */
  digitalWrite(trig,0);//tắt chân trig
  delayMicroseconds(2);
  digitalWrite(trig,1);// phát xung từ chân trig
  delayMicroseconds(5);// xung có độ dài 5 microSeconds
  digitalWrite(trig,0);//tắt chân trig

  /*tính toán thời gian*/
  duration = pulseIn(echo,HIGH);//đo độ rộng xung HIGH ở chân echo. ( http://arduino.vn/reference/pulsein )
  distance = int(duration/2/29.412);//tính khoảng cách đến vật.

  /*in kết quả ra Serial monitor*/
  Serial.print(distance);
  Serial.println("cm");
  delay(200);
}
