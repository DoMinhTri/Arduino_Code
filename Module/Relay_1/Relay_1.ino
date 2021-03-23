  
int Relay = 2;
int incomingByte;      // a variable to read incoming serial data into

void setup()
{
  Serial.begin(9600); // initialize serial communication
  pinMode(Relay, OUTPUT);
  Serial.println("Begin ...");
 }
 
 void loop() 
 {
   if (Serial.available() > 0) 
   {
     incomingByte = Serial.read(); // read the oldest byte in the serial buffer
     if (incomingByte == '1') { digitalWrite(Relay, HIGH); Serial.println("1");   }
     if (incomingByte == '2') { digitalWrite(Relay, LOW) ; Serial.println("2");   }
   }
 }
