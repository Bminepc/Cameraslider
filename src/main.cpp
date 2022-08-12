#include <AH_EasyDriver.h>

//Webserver
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"

const char* ssid = "Cameraslider";
const char* password = "1234567890";
AsyncWebServer server(80);

#define TasterLeft 19
#define TasterRight 18

AH_EasyDriver stepperCamera(200,14,12,27,26,28);
AH_EasyDriver stepperSlider(200,32,33,27,26,28);

int direction = 1;
int nextDirection = 1;
int rotationSlider = 5;
int rotationCamera = 1;
float speedFactor = 1;
boolean hit = false;

void setup(){
  Serial.begin(115200);
  // WiFi Station initialisation
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("Cameraslider") ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  // HTTP server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", html);
  });
  server.begin();
  Serial.println("HTTP server started!");

  //Steppermotorsetup
  stepperCamera.enableDriver();
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
  delay(1);
}
