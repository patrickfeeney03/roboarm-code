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
            const socket = new WebSocket("ws://" + location.hostname + ":81/");
            socket.onopen = function (event) {
                console.log("WebSocket connection opened:", event);
            };
            socket.onclose = function (event) {
                console.log("WebSocket connection closed:", event);
            };
            socket.onerror = function (event) {
                console.error("WebSocket error:", event);
            };

            var sliders = document.querySelectorAll("input[type=range]");
            sliders.forEach(function (slider) {
                slider.addEventListener("input", function () {
                    var key = slider.getAttribute("data-key");
                    var value = slider.value;
                    socket.send(key + ":" + value);
                });
            });

            function updateSlider(key, delta) {
                var slider = document.querySelector("input[data-key=" + key + "]");
                if (slider) {
                    var newValue = parseInt(slider.value) + delta;
                    slider.value = newValue;
                    socket.send(key + ":" + slider.value);
                }
            }

            document.addEventListener("keydown", function (event) {
                var key = event.key.toUpperCase();

                switch (key) {
                    case 'W':
                        updateSlider('S', 1);
                        break;
                    case 'S':
                        updateSlider('S', -1);
                }
            });
        });
    </script>
</head>

<body>
    <h1>Keypress Detection</h1>
    <p>Press the specified keys: W, S, A, D, Q, E, J, I, K, O, U, or H.</p>
    <p id="output"></p>
    <div>
        <label>Base:</label>
        <input type="range" min="0" max="180" value="90" step="1" data-key="B">
    </div>
    <div>
        <label>Shoulder:</label>
        <input type="range" min="15" max="165" value="90" step="1" data-key="S">
    </div>
    <div>
        <label>Elbow:</label>
        <input type="range" min="0" max="180" value="90" step="1" data-key="E">
    </div>
    <div>
        <label>Vertical Wrist:</label>
        <input type="range" min="0" max="180" value="90" step="1" data-key="V">
    </div>
    <div>
        <label>Rotatory Wrist:</label>
        <input type="range" min="0" max="180" value="90" step="1" data-key="R">
    </div>
    <div>
        <label>Gripper:</label>
        <input type="range" min="10" max="73" value="50" step="1" data-key="G">
    </div>
</body>

</html>
)rawliteral";