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
    </head>
    <body>
      <div class="Top">
        <h1>Cameraslider</h1>
        <br>
        <h2>By DasRudeltier and Bminepc</h2>
      </div>
      <div class="Settings">
        <button type="button" name="Stop" class="Emergency">STOP</button>
      </div>
      <div class="Data">

      </div>
      <div class="Bottom">
        © Bminepc 2022
      </div>
    </body>
  </html>

)=====";
