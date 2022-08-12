#include <AH_EasyDriver.h>

#define TasterH 19

AH_EasyDriver stepperH(200,14,12,27,26,28);
AH_EasyDriver stepperV(200,32,33,27,26,28);



void setup(){
  stepperH.enableDriver();
  Serial.begin(9600);
  stepperH.setSpeedRPM(100);
  stepperV.setSpeedRPM(100);
  pinMode(TasterH, INPUT);
}

void loop(){
    if(digitalRead(TasterH) != HIGH){
    stepperH.rotate(1);
    stepperV.rotate(1);
    }
}
