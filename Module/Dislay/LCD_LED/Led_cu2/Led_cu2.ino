/*

  Blink

  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is created by VanceAnce
  feel free to use it and visit my site
  vanceance.blogspot.com
 */

  int e[3][3]={{2,3,4},{5,6,7},{8,9,10}};
  int i=0;
  int d=200;
void setup() {               

  // initialize the digital pin as an output.

  // Pin 13 has an LED connected on most Arduino boards


  int i=2;

  while(i<=13)
  {
    pinMode(i, OUTPUT);
    i++;
  }
}

void loop() {
   i=0;
  //---------------------------------------------------
  while(i<3)
  {

    digitalWrite(e[i][0], HIGH);
    digitalWrite(13, HIGH); 
    delay(d);
    digitalWrite(13, LOW);

    digitalWrite(12, HIGH);
    delay(d);
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    delay(d);  

    digitalWrite(e[i][0], LOW);
    //digitalWrite(11,LOW);  

    digitalWrite(e[i][1], HIGH);
    delay(d);
    digitalWrite(e[i][1], LOW);
    digitalWrite(e[i][2], HIGH);
    delay(d);
    digitalWrite(11, LOW);  

    digitalWrite(12, HIGH);
    delay(d);
    digitalWrite(12, LOW);

    digitalWrite(13, HIGH);
    delay(d);
    digitalWrite(e[i][2], LOW);

    digitalWrite(e[i][1], HIGH);
    delay(d);
    digitalWrite(e[i][1], LOW);

    digitalWrite(13, LOW);
    i++;
  }

  digitalWrite(12,HIGH);
  digitalWrite(9,HIGH);
  delay(d);
  digitalWrite(9,LOW);

  digitalWrite(6,HIGH);
  delay(d);
  digitalWrite(6,LOW);

  digitalWrite(3,HIGH);
  delay(d);
  digitalWrite(12,LOW);

  digitalWrite(13,HIGH);
  delay(d);
  digitalWrite(13,LOW);

  digitalWrite(3,LOW);
  i=0;

  //--------------------------------------------------- E3,S1 -> E2,S2 -> E1,S1

  digitalWrite(11, HIGH);   // S1 on
  digitalWrite(2, HIGH);    // E1 on
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(d);
  digitalWrite(11, LOW);   // S1 off
  digitalWrite(2, LOW);    // E1 off
  digitalWrite(3, LOW);

  digitalWrite(4, LOW);
  digitalWrite(12, HIGH);   // S2 on
  digitalWrite(5, HIGH);    // E2 on
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  delay(d);
  digitalWrite(12, LOW);   // S2 off
  digitalWrite(5, LOW);    // E2 off
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(13, HIGH);   // S2 on
  digitalWrite(8, HIGH);    // E3 on
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  delay(d);
  digitalWrite(13, LOW);   // S2 off
  digitalWrite(8, LOW);    // E3 off
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  delay(d);

  //--------------------------------------------------- E3,S1 -> E2,S2 -> E1,S1

  digitalWrite(13, HIGH);   // S1 on
  digitalWrite(2, HIGH);    // E1 on
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(d);
  digitalWrite(13, LOW);   // S1 off
  digitalWrite(2, LOW);    // E1 off

  digitalWrite(3, LOW);
  digitalWrite(4, LOW);

  digitalWrite(12, HIGH);   // S2 on
  digitalWrite(5, HIGH);    // E2 on
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  delay(d);
  digitalWrite(12, LOW);   // S2 off
  digitalWrite(5, LOW);    // E2 off
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);

  digitalWrite(11, HIGH);   // S2 on
  digitalWrite(8, HIGH);    // E3 on
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  delay(d);
  digitalWrite(11, LOW);   // S2 off
  digitalWrite(8, LOW);    // E3 off
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  delay(d);

  //--------------------------------------------------- Mittlere Reihe v.h.n.v

  digitalWrite(13, HIGH);   // set the LED on
  digitalWrite(5, HIGH);    // set the LED off
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  delay(d);
  digitalWrite(13, LOW);
  //delay(400);
  digitalWrite(12, HIGH);
  delay(d);
  digitalWrite(12, LOW);
  //delay(400);
  digitalWrite(11, HIGH);
  delay(d);
  digitalWrite(11, LOW);
  delay(d);
  //------------------------------------ Ganzer Würfel 3x Blinken

  while(i<3) {

    digitalWrite(2, HIGH);    // E1 on
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);

    digitalWrite(5, HIGH);    // E2 on
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);

    digitalWrite(8, HIGH);    // E3 on
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);

    digitalWrite(13, HIGH);   // S1-3 on
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);

    delay(d);

    digitalWrite(2, LOW);    // E1 off
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);

    digitalWrite(5, LOW);    // E2 off
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);

    digitalWrite(8, LOW);    // E3 off
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);

    digitalWrite(13, LOW);    // S1-3 off
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);

    delay(d);

    i++; }

  //-------------------------------------------------------- Ebenen Ein/aus

  digitalWrite(13, HIGH);   // S1-3 on
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);

  digitalWrite(2, HIGH);    // E1 on
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);

  delay(d);

  digitalWrite(2, LOW);    // E1 off
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);

  digitalWrite(5, HIGH);    // E2 on
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);

  delay(d);

  digitalWrite(5, LOW);    // E2 off
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);

  digitalWrite(8, HIGH);    // E3 on
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);

  delay(d);

  digitalWrite(8, LOW);    // E3 off
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);

  digitalWrite(13, LOW);    // S1-3 off
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);

  delay(d);

  //-------------------------------------------------------- Spalten ein/aus

  digitalWrite(2, HIGH);    // E1 on
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);

  digitalWrite(5, HIGH);    // E2 on
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);

  digitalWrite(8, HIGH);    // E3 on
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);

  digitalWrite(13, HIGH);   // S1 on

  delay(d);

  digitalWrite(13, LOW); // S1 off

  digitalWrite(12, HIGH);   // S1 on
  delay(d);
  digitalWrite(12, LOW); // S1 off

  digitalWrite(11, HIGH);   // S1 on
  delay(d);
  digitalWrite(11, LOW); // S1 off

  digitalWrite(2, LOW);    // E1 off
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);

  digitalWrite(5, LOW);    // E2 off
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);

  digitalWrite(8, LOW);    // E3 off
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);

  delay(d);

  //--------------------------------------------------------
}
