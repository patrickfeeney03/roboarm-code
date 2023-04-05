#include <Arduino.h>
#include <ESP32Servo.h>

const uint8_t BASE_PIN = 15, SHOULDER_PIN = 16, ELBOW_PIN = 17;
const uint8_t VERTICAL_WRIST_PIN = 18, ROTATORY_WRIST_PIN = 19, GRIPPER_PIN = 21;

Servo base;
Servo shoulder;
Servo elbow;
Servo verticalWrist;
Servo rotatoryWrist;
Servo gripper;

void moveServos();
void clearSerialBuffer();
void moveForward(uint8_t stepSize);

void setup() {
  Serial.begin(115200);

  base.attach(BASE_PIN);
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  verticalWrist.attach(VERTICAL_WRIST_PIN);
  rotatoryWrist.attach(ROTATORY_WRIST_PIN);
  gripper.attach(GRIPPER_PIN);

  shoulder.write(90);
  elbow.write(90);
  verticalWrist.write(90);
}

void loop() {
  clearSerialBuffer();
  Serial.println("Press r to run");
  while (Serial.available() == 0);
  uint8_t userInput = Serial.read();
  Serial.print("Received: ");
  Serial.println((char)userInput);

  delay(50);

  if (userInput == 'r') {
    Serial.println("Moving arm forward");
    moveForward(5);
  }

  delay(300);
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
      verticalWrist.write(angle);
      break;
    case 5:
      rotatoryWrist.write(angle);
      break;
    case 6:
      gripper.write(angle);
      break;
    default:
      printf("Something broke\n\n");
  }
}

void clearSerialBuffer() {
  while (Serial.available()  > 0 ) {
    Serial.read();
  }
}

void moveForward(uint8_t stepSize) {
  Serial.println("Entering moveForward function");
  
  int shoulderAngle = shoulder.read();
  int elbowAngle = elbow.read();
  int verticalWristAngle = verticalWrist.read();

  Serial.print("Initial angles: shoulder=");
  Serial.print(shoulderAngle);
  Serial.print(", elbow=");
  Serial.print(elbowAngle);
  Serial.print(", verticalWrist=");
  Serial.println(verticalWristAngle);

  shoulderAngle += stepSize;
  elbowAngle += stepSize;
  verticalWristAngle += stepSize;

  shoulderAngle = constrain(shoulderAngle, 15, 165);
  elbowAngle = constrain(elbowAngle, 0, 180);
  verticalWristAngle = constrain(verticalWristAngle, 0, 180);

  Serial.print("Updated angles: shoulder=");
  Serial.print(shoulderAngle);
  Serial.print(", elbow=");
  Serial.print(elbowAngle);
  Serial.print(", verticalWrist=");
  Serial.println(verticalWristAngle);

  shoulder.write(shoulderAngle);
  elbow.write(elbowAngle);
  verticalWrist.write(verticalWristAngle);

  Serial.println("Exiting moveForward function");
}