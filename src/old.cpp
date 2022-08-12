#include <AH_EasyDriver.h>
AH_EasyDriver stepperH(200,14,12,27,26,28);
AH_EasyDriver stepperV(200,32,33,27,26,28);
void setup(){
  stepperH.resetDriver();
  stepperH.enableDriver();
  stepperH.setMicrostepping(0);/
  Serial.begin(9600);
  stepperH.setSpeedRPM(100);
  stepperV.setSpeedRPM(100);
  pinMode(17,INPUT);
}

void loop(){
  for (size_t i = 0; i < 360; i++) {
    stepperH.rotate(1)
    stepperV.rotate(1)
  }
  Serial.println(analogRead(17));

  /stepperH.rotate(360);
  stepperV.rotate(360);


  delay(1000);

  stepperH.rotate(-360);
  stepperV.rotate(-360);

  delay(1000);/
}


/
int Schrittmotor=12; // Pin 8 an „Step“
int Richtung = 14; // Pin7 an „Dir“
int MODE1 = 27;
int MODE2 = 26;

void setup()
{
  Serial.begin(9600);
  pinMode(MODE1, OUTPUT);
  pinMode(MODE2, OUTPUT);
  pinMode(Schrittmotor, OUTPUT);
  pinMode(Richtung, OUTPUT);
  digitalWrite(Richtung, HIGH);
  digitalWrite(MODE1, HIGH);
  digitalWrite(MODE2, HIGH);
}

void loop()
{
  Serial.println("LOW");
  digitalWrite(Schrittmotor, HIGH);
  //delay(100);
  Serial.println("HIGH");
  digitalWrite(Schrittmotor, LOW);
  //delay(100);
}
