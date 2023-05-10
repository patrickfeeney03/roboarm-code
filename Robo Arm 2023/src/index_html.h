#include <Arduino.h>
#pragma once

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Keypress Detection</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 0;
        }

        h1 {
            background-color: #FF6600;
            color: white;
            padding: 20px;
            text-align: center;
        }

        p {
            padding-left: 20px;
            font-size: 20px;
        }

        input[type=range] {
            -webkit-appearance: none;
            width: 90%;
            height: 15px;
            border-radius: 5px;
            background: #d3d3d3;
            outline: none;
        }

        input[type=range]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 25px;
            height: 25px;
            border-radius: 50%;
            background: #FF6600;
            cursor: pointer;
        }

        input[type=range]:focus::-webkit-slider-thumb {
            background: #FF6600;
        }

        div {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 10px;
            padding-left: 20px;
            padding-right: 20px;
            font-size: 18px;
        }

        label {
            margin-right: 10px;
        }
    </style>
    <script>
        document.addEventListener("DOMContentLoaded", function () {
            const socket = new WebSocket("ws://" + location.hostname + ":80/ws");
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

            const keyState = {};
            let animationRunning = false;

            document.addEventListener("keydown", function (event) {
                const key = event.key.toUpperCase();
                if (key === "SHIFT") {
                    keyState[key] = true;
                } else if (!keyState[key]) {
                    keyState[key] = true;
                    if (!animationRunning) {
                        animationRunning = true;
                        requestAnimationFrame(updateSliders);
                    }
                }
            });

            document.addEventListener("keyup", function (event) {
                const key = event.key.toUpperCase();
                keyState[key] = false;
            });

            const keyToSliderMapping = {
                'W': { key: 'S', delta: 1 },
                'S': { key: 'S', delta: -1 },
                'A': { key: 'B', delta: -1 },
                'D': { key: 'B', delta: 1 },
                'E': { key: 'R', delta: 1 },
                'Q': { key: 'R', delta: -1 },
                'U': { key: 'G', delta: 1 },
                'H': { key: 'G', delta: -1 },
                'I': { key: 'E', delta: 1 },
                'J': { key: 'E', delta: -1 },
                'O': { key: 'V', delta: 1 },
                'K': { key: 'V', delta: -1 },
            };
            

            function updateSlider(key, delta) {
                var slider = document.querySelector("input[data-key=" + key + "]");
                if (slider) {
                    var newValue = parseInt(slider.value) + delta;
                    slider.value = newValue;
                    console.log("value: " + slider.value);
                    if (slider.value != slider.dataset.prevValue) {
                        slider.dataset.prevValue = slider.value;
                        socket.send(key + ":" + slider.value);
                    }
                }
            }

            function updateSliders() {
                for (const key in keyState) {
                    if (keyState[key]) { // If the value of the key is true...
                        const mapping = keyToSliderMapping[key];
                        if (mapping) { // If the key is part of the keyToSliderMapping...
                            const delta = keyState["SHIFT"] ? mapping.delta * 2 : mapping.delta;
                            updateSlider(mapping.key, delta);
                        }
                    }
                }
                if (Object.values(keyState).some(Boolean)) {
                    requestAnimationFrame(updateSliders);
                } else {
                    animationRunning = false;
                }
            }
        });
    </script>
</head>
<body>
    <h1>Keypress Detection</h1>
    <p>Press the specified keys: W, S, A, D, Q, E, H, U, J, I, K, or O.</p>
    <div>
        <label for="base">Base:</label>
        <input type="range" id="base" min="0" max="180" value="90" step="1" data-key="B">
    </div>
    <div>
        <label for="shoulder">Shoulder:</label>
        <input type="range" id="shoulder" min="15" max="165" value="90" step="1" data-key="S">
    </div>
    <div>
        <label for="elbow">Elbow:</label>
        <input type="range" id="elbow" min="0" max="180" value="90" step="1" data-key="E">
    </div>
    <div>
        <label for="vertical-wrist">Vertical Wrist:</label>
        <input type="range" id="vertical-wrist" min="0" max="180" value="90" step="1" data-key="V">
    </div>
    <div>
        <label for="rotatory-wrist">Rotatory Wrist:</label>
        <input type="range" id="rotatory-wrist" min="0" max="180" value="90" step="1" data-key="R">
    </div>
    <div>
        <label for="gripper">Gripper:</label>
        <input type="range" id="gripper" min="10" max="73" value="41" step="1" data-key="G">
    </div>
</body>
</html>
)rawliteral";