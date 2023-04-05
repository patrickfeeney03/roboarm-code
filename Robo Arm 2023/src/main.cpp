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

void moveServos();
void clearSerialBuffer();

void setup() {
  Serial.begin(115200);

  base.attach(BASE_PIN);
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  wrist.attach(WRIST_PIN);
  rotateWrist.attach(ROTATE_WRIST_PIN);
  claws.attach(CLAWS_PIN);
}

void loop() {
  moveServos();
  Serial.println("Where even am I?");
}

void moveServos() {
  clearSerialBuffer();
  Serial.print("1 - 6 to select servo: ");
  while (Serial.available() == 0);
  uint8_t servoNum = Serial.parseInt();
  Serial.print("servoNum: ");
  Serial.println(servoNum);
  
  clearSerialBuffer();
  Serial.print("\nEnter angle: ");
  while (Serial.available() == 0);
  uint8_t angle = Serial.parseInt();
  Serial.print("Angle: ");
  Serial.println(angle);


  switch (servoNum) {
    case 1:
      base.write(angle);
      break;
    case 2:
      shoulder.write(angle);
      break;
    case 3:
      elbow.write(angle);
      break;
    case 4:
      wrist.write(angle);
      break;
    case 5:
      rotateWrist.write(angle);
      break;
    case 6:
      claws.write(angle);
      break;
    default:
      printf("Something broke\n");
  }
}

void clearSerialBuffer() {
  while (Serial.available()  > 0 ) {
    Serial.read();
  }
}