#include <AH_EasyDriver.h>
AH_EasyDriver stepperH(200,14,12,27,26,28);
AH_EasyDriver stepperV(200,32,33,27,26,28);
void setup(){
  stepperH.resetDriver();
  stepperH.enableDriver();
  stepperH.setMicrostepping(0);
  Serial.begin(9600);
  stepperH.setSpeedRPM(100);
  stepperV.setSpeedRPM(100);
  pinMode(17,INPUT);
}

void loop(){
  for (size_t i = 0; i < 360; i++) {
    stepperH.rotate(1);
    stepperV.rotate(1);
  }
  delay(1000);
}
