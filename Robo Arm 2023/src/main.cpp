#include <Arduino.h>
#include <ESP32Servo.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
//#include <WebSocketsServer.h>
#include <index_html.h>
#include <WiFi.h>

const char* ssid = "moto20";
const char* password = "parque2021";

const uint8_t BASE_PIN = 15, SHOULDER_PIN = 16, ELBOW_PIN = 17;
const uint8_t VERTICAL_WRIST_PIN = 18, ROTATORY_WRIST_PIN = 19, GRIPPER_PIN = 21;

AsyncWebServer server(80);
//WebSocketsServer webSocket(81);
AsyncWebSocket ws("/ws");

Servo base;
Servo shoulder;
Servo elbow;
Servo verticalWrist;
Servo rotatoryWrist;
Servo gripper;

void moveServo(Servo& servo, int value, int lowConstrain, int highConstrain);
void handleServo(char key, int value);
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
//void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

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

  //webSocket.begin();
  //webSocket.onEvent(webSocketEvent);
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
}

void loop() {
  //webSocket.loop();
  //Serial.println("abc");
}

// For both keypress and slider
void handleServo(char key, int value) {
  Serial.print("Received: ");
  Serial.print(key);
  Serial.print("\t");
  Serial.println(value);

  if (key == 'B') {
    moveServo(base, value, 0, 180);
  }
  else if (key == 'S') {
    moveServo(shoulder, value, 15, 165);
  }
  else if (key == 'E') {
    moveServo(elbow, value, 0, 180);
  }
  else if (key == 'V') {
    moveServo(verticalWrist, value, 0, 180);
  }
  else if (key == 'R') {
    moveServo(rotatoryWrist, value, 0, 180);
  }
  else if (key == 'G') {
    moveServo(gripper, value, 10, 73);
  }
  else {
    Serial.println("Something went wrong. Currently inside handleServo()");
  }
}

void moveServo(Servo& servo, int value, int lowConstrain, int highConstrain) {
  int position = constrain(value, lowConstrain, highConstrain);
  servo.write(position);
}

/*
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String receivedData = String((char *)payload);
    char key = receivedData.charAt(0);
    int value = receivedData.substring(2).toInt();
    handleServo(key, value);
  }
}*/

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
  if(type == WS_EVT_DATA){
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT){
      String receivedData = String((char*)data);
      char key = receivedData.charAt(0);
      int value = receivedData.substring(2).toInt();
      handleServo(key, value);
    }
  }
}