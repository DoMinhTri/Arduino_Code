 
#include "x_nucleo_nfc04.h"



void setup() {

  Serial.begin(9600);
  if(X_Nucleo_Nfc04.begin() == 0) {
    Serial.println("System Init done!");
  
  } else {
    Serial.println("System Init failed!");
    while(1);
  }

  Serial.println(X_Nucleo_Nfc04.ReadID());
}

void loop() {  
  
} 
