const int trigPin = 3;
const int echoPin = 4;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
  Serial.print(KhoangCach());
  Serial.print("cm");
  Serial.println();
  delay(100);
}

int KhoangCach()
{
  long duration, inches, cm;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  cm     = microsecondsToCentimeters(duration);
  inches = microsecondsToInches(duration);
  return cm;
}

long microsecondsToInches(long microseconds){  return microseconds / 74 / 2; }
long microsecondsToCentimeters(long microseconds){  return microseconds / 29 / 2; }
