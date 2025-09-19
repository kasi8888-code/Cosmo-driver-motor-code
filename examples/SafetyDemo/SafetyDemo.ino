/**
 * @file SafetyDemo.ino
 * @brief Safety features demonstration for Cosmo Motor Driver
 * @version 2.0.0
 * @date 2024
 * 
 * This example demonstrates the comprehensive safety features including:
 * - Emergency stop functionality
 * - Safety timeout protection
 * - Error handling and recovery
 * - Status monitoring
 */

#include "../../src/CosmoMotorDriver.h"

// Pin configuration
const uint8_t PIN_ENA = 5, PIN_IN1 = 7, PIN_IN2 = 8;
const uint8_t PIN_ENB = 6, PIN_IN3 = 9, PIN_IN4 = 11;

// Safety demonstration button pin (optional)
const uint8_t EMERGENCY_BUTTON_PIN = 2;

CosmoMotorDriver motors(PIN_ENA, PIN_IN1, PIN_IN2, PIN_ENB, PIN_IN3, PIN_IN4);

uint32_t lastSafetyCheck = 0;
bool demonstrationRunning = false;

void setup() {
  Serial.begin(115200);
  
  // Initialize emergency button (optional)
  pinMode(EMERGENCY_BUTTON_PIN, INPUT_PULLUP);
  
  if (motors.begin() != CosmoMotorConfig::STATUS_OK) {
    Serial.println("CRITICAL: Motor initialization failed!");
    while(1) {
      // Flash LED to indicate critical error
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(200);
    }
  }
  
  // Configure safety parameters for demonstration
  motors.setMaxSpeed(150);           // Conservative max speed
  motors.setSafetyTimeout(2000);     // Short timeout for demo
  motors.setAccelerationRate(40);    // Controlled acceleration
  
  Serial.println("=== SAFETY DEMONSTRATION READY ===");
  Serial.println("This demo shows safety features:");
  Serial.println("1. Emergency stop (hardware button or serial 'E')");
  Serial.println("2. Automatic safety timeout");
  Serial.println("3. Error detection and recovery");
  Serial.println("4. Status monitoring");
  Serial.println("Press 'S' to start demonstration");
  Serial.println("=====================================");
}

void loop() {
  // Update motor safety systems
  motors.update();
  
  // Check for emergency button press
  if (digitalRead(EMERGENCY_BUTTON_PIN) == LOW) {
    handleEmergencyStop("Hardware button pressed");
  }
  
  // Process serial commands
  processCommands();
  
  // Run safety demonstration
  if (demonstrationRunning) {
    runSafetyDemo();
  }
  
  // Periodic safety monitoring
  performSafetyChecks();
}

void handleEmergencyStop(const char* reason) {
  Serial.print("EMERGENCY STOP TRIGGERED: ");
  Serial.println(reason);
  
  CosmoMotorConfig::StatusCode status = motors.emergencyStop();
  
  if (status == CosmoMotorConfig::STATUS_EMERGENCY_STOP) {
    Serial.println("✓ Emergency stop successful");
    demonstrationRunning = false;
  } else {
    Serial.println("✗ Emergency stop failed!");
  }
  
  // Flash LED rapidly during emergency stop
  for (int i = 0; i < 20; i++) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(50);
  }
}

void processCommands() {
  if (Serial.available()) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case 'S':
      case 's':
        if (!motors.isEmergencyStopActive()) {
          demonstrationRunning = true;
          Serial.println("Starting safety demonstration...");
        } else {
          Serial.println("Cannot start - emergency stop active");
        }
        break;
        
      case 'E':
      case 'e':
        handleEmergencyStop("Serial command");
        break;
        
      case 'R':
      case 'r':
        if (motors.isEmergencyStopActive()) {
          motors.resetEmergencyStop();
          Serial.println("Emergency stop reset - system ready");
        }
        break;
        
      case 'I':
      case 'i':
        printSystemInfo();
        break;
        
      case 'H':
      case 'h':
        printHelpMenu();
        break;
    }
  }
}

void runSafetyDemo() {
  static uint32_t demoStartTime = 0;
  static uint8_t demoPhase = 0;
  static uint32_t phaseStartTime = 0;
  
  if (demoStartTime == 0) {
    demoStartTime = millis();
    phaseStartTime = millis();
    demoPhase = 0;
  }
  
  uint32_t currentTime = millis();
  
  switch (demoPhase) {
    case 0: // Normal operation
      if (currentTime - phaseStartTime < 1000) {
        Serial.println("Phase 1: Normal forward movement");
        motors.moveForward(100);
      } else {
        demoPhase = 1;
        phaseStartTime = currentTime;
      }
      break;
      
    case 1: // Demonstrate timeout
      if (currentTime - phaseStartTime < 100) {
        Serial.println("Phase 2: Testing safety timeout (no commands for 2+ seconds)");
        motors.moveForward(80);
      } else if (currentTime - phaseStartTime < 3000) {
        // Don't send commands - let timeout occur
        if (motors.checkSafetyTimeout()) {
          Serial.println("✓ Safety timeout activated - motors stopped");
          demoPhase = 2;
          phaseStartTime = currentTime;
        }
      } else {
        demoPhase = 2;
        phaseStartTime = currentTime;
      }
      break;
      
    case 2: // Recovery and status check
      Serial.println("Phase 3: Recovery and status monitoring");
      motors.moveBackward(60);
      delay(1000);
      motors.stop();
      
      uint32_t commands, errors;
      motors.getDiagnostics(commands, errors);
      Serial.print("Commands processed: "); Serial.println(commands);
      Serial.print("Errors detected: "); Serial.println(errors);
      
      Serial.println("Safety demonstration complete!");
      demonstrationRunning = false;
      demoStartTime = 0;
      break;
  }
}

void performSafetyChecks() {
  if (millis() - lastSafetyCheck >= 5000) { // Every 5 seconds
    
    // Check for timeout condition
    if (motors.checkSafetyTimeout()) {
      Serial.println("WARNING: Safety timeout detected");
    }
    
    // Check emergency stop status
    if (motors.isEmergencyStopActive()) {
      Serial.println("STATUS: Emergency stop active");
    }
    
    // Get current motor speeds
    int16_t speedA, speedB;
    motors.getCurrentSpeeds(speedA, speedB);
    
    Serial.print("Motor speeds - A: "); Serial.print(speedA);
    Serial.print(", B: "); Serial.println(speedB);
    
    lastSafetyCheck = millis();
  }
}

void printSystemInfo() {
  Serial.println("\n=== SYSTEM STATUS ===");
  motors.printStatus();
  
  Serial.print("Free RAM: ");
  Serial.print(freeRam());
  Serial.println(" bytes");
  
  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" seconds");
  
  Serial.println("====================\n");
}

void printHelpMenu() {
  Serial.println("\n=== SAFETY DEMO COMMANDS ===");
  Serial.println("S - Start safety demonstration");
  Serial.println("E - Trigger emergency stop");
  Serial.println("R - Reset emergency stop");
  Serial.println("I - Show system information");
  Serial.println("H - Show this help menu");
  Serial.println("Hardware button on pin 2 also triggers emergency stop");
  Serial.println("============================\n");
}

// Memory diagnostic function
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}