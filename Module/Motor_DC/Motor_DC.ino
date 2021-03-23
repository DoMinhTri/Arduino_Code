// Define motor pins
int motorPinPlus = 4;
int motorPinMinus = 5;
int motorPinEnable = 6;

void setup()
{
   pinMode(motorPinPlus, OUTPUT);
   pinMode(motorPinMinus, OUTPUT);
   pinMode(motorPinEnable, OUTPUT);
   Serial.begin(9600);
   pinMode(7,INPUT_PULLUP);
   pinMode(8,OUTPUT);
   digitalWrite(7,HIGH);
   digitalWrite(8,LOW);
}

void loop()
{
  
   // Define speed
   bool NutNhan = digitalRead(7);
    digitalWrite(8,NutNhan);
   //if(NutNhan)
     //digitalWrite(8,HIGH);
   //else
     // digitalWrite(8,LOW);
   int motor_speed;

   // Accelerate forward
   //for (motor_speed = 0; motor_speed < 250; motor_speed+=50)
   {
      setMotor(NutNhan, 1000);
      delay(10);
   }
}

// Function to control the motor
void setMotor(boolean forward, int motor_speed)
{
   digitalWrite(motorPinPlus, forward);
   digitalWrite(motorPinMinus, !forward);
   analogWrite(motorPinEnable, motor_speed);
}
