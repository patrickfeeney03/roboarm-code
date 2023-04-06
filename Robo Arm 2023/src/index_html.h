#include <Arduino.h>
#pragma once

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <title>Keypress Detection</title>
    <style>
        body {
            font-family: Arial, sans-serif;
        }
    </style>
    <script>
        document.addEventListener("DOMContentLoaded", function () {
            const socket = new WebSocket("ws://" + location.hostname + ":81/ws");

            socket.onopen = function (event) {
                console.log("WebSocket connection opened:", event);
            };

            socket.onclose = function (event) {
                console.log("WebSocket connection closed:", event);
            };

            socket.onerror = function (event) {
                console.error("WebSocket error:", event);
            };

            let keysPressed = {};
            let intervalID;

            function startSendingCommands() {
                if (intervalID) {
                    clearInterval(intervalID);
                }

                intervalID = setInterval(() => {
                    for (let key in keysPressed) {
                        if (keysPressed[key]) {
                            if (socket.readyState === WebSocket.OPEN) {
                                socket.send(key);
                                console.log("key sent: " + key);
                            } else {
                                console.error("WebSocket is not open. Cannot send data.");
                            }
                        }
                    }
                }, 80); // You can adjust this value to change the interval between sending commands
            }

            document.body.addEventListener("keydown", function (event) {
                const key = event.key;

                if (!["w", "W", "s", "S", "a", "A", "d", "D", "q", "Q", "e", "E", "j", "J", "i", "I", "k", "K", "o", "O", "u", "U", "h", "H"].includes(key)) {
                    document.getElementById("output").innerHTML = "Invalid key: " + key;
                    return;
                }

                if (!keysPressed[key]) {
                    keysPressed[key] = true;
                    startSendingCommands();
                }
            });

            document.body.addEventListener("keyup", function (event) {
                const key = event.key;

                if (keysPressed[key]) {
                    keysPressed[key] = false;
                    document.getElementById("output").innerHTML = "Pressed: " + key;
                }
            });
        });
    </script>
</head>
<body>
    <h1>Keypress Detection</h1>
    <p>Press the specified keys: W, S, A, D, Q, E, J, I, K, O, U, or H.</p>
    <p id="output"></p>
</body>
</html>
)rawliteral";