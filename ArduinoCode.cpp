#include <Servo.h>

// --- Configuration & Pin Definitions ---
// Define which digital pins control which servo
const int PIN_SHOULDER = 9;   // Base rotation (Pin 9)
const int PIN_ELBOW = 10;     // Shoulder lift (Pin 10)
const int PIN_WRIST = 11;     // Elbow bend (Pin 11)
const int PIN_GRIPPER = 12;   // Wrist rotate/Gripper (Pin 12)

// Define servo limits (Degrees)
// Adjust these based on your specific servo model and arm mechanics
const int MIN_ANGLE = 15;
const int MAX_ANGLE = 165;

// --- Global Objects ---
Servo servoBase;     // Controls the base rotation
Servo servoShoulder; // Controls the shoulder lift
Servo servoElbow;    // Controls the elbow bend
Servo servoWrist;    // Controls the wrist/gripper

// --- State Variables ---
struct Position {
  int base;
  int shoulder;
  int elbow;
  int wrist;
};

// Store current positions
Position currentPos = {90, 90, 90, 90};
Position targetPos = {90, 90, 90, 90};

// --- Helper Functions ---

/**
 * Moves a servo from 'current' to 'target' in small steps.
 * Prevents sudden jerks that can damage the arm or strip gears.
 */
void smoothMove(Servo &servo, int &currentAngle, int targetAngle) {
  // Clamp target angle to physical limits
  if (targetAngle < MIN_ANGLE) targetAngle = MIN_ANGLE;
  if (targetAngle > MAX_ANGLE) targetAngle = MAX_ANGLE;

  int step = 1;
  if (targetAngle < currentAngle) step = -1;

  while (currentAngle != targetAngle) {
    currentAngle += step;
    servo.write(currentAngle);
    delay(15); // Control speed: higher delay = slower movement
  }
}


void homeArm() {
  targetPos.base = 90;
  targetPos.shoulder = 90;
  targetPos.elbow = 90;
  targetPos.wrist = 90;
}

/**
 * Placeholder for Inverse Kinematics.
 * Converts X, Y, Z coordinates into servo angles.
 * TODO: 
 */
void calculateIK(float x, float y, float z) {
 
  targetPos.base = (int)(x * 10) + 90; 
  targetPos.shoulder = (int)(y * 10) + 90;
  targetPos.elbow = (int)(z * 10) + 90;
  
  // Ensure we don't exceed limits before moving
  if(targetPos.base < MIN_ANGLE) targetPos.base = MIN_ANGLE;
  if(targetPos.base > MAX_ANGLE) targetPos.base = MAX_ANGLE;
  // ... repeat for other axes
}

void updateArm() {
  // Move Base
  smoothMove(servoBase, currentPos.base, targetPos.base);
  
  // Move Shoulder
  smoothMove(servoShoulder, currentPos.shoulder, targetPos.shoulder);
  
  // Move Elbow
  smoothMove(servoElbow, currentPos.elbow, targetPos.elbow);
  
  // Move Wrist/Gripper
  smoothMove(servoWrist, currentPos.wrist, targetPos.wrist);
}

// --- Setup & Loop ---

void setup() {
  Serial.begin(9600);
  Serial.println("Robot Arm System Initialized...");

  // Attach servos to pins
  servoBase.attach(PIN_SHOULDER);
  servoShoulder.attach(PIN_ELBOW);
  servoElbow.attach(PIN_WRIST);
  servoWrist.attach(PIN_GRIPPER);

  // Initial safety: Move to home position
  Serial.println("Moving to Home Position...");
  homeArm();
  updateArm();
  
  Serial.println("System Ready. Waiting for commands...");
  Serial.println("Send 'h' to home, or coordinate data (X,Y,Z) to move.");
}

void loop() {
  // 1. Input Handling (Serial Monitor or Sensors)
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    if (command == 'h') {
      homeArm();
      updateArm();
      Serial.println("Returning to Home.");
    } 
    else if (command == 'r') {
      // Placeholder for "Record" mode
      Serial.println("Record mode active (Not implemented in placeholder).");
    }
    else if (isdigit(command)) {
      // Placeholder for parsing coordinate strings
      // In a real app, you'd parse "10,20,30" from the buffer
      Serial.println("Coordinate parsing not fully implemented.");
    }
  }


  if (currentPos.base != targetPos.base || 
      currentPos.shoulder != targetPos.shoulder || 
      currentPos.elbow != targetPos.elbow || 
      currentPos.wrist != targetPos.wrist) {
    
    updateArm();
  }

  delay(50); // Small delay to stabilize the loop
}