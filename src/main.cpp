#include <AH_EasyDriver.h>

#define TasterLeft 19
#define TasterRight 18

AH_EasyDriver stepperCamera(200,14,12,27,26,28);
AH_EasyDriver stepperSlider(200,32,33,27,26,28);

int direction = 1;
int rotationSlider = 5;
int rotationCamera = 1;
float afterTurn = 1;

void setup(){
  //Steppermotorsetup
  stepperCamera.enableDriver();
  Serial.begin(9600);
  stepperCamera.setSpeedRPM(100);
  stepperSlider.setSpeedRPM(100);

  //Endtaster
  pinMode(TasterLeft, INPUT);
  pinMode(TasterRight, INPUT);
}

void loop(){
    if(digitalRead(TasterLeft) == HIGH){
      direction = 1;
      afterTurn = 0.1;
    }else if(digitalRead(TasterRight) == HIGH){
      direction = -1;
      afterTurn = 0.1;
    }

    stepperCamera.setSpeedRPM(100 * afterTurn);
    stepperSlider.setSpeedRPM(100 * afterTurn);
    stepperCamera.rotate(direction * rotationCamera);
    stepperSlider.rotate(direction * rotationSlider);

    if(afterTurn < 1){
      afterTurn = afterTurn + 0.001;
      Serial.println(afterTurn);
    }
}



/*
  stepperCamera.rotate(1);
  stepperSlider.rotate(1);
*/
