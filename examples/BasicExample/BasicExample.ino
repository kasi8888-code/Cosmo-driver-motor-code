/**
 * @file BasicExample.ino
 * @brief Basic usage example for Cosmo Motor Driver
 * @version 2.0.0
 * @date 2024
 * 
 * This example demonstrates the simplest way to use the Cosmo Motor Driver library
 * for basic robot movement control.
 */

#include "../../src/CosmoMotorDriver.h"

// Pin configuration - adjust for your hardware
const uint8_t PIN_ENA = 5;   // Motor A PWM
const uint8_t PIN_IN1 = 7;   // Motor A Direction 1
const uint8_t PIN_IN2 = 8;   // Motor A Direction 2
const uint8_t PIN_ENB = 6;   // Motor B PWM
const uint8_t PIN_IN3 = 9;   // Motor B Direction 1
const uint8_t PIN_IN4 = 11;  // Motor B Direction 2

// Create motor driver instance
CosmoMotorDriver motors(PIN_ENA, PIN_IN1, PIN_IN2, PIN_ENB, PIN_IN3, PIN_IN4);

void setup() {
  Serial.begin(115200);
  
  // Initialize motor driver
  if (motors.begin() != CosmoMotorConfig::STATUS_OK) {
    Serial.println("Motor initialization failed!");
    while(1);
  }
  
  Serial.println("Basic Cosmo Motor Driver Example");
}

void loop() {
  // Simple movement sequence
  Serial.println("Forward");
  motors.moveForward(150);
  delay(2000);
  
  Serial.println("Backward");
  motors.moveBackward(100);
  delay(2000);
  
  Serial.println("Turn Right");
  motors.turnRight(120);
  delay(1000);
  
  Serial.println("Turn Left");
  motors.turnLeft(120);
  delay(1000);
  
  Serial.println("Stop");
  motors.stop();
  delay(2000);
}