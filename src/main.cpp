#include <AH_EasyDriver.h>

#define TasterLeft 19
#define TasterRight 18

AH_EasyDriver stepperCamera(200,14,12,27,26,28);
AH_EasyDriver stepperSlider(200,32,33,27,26,28);

int direction = 1;
int nextDirection = 1;
int rotationSlider = 5;
int rotationCamera = 1;
float speedFactor = 1;
Boolean hit = false;

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
    hit = true;
    nextDirection = 1;
  }else if(digitalRead(TasterRight) == HIGH){
    hit = true;
    nextDirection = -1;
  }
  if(hit && speedFactor >= 0.1){
    speedFactor = speedFactor - 0.005;
  }else if(hit){
    direction = nextDirection;
    hit = false;
  }else if(speedFactor < 1){
    speedFactor = speedFactor + 0.005;
  }


  stepperCamera.setSpeedRPM(100 * speedFactor);
  stepperSlider.setSpeedRPM(100 * speedFactor);
  stepperCamera.rotate(direction * rotationCamera);
  stepperSlider.rotate(direction * rotationSlider);
}

/*
void loop(){
    if(digitalRead(TasterLeft) == HIGH){
      direction = 1;
      speedFactor = 0.1;
    }else if(digitalRead(TasterRight) == HIGH){
      direction = -1;
      speedFactor = 0.1;
    }

    stepperCamera.setSpeedRPM(100 * speedFactor);
    stepperSlider.setSpeedRPM(100 * speedFactor);
    stepperCamera.rotate(direction * rotationCamera);
    stepperSlider.rotate(direction * rotationSlider);

    if(speedFactor < 1){
      speedFactor = speedFactor + 0.005;
      Serial.println(speedFactor);
    }
    delay(1);
}
*/


/*
  stepperCamera.rotate(1);
  stepperSlider.rotate(1);
*/
