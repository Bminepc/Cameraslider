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

int direction = 1; //Direction of Turning. Reversed at every End
int nextDirection = 1; //Temporary direction. Only used while deacelerating
int rotationSlider = 5; //Some Speed Modifier
int rotationCamera = 1; //Some Speed Modifier
float speedFactor = 1; //Factor for Acceleration and deacceleration
boolean hit = false; //Shows if endswitch was hit and Device is deacelerating
int Timeout = 1; //Simple Network Timeout 1 = Not Timeouted -1 = Timeouted
int millisToEnd = 0; //Time needed to travel from one Side to another in Milliseconds

//Positiondetection of the Motors
void detectBounds(){
  millisToEnd = 0;
  Serial.println("Driving to one End");
  while (digitalRead(TasterLeft) != HIGH) {
    stepperSlider.rotate(1);
  }
  Serial.println("Driving to other End");
  while (digitalRead(TasterRight) != HIGH) {
    stepperSlider.rotate(-1);
    millisToEnd++;
    delay(1);
  }
  millisToEnd = millisToEnd * 2; //Correct some... well... bullshit...
  Serial.println("Time in Milliseconds from Left to Right:");
  Serial.println(millisToEnd);
}

// Function Prototypes
void webSocketEvent(uint8_t , WStype_t, uint8_t * , size_t );
void sendResponse();

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
}

void loop(){
  webSocket.loop();
  if(Timeout == 1){
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
}else{
  //Timeoutaction
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
        if(!doc["stop"].isNull()) Timeout = Timeout * -1;
        /*
        if (!doc["dreh"].isNull()) Value = doc["dreh"];
        if (!doc["LED1"].isNull())
          setLED1(doc["LED1"]);
        bChange = true;
        */
      }

      //webSocket.sendTXT(num, payload, lenght);
      //webSocket.broadcastTXT(payload, lenght);
      break;
  }
}
