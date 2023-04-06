#include <Arduino.h>
#include <ESP32Servo.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WebSocketsServer.h>
#include <index_html.h>

const char* ssid = "moto20";
const char* password = "parque2021";

const uint8_t BASE_PIN = 15, SHOULDER_PIN = 16, ELBOW_PIN = 17;
const uint8_t VERTICAL_WRIST_PIN = 18, ROTATORY_WRIST_PIN = 19, GRIPPER_PIN = 21;



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
  //webSocket.loop();
  int elbowA = elbow.read();
  Serial.print("elbowA: ");
  Serial.println(elbowA);
  elbowA = elbowA + 3;
  Serial.print("elbowA+1: ");
  Serial.println(elbowA);
  elbow.write(elbowA);  
  delay(5);
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
    moveServo(base, -3);
  } else if (key == "d" || key == "D") {
    moveServo(base, 3);
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