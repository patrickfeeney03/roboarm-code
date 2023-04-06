#include <Arduino.h>
#include <ESP32Servo.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WebSocketsServer.h>

const char* ssid = "moto20";
const char* password = "parque2021";

const uint8_t BASE_PIN = 15, SHOULDER_PIN = 16, ELBOW_PIN = 17;
const uint8_t VERTICAL_WRIST_PIN = 18, ROTATORY_WRIST_PIN = 19, GRIPPER_PIN = 21;

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
      let countVar = 0;
      document.addEventListener("DOMContentLoaded", function () {
        document.body.addEventListener("keydown", function (event) {
          const key = event.key;

          switch (key) {
            case "w":
            case "W":
            case "s":
            case "S":
            case "a":
            case "A":
            case "d":
            case "D":
            case "q":
            case "Q":
            case "e":
            case "E":
            case "j":
            case "J":
            case "i":
            case "I":
            case "k":
            case "K":
            case "o":
            case "O":
            case "u":
            case "U":
            case "h":
            case "H":
              document.getElementById("output").innerHTML = "Pressed: " + key;
              console.log((countVar += 3));
              fetch("/keypress?key=" + key)
                .then((response) => console.log(response))
                .catch((error) => console.erro(error));
              break;
            default:
              document.getElementById("output").innerHTML =
                "Invalid key: " + key;
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

AsyncWebServer server(80);
WebSocketsServer webSocket(81);

Servo base;
Servo shoulder;
Servo elbow;
Servo verticalWrist;
Servo rotatoryWrist;
Servo gripper;

void clearSerialBuffer();
void handleKeypress(String key);
void moveServo(Servo& servo, int change);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

void setup() {
  Serial.begin(115200);

  base.attach(BASE_PIN);
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  verticalWrist.attach(VERTICAL_WRIST_PIN);
  rotatoryWrist.attach(ROTATORY_WRIST_PIN);
  gripper.attach(GRIPPER_PIN);

  base.write(90);
  shoulder.write(90);
  elbow.write(90);
  verticalWrist.write(90);
  rotatoryWrist.write(90);
  gripper.write(50);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("CONNECTED");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Inside server on '/'");
    request->send_P(200, "text/html", index_html);
  });

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  server.begin();
}

void loop() {
}

void clearSerialBuffer() {
  while (Serial.available()  > 0 ) {
    Serial.read();
  }
}

void handleKeypress(String key) {
  Serial.print("Received key: ");
  Serial.println(key);

  if (key == "w" || key == "W") {
    moveServo(shoulder, 3);

  } else if (key == "s" || key == "S") {
    moveServo(shoulder, -3);
  } else if (key == "a" || key == "A") {
    moveServo(base, 3);
  } else if (key == "d" || key == "D") {
    moveServo(base, -3);
  } else if (key == "q" || key == "Q") {
    moveServo(rotatoryWrist, -3);
  } else if (key == "e" || key == "E") {
    moveServo(rotatoryWrist, 3);
  } else if (key == "i" || key == "I") {
    moveServo(elbow, 3);
  } else if (key == "j" || key == "J") {
    moveServo(elbow, -3);
  } else if (key == "k" || key == "K") {
    moveServo(verticalWrist, -3);
  } else if (key == "o" || key == "O") {
    moveServo(verticalWrist, 3);
  } else if (key == "u" || key == "U") {
    moveServo(gripper, 3);
  } else if (key == "h" || key == "H") {
    moveServo(gripper, -3);
  }
}

void moveServo(Servo& servo, int change) {
  int currentPosition = servo.read();
  int newPosition = currentPosition + change;
  servo.write(newPosition);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String key = String((char *)payload);
    handleKeypress(key);
  }
}