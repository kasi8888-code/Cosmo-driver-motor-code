/**
 * @file AdvancedControl.ino
 * @brief Advanced motor control features demonstration
 * @version 2.0.0
 * @date 2024
 * 
 * This example demonstrates advanced features of the Cosmo Motor Driver:
 * - Individual motor control
 * - Speed profiling
 * - Safety features
 * - Diagnostics
 */

#include "../../src/CosmoMotorDriver.h"

// Pin configuration
const uint8_t PIN_ENA = 5, PIN_IN1 = 7, PIN_IN2 = 8;
const uint8_t PIN_ENB = 6, PIN_IN3 = 9, PIN_IN4 = 11;

CosmoMotorDriver motors(PIN_ENA, PIN_IN1, PIN_IN2, PIN_ENB, PIN_IN3, PIN_IN4);

void setup() {
  Serial.begin(115200);
  
  if (motors.begin() != CosmoMotorConfig::STATUS_OK) {
    Serial.println("Initialization failed!");
    while(1);
  }
  
  // Configure advanced settings
  motors.setMaxSpeed(200);           // Limit maximum speed
  motors.setAccelerationRate(30);    // Slow acceleration
  motors.setSafetyTimeout(3000);     // 3-second timeout
  
  Serial.println("Advanced Control Example Ready");
}

void loop() {
  // Demonstrate individual motor control
  Serial.println("Individual Motor Control");
  motors.setMotorSpeeds(100, 150);   // Left slower than right
  delay(2000);
  
  motors.setMotorSpeeds(-80, 80);    // Pivot turn
  delay(1500);
  
  // Speed ramping demonstration
  Serial.println("Speed Ramping");
  for (int speed = 50; speed <= 180; speed += 10) {
    motors.moveForward(speed);
    delay(200);
  }
  
  for (int speed = 180; speed >= 50; speed -= 10) {
    motors.moveForward(speed);
    delay(200);
  }
  
  motors.stop();
  
  // Display diagnostics
  uint32_t commands, errors;
  motors.getDiagnostics(commands, errors);
  Serial.print("Commands: "); Serial.print(commands);
  Serial.print(", Errors: "); Serial.println(errors);
  
  delay(3000);
}