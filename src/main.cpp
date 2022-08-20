#include <AH_EasyDriver.h>

//Webserver
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"
#include <ArduinoJson.h>
#include <WebSocketsServer.h>

const char* ssid = "Cameraslider";
const char* password = "1234567890";
const size_t CAPACITY = JSON_ARRAY_SIZE(256);
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
bool con = false;                                  // Connection state of WebSocket

#define TasterLeft 19
#define TasterRight 18

AH_EasyDriver stepperCamera(200,14,12,27,26,28);
AH_EasyDriver stepperSlider(200,32,33,27,26,28);

int speedOfCamera = 100;
int speedOfSlider = 100;
int direction = 1; //Direction of Turning. Reversed at every End
int nextDirection = 1; //Temporary direction. Only used while deacelerating
int rotationSlider = 5; //Some Speed Modifier
int rotationCamera = 1; //Some Speed Modifier
float speedFactor = 1; //Factor for Acceleration and deacceleration
boolean hit = false; //Shows if endswitch was hit and Device is deacelerating
int Timeout = 1; //Simple Network Timeout 1 = Not Timeouted -1 = Timeouted
int millisToEnd = 0; //Time needed to travel from one Side to another in Milliseconds
int stateOfSlider = 0; //-1 = Errorstate 0 = Startup 1 = Setting 2 = Running
//Start and end position of Cameraangle
int angleLeft = 0;
int angleRight = 0;
int side = -1; //Used in Settingsmode (1 = Left; 0 = Right)
int numberOfSteps = 0; //The Number of Cycles the Camera turn must timeout to get the right rotation
int numberOfStepsCountdown = 0;


//Positiondetection of the Motors
void detectBounds(){
  millisToEnd = 0;
  Serial.println("Driving to Left End");
  while (digitalRead(TasterLeft) != HIGH) {
    stepperSlider.rotate(1);
  }
  Serial.println("Driving to Right End");
  while (digitalRead(TasterRight) != HIGH) {
    stepperSlider.rotate(-1);
    millisToEnd++;
    delay(1);
  }
  millisToEnd = millisToEnd * 2; //Correct some... well... bullshit...
  Serial.println("Time in Milliseconds from Left to Right:");
  Serial.println(millisToEnd);
}

void driveLeft(){
  if (stateOfSlider == 1) {
    while (digitalRead(TasterLeft) != HIGH) {
      stepperSlider.rotate(1);
    }
    Serial.println("Moved to left End");
    side = 1;
  }else{
    Serial.println("Wrong Mode");
  }
}

void driveRight(){
  if (stateOfSlider == 1) {
    while (digitalRead(TasterRight) != HIGH) {
      stepperSlider.rotate(-1);
    }
    Serial.println("Moved to right End");
    side = 0;
  }else{
    Serial.println("Wrong Mode");
  }
}

void turnLeft(){
  if (stateOfSlider == 1) {
      stepperCamera.rotate(1);
    Serial.println("Turned Left");
    if(side == 1) angleLeft--; else angleRight--;
  }else{
    Serial.println("Wrong Mode");
  }
}

void turnRight(){
  if (stateOfSlider == 1) {
      stepperCamera.rotate(-1);
    Serial.println("Turned Right");
    if(side == 1) angleLeft++; else angleRight++;
  }else{
    Serial.println("Wrong Mode");
  }
}

// Function Prototypes
void webSocketEvent(uint8_t , WStype_t, uint8_t * , size_t );
void sendResponse();

void endSettings(){
  if(angleLeft-angleRight != 0){
    numberOfSteps = millisToEnd/((angleLeft-angleRight)<0 ? ((angleLeft-angleRight) * -1) : (angleLeft-angleRight));
  }else{
    numberOfSteps = 0;
  }
}

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

  // WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started!");

  //Steppermotorsetup
  stepperCamera.enableDriver();
  stepperCamera.setSpeedRPM(100);
  stepperSlider.setSpeedRPM(100);

  //Endtaster
  pinMode(TasterLeft, INPUT);
  pinMode(TasterRight, INPUT);

  //Temporary
  Serial.println("Start");
  stepperSlider.rotate(1);
  Serial.println("End");

  detectBounds();
  stateOfSlider = 2;
}


void loop(){
  webSocket.loop();
  if(Timeout == 1 && stateOfSlider == 2){
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

    if(numberOfStepsCountdown == numberOfSteps){
      stepperCamera.rotate(direction * rotationCamera);
      stepperCamera.setSpeedRPM(speedOfCamera * speedFactor);
      numberOfStepsCountdown = 1;
    }else{
      numberOfStepsCountdown++;
    }
    stepperSlider.setSpeedRPM(speedOfSlider * speedFactor);
    stepperSlider.rotate(direction * rotationSlider);
    delay(1);
}else{
}
}



// Server receives Message from Client(Browser)
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Websocket disconnected!");
      con = false;
      break;

    case WStype_CONNECTED:
      Serial.print("Websocket connected to ");
      Serial.println(webSocket.remoteIP(num));
      con = true;
      break;

    case WStype_TEXT:
      // Receive request from Client
      String text = String((char *) payload);
      Serial.println((String)"client " + num + (String)": " + text);

      DynamicJsonDocument doc(128);
      deserializeJson(doc, (char *) payload);

      if (doc["action"] == "request") {
        if(!doc["wanted"].isNull()){
          if(doc["wanted"] == "STOP"){
            Timeout = Timeout * -1;
          }else if(doc["wanted"] == "slideleft"){
            driveLeft();
          }else if(doc["wanted"] == "slideright"){
            driveRight();
          }else if(doc["wanted"] == "startSettings"){
            if(stateOfSlider == 1){
              stateOfSlider = 2;
              endSettings();
            }else {
              stateOfSlider = 1;
              driveRight();
            };
            Serial.println(stateOfSlider);
          }else if(doc["wanted"] == "turnLeft"){
            turnLeft();
          }else if(doc["wanted"] == "turnRight"){
            turnRight();
          }
        }
      }

      //webSocket.sendTXT(num, payload, lenght);
      //webSocket.broadcastTXT(payload, lenght);
      break;
  }
}
