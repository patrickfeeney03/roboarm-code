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
            const updateInterval = 1000 / 30; // 30 fps
            let lastSliderUpdate = 0;
            sliders.forEach(function (slider) {
                slider.addEventListener("input", function () {
                    const timeNow = performance.now();
                    if (timeNow - lastSliderUpdate >= updateInterval) {
                        console.log(timeNow - lastSliderUpdate);
                        var key = slider.getAttribute("data-key");
                        var value = slider.value;
                        socket.send(key + ":" + value);
                        lastSliderUpdate = timeNow;
                    }                    
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
                'Q': { key: 'R', delta: 1 },
                'E': { key: 'R', delta: -1 },
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
                    //console.log("newValue:" + newValue);
                    //console.log("prevValue: " + slider.dataset.prevValue);
                    slider.value = newValue;
                    console.log("value: " + slider.value);
                    if (slider.value != slider.dataset.prevValue) {
                        slider.dataset.prevValue = slider.value;
                        socket.send(key + ":" + slider.value);
                    }
                }
            }

            let lastKeyUpdate = 0;

            function updateSliders(timestamp) {
                if (timestamp - lastKeyUpdate > updateInterval) {
                    lastKeyUpdate = timestamp;
                    for (const key in keyState) {
                        if (keyState[key]) {
                            const mapping = keyToSliderMapping[key];
                            if (mapping) {
                                const delta = keyState["SHIFT"] ? mapping.delta * 3 : mapping.delta;
                                updateSlider(mapping.key, delta);
                            }

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