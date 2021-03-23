#include <Wire.h>
#include <DS1307.h> // written by  mattt on the Arduino forum and modified by D. Sjunnesson

char buffer[30];
int hour, minute, seconds, day, month, year;

void getTimeDate() {
  hour = RTC.get(DS1307_HR,true);        //read the hour and also update all the values by pushing in true
  minute = RTC.get(DS1307_MIN,false);    //read minutes without update (false)
  seconds = RTC.get(DS1307_SEC,false);   //read seconds
  day = RTC.get(DS1307_DATE,false);      //read date
  month = RTC.get(DS1307_MTH,false);     //read month
  year = RTC.get(DS1307_YR,false);       //read year 
}

void splitString(char* data) {
  Serial.print("Data entered: ");
  Serial.println(data);

  char* parameter; 
  parameter = strtok (data, " ,");

  while (parameter != NULL) {
    setDateTime(parameter);
    parameter = strtok (NULL, " ,");
  }

  // Clear the text and serial buffers
  for (int x=0; x<30; x++) {
    buffer[x]='\0';
  }
  Serial.flush();
}

void setDateTime(char* data) {
  RTC.stop();

  if((data[0] == 'H')) {
    int value = strtol(data+1, NULL, 10);
    value = constrain(value, 00, 24);
    RTC.set(DS1307_HR, value);  //set the hour
  }
  if((data[0] == 'M')) {
    int value = strtol(data+1, NULL, 10);
    value = constrain(value, 00, 59);
    RTC.set(DS1307_MIN, value);    //set the minutes 
  }
  /*if((data[0] == 'S')) {
   int value = strtol(data+1, NULL, 10);
   value = constrain(value, 00, 59);
   RTC.set(DS1307_SEC, value);    //set the seconds
   }*/
  if((data[0] == 'D')) {
    int value = strtol(data+1, NULL, 10);
    value = constrain(value, 1, 7);
    RTC.set(DS1307_DOW, value);    //set the day of the week
  }
  if((data[0] == 'A')) {
    int value = strtol(data+1, NULL, 10);
    value = constrain(value, 1, 31);
    RTC.set(DS1307_DATE, value);    //set the date
  }
  if((data[0] == 'T')) {
    int value = strtol(data+1, NULL, 10);
    value = constrain(value, 1, 12);
    RTC.set(DS1307_MTH, value);    //set the month
  }
  if((data[0] == 'Y')) {
    int value = strtol(data+1, NULL, 10);
    value = constrain(value, 00, 99);
    RTC.set(DS1307_YR, value);    //set the year 
  }  

  RTC.start(); 
}

void setup() {
  Serial.begin(9600);
  Serial.flush();
  Serial.println("--------------------------------------");
  Serial.println("| DS1307 - Real Time Clock Interface |");
  Serial.println("--------------------------------------");
  Serial.println();
  Serial.println("Usage: (CAPITAL LETTERS)");
  Serial.println(" H[0-24] (Hour), M[0-59] (Minutes),");
  Serial.println(" D[1-7] (Day of the Week), A[1-31] (Date), T[1-12] (Months), Y[0-99] (Year)");
  Serial.println();
  Serial.println("Ex.: 18:20 Monday, 06/01/2012 => H18, M20, D04, A06, T01, Y12");
  Serial.println();
}

void loop() {
  if (Serial.available() > 0) {
    int index=0;
    delay(100); // let the buffer fill up
    int numChar = Serial.available();
    if (numChar>34) {
      numChar=34;
    }
    while (numChar--) {
      buffer[index++] = Serial.read();
    }
    splitString(buffer);
  }

  getTimeDate();
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(seconds, DEC);
  Serial.print("  ");
  Serial.print(day, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.println();

  delay(5000);
}



