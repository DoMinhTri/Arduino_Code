#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 10  //using digital pin 4 on arduino nano 328
#include <TMRpcm.h>           //  also need to include this library...
 
TMRpcm tmrpcm;   // create an object for use in this sketch
 int SW1; 
 int SW2;
 int SW3;
 int SW4;
void setup(){
 pinMode(A0,INPUT);  //Define A0 as digital input.
 pinMode(A1,INPUT);  //Define A1 as digital input. 
 pinMode(A2,INPUT);  //Define A2 as digital input.
 pinMode(A3,INPUT);  //Define A3 as digital input.
 
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
 
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
  return;   // don't do anything more if not
  }
 tmrpcm.volume(1);
 tmrpcm.play("1.wav"); //the sound file "1" will play each time the arduino powers up, or is reset
}
 
void loop(){  
 SW1=digitalRead(A0); 
 SW2=digitalRead(A1);
 SW3=digitalRead(A2);
 SW4=digitalRead(A3);
  
     if (SW1 == LOW) { //if SW1 pressed then play file "6.wav"
      tmrpcm.play("6.wav");
    } else if(SW2 == LOW){ //if SW2 pressed then play file "4.wav"
      tmrpcm.play("4.wav");
    } else if(SW3 == LOW){ //if SW3 pressed then play file "5.wav"
      tmrpcm.play("5.wav");
    } else if(SW4 == LOW){  //if SW4 pressed then play file "3.wav"
      tmrpcm.play("3.wav");
    }
  
}

