#include <Arduino.h>
#include <ESP32Servo.h>

const uint8_t BASE_PIN = 15, SHOULDER_PIN = 16, ELBOW_PIN = 17;
const uint8_t WRIST_PIN = 18, ROTATE_WRIST_PIN = 19, CLAWS_PIN = 21;

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist;
Servo rotateWrist;
Servo claws;

void setup() {
  base.attach(BASE_PIN);
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  wrist.attach(WRIST_PIN);
  rotateWrist.attach(ROTATE_WRIST_PIN);
  claws.attach(CLAWS_PIN);  
}

void loop() {
  
}