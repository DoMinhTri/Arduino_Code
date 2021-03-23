#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 5
#define SP1 6
#define SP2 9
#include <SoftwareSerial.h>
SoftwareSerial Blueth(12, 13); // RX, TX

char cmd;
int sp = 147;

void setup()
{
  Serial.begin(9600);
  Blueth.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(SP1, OUTPUT);
  pinMode(SP2, OUTPUT);
  motor(0, 0);
}

void loop()
{
  if (Blueth.available())
  {
    cmd = Blueth.read();
    switch (cmd)
    {
      default:
        motor(0, 0);
        break;
      case 'S':
        motor(0, 0);
        break;
      case 'F':
        motor(1, 1);
        break;
      case 'B':
        motor(-1, -1);
        break;
      case 'L':
        motor(-1, 1);
        break;
      case 'R':
        motor(1, -1);
        break;
      case 'I':
        motor(1, 0);
        break;
      case 'G':
        motor(0, 1);
        break;
      case 'H':
        motor(0, -1);
        break;
      case 'J':
        motor(-1, 0);
        break;
      case '1':
        sp = 1*12 + 147;
        break;
      case '2':
        sp = 2*12 + 147;
        break;
      case '3':
        sp = 3*12 + 147;
        break;
      case '4':
        sp = 4*12 + 147;
        break;
      case '5':
        sp = 5*12 + 147;
        break;
      case '6':
        sp = 6*12 + 147;
        break;
      case '7':
        sp = 7*12 + 147;
        break;
      case '8':
        sp = 8*12 + 147;
        break;
      case '9':
        sp = 9*12 + 147;
        break;
    } 
    Serial.print(cmd);
    Serial.print("-");
    Serial.println(sp);
    analogWrite(SP1, sp);
    analogWrite(SP2, sp);
  }
}

void motor(int motor1, int motor2)
{
  switch (motor1)
  {
    case 0:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      break;
    case 1:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      break;
    case -1:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      break;
    default:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      break;    
  }
  switch (motor2)
  {
    case 0:
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    case 1:
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    case -1:
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
    default:
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;    
  }  
}
