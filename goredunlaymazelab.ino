#include "MecanumRobot.h"

MecanumRobot robot(4, 3, 6, 5, 10, 9, 12, 11);
const int trigPin = 7;
const int echoPin = 8;
int turns = 0; // Global variable

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  robot.begin();
  robot.setTurnTime(1000);
  turns = 0; // Corrected: Assigning to the global variable, not redeclaring
}

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void loop() {
  float distance = getDistance();
  
  Serial.print("Start of loop. Turns: ");
  Serial.print(turns);
  Serial.print(" | Distance: ");
  Serial.println(distance);

  // Check if we have completed the course
  if (turns >= 6) {
    robot.forwardStart();
    delay(3000);
    robot.stop();
    Serial.println("Course Finished.");
    while(true); // Stop the robot indefinitely
  }

  // Navigation Logic
  if (distance < 10 && distance > 0) { // Added > 0 to ignore sensor errors
    robot.stop();
    delay(100); // Short settle time

    if (turns >= 4) {
      Serial.println("Action: Turning Left");
      robot.left(90);
    } else {
      Serial.println("Action: Turning Right");
      robot.right(80);
    }
    
    turns++; 
    delay(500); // Wait for turn to complete/sensor to clear the wall
  } else {
    robot.forwardStart();
  }

  delay(50); // Small delay for stability
}