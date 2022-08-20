const char html[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html lang="de" dir="ltr">
    <head>
      <meta charset="utf-8">
      <title>Cameraslider</title>
      <style>
      body{
        background-color: black;
        color: lightgreen;
        text-align: center;
      }

      .Bottom{
        background-color: gray;
        margin-bottom: 0px;
        color: black;
      }

      .Emergency{
        background-color: red;
        color: white;
      }
      </style>
      <script language="JavaScript">
  'use strict'
  var ip = location.host;
  var connection = null;

  // Setup EventListener
  document.addEventListener('DOMContentLoaded', function() {
    const stop = document.querySelector("#stop");
    const slideleft = document.querySelector("#slideleft");
    const slideright = document.querySelector("#slideright");
    const startSettings = document.querySelector("#startSettings");
    const turnRight = document.querySelector("#turnRight");
    const turnLeft = document.querySelector("#turnLeft");

    stop.addEventListener("click", function () {
      sendLED1('STOP');
    });

    slideleft.addEventListener("click", function () {
      sendLED1('slideleft');
    });

    slideright.addEventListener("click", function () {
      sendLED1('slideright');
    });

    startSettings.addEventListener("click", function () {
      sendLED1('startSettings');
    });

    turnLeft.addEventListener("click", function () {
      sendLED1('turnLeft');
    });

    turnRight.addEventListener("click", function () {
      sendLED1('turnRight');
    });

    start();
    setInterval(check, 1000);

    });

  // Setup WebSocket connection
  function start(){
    if (connection != null)
      connection = null;;
    connection=new WebSocket("ws://"+ip+":81");
    //connection=new WebSocket("ws://192.168.2.111:81");

    connection.onopen = function(){
      connection.send('Message from Browser to ESP32 yay its Working!! ' + new Date());
      connection.send('ping');
      console.log('Websocket opened!');
    };

    connection.onerror = function(error){
      console.log('WebSocket Error ', error);
    };

    connection.onclose = function(){
      console.log('Websocket closed!');
      //check();//reconnect now
    };

    connection.onmessage = function(e){
    };
  }

  // Check WebSocket connection
  function check(){
    if(!connection || connection.readyState == 3)
      start();
  }


  // Send new LED1 state to server
  function sendLED1(Value)
  {
    if (connection == null){
      window.alert("Keine verbindung");
      return;
    }

    // Construct a msg object containing the data the server needs to process the message from the client.
    var msg = { };
    msg.action = "request";
    msg.wanted = Value;
    connection.send(JSON.stringify(msg));
  }
  </script>
    </head>
    <body>
      <div class="Top">
        <h1>Cameraslider</h1>
        <br>
        <h2>By DasRudeltier and Bminepc</h2>
      </div>
      <div class="Settings">
        <button name="Stop" class="Emergency" id="stop">STOP</button>
        <div class="Outerbox">
          <button class="Movement" id="slideleft">Slide Left</button>
          <button class="Movement" id="slideright">Slide Right</button>
        </div>
        <hr>
        <div class="Outerbox">
          <button class="Movement" id="turnLeft">Turn Left</button>
          <button class="Movement" id="turnRight">Turn Right</button>
        </div>
        <button class="General" id="startSettings">Start or end Settingsmode</button>
      </div>
      <div class="Data">

      </div>
      <div class="Bottom">
        © Bminepc & DasRudeltier 2022
      </div>
    </body>
  </html>
)=====";
