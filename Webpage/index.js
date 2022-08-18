const ip = location.hostname; // this has to be hostname since host would return an port too and would conflict with line 19
let connection = null;

// Setup EventListener
document.addEventListener("DOMContentLoaded", () => {
  const stop = document.querySelector("#stop");

  stop.addEventListener("click", () => {
    sendLED1("STOP");
  });

  start();
  setInterval(check, 1000);
});

// Setup WebSocket connection
const start = () => {
  if (connection !== null) connection = null;
  connection = new WebSocket(`ws://${ip}:81`);
  //connection = new WebSocket("ws://192.168.2.111:81");

  connection.onopen = () => {
    connection.send(
      `Message from Browser to ESP32 yay its Working!! ${new Date()}`
    );
    connection.send("ping");
    console.log("Websocket opened!");
  };

  connection.onerror = (error) => {
    console.log("WebSocket Error ", error);
  };

  connection.onclose = () => {
    console.log("Websocket closed!");
    //check();//reconnect now
  };

  connection.onmessage = (e) => {};
};

// Check WebSocket connection
const check = () => {
  if (!connection || connection.readyState === 3) start();
};

// Send new LED1 state to server
const sendLED1 = (value) => {
  if (connection === null) {
    window.alert("Keine verbindung");
    return;
  }

  // Construct a msg object containing the data the server needs to process the message from the client.
  let msg = {};
  msg.action = "request";
  msg.stop = value; // State of the LED
  connection.send(JSON.stringify(msg));
};
