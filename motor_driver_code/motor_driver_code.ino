
/**
 * @file motor_driver_code.ino
 * @brief Professional Cosmo Robot Motor Driver - Main Application
 * @version 2.0.0
 * @date 2024
 * @author Cosmo Robot Team
 * @copyright MIT License
 * 
 * This is the main application demonstrating the professional Cosmo Motor Driver library.
 * It showcases advanced motor control features, safety mechanisms, and professional
 * coding practices for competitive robotics applications.
 * 
 * Hardware Requirements:
 * - Arduino Uno/Nano/Mega or compatible
 * - Dual H-Bridge Motor Driver (L298N, L293D, TB6612FNG, etc.)
 * - 2x DC Motors with appropriate voltage rating
 * - External power supply for motors
 * 
 * Pin Configuration (Customizable):
 * - ENA (Pin 5): Motor A PWM Speed Control - MUST be PWM capable (~)
 * - IN1 (Pin 7): Motor A Direction Control 1
 * - IN2 (Pin 8): Motor A Direction Control 2  
 * - ENB (Pin 6): Motor B PWM Speed Control - MUST be PWM capable (~)
 * - IN3 (Pin 9): Motor B Direction Control 1
 * - IN4 (Pin 11): Motor B Direction Control 2
 */

#include "src/CosmoMotorDriver.h"

// Hardware Configuration - Update these pins based on your wiring
const uint8_t PIN_ENA = 5;   // Motor A PWM (must be PWM pin ~)
const uint8_t PIN_IN1 = 7;   // Motor A Direction 1
const uint8_t PIN_IN2 = 8;   // Motor A Direction 2
const uint8_t PIN_ENB = 6;   // Motor B PWM (must be PWM pin ~)
const uint8_t PIN_IN3 = 9;   // Motor B Direction 1
const uint8_t PIN_IN4 = 11;  // Motor B Direction 2

// Speed Configuration Constants
const uint8_t SPEED_FAST = 200;      // High-speed movement (0-255)
const uint8_t SPEED_MEDIUM = 150;    // Medium-speed movement (0-255)
const uint8_t SPEED_SLOW = 100;      // Low-speed precision movement (0-255)
const uint8_t SPEED_CRAWL = 50;      // Very slow crawl speed (0-255)

// Timing Configuration
const uint16_t DEMO_MOVEMENT_TIME = 2000;   // Movement duration in ms
const uint16_t DEMO_TURN_TIME = 1500;       // Turn duration in ms
const uint16_t DEMO_PAUSE_TIME = 3000;      // Pause between cycles in ms
const uint16_t STATUS_REPORT_INTERVAL = 10000; // Status report every 10s

// Create motor driver instance with professional configuration
CosmoMotorDriver motorDriver(PIN_ENA, PIN_IN1, PIN_IN2, PIN_ENB, PIN_IN3, PIN_IN4);

// System state variables
uint32_t lastStatusReport = 0;
uint32_t demoStepStartTime = 0;
uint8_t currentDemoStep = 0;
bool demoMode = true;

/**
 * @brief Arduino setup function - Initialize motor driver and system
 */
void setup() {
  // Initialize serial communication for diagnostics
  Serial.begin(115200);
  while (!Serial && millis() < 3000); // Wait for serial or timeout
  
  Serial.println(F("\n" "==================================="));
  Serial.println(F("  COSMO ROBOT MOTOR DRIVER v2.0"));
  Serial.println(F("  Professional Competition Grade"));
  Serial.println(F("==================================="));
  
  // Initialize motor driver with error checking
  CosmoMotorConfig::StatusCode initStatus = motorDriver.begin();
  if (initStatus != CosmoMotorConfig::STATUS_OK) {
    Serial.print(F("ERROR: Motor driver initialization failed with code: "));
    Serial.println(initStatus);
    while(1) {
      // Flash LED or beep to indicate error
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(500);
    }
  }
  
  // Configure motor driver parameters
  motorDriver.setMaxSpeed(255);           // Allow full speed range
  motorDriver.setAccelerationRate(75);    // Moderate acceleration
  motorDriver.setSafetyTimeout(5000);     // 5-second safety timeout
  
  // Initialize built-in LED for status indication
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Indicate ready state
  
  Serial.println(F("System initialized successfully!"));
  Serial.println(F("Starting professional demo sequence..."));
  Serial.println(F("Send 'h' for help, 'q' to quit demo mode\n"));
  
  demoStepStartTime = millis();
  lastStatusReport = millis();
}

/**
 * @brief Arduino main loop - Professional demo and command processing
 */
void loop() {
  // Update motor driver (handles safety timeouts and diagnostics)
  motorDriver.update();
  
  // Process serial commands
  processSerialCommands();
  
  // Run demo sequence if enabled
  if (demoMode) {
    runProfessionalDemo();
  }
  
  // Periodic status reporting
  if (millis() - lastStatusReport >= STATUS_REPORT_INTERVAL) {
    reportSystemStatus();
    lastStatusReport = millis();
  }
  
  // Blink LED to show system is alive
  if ((millis() / 1000) % 2 == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

/**
 * @brief Execute professional demonstration sequence
 */
void runProfessionalDemo() {
  static const char* stepNames[] = {
    "Forward Fast", "Backward Medium", "Turn Right", "Turn Left", 
    "Figure-8 Demo", "Speed Ramp", "Emergency Stop Test", "Pause"
  };
  
  uint32_t currentTime = millis();
  uint32_t stepDuration = 0;
  
  switch (currentDemoStep) {
    case 0: // Forward movement
      stepDuration = DEMO_MOVEMENT_TIME;
      if (currentTime - demoStepStartTime < 100) { // First execution
        Serial.print(F("Demo Step: "));
        Serial.println(stepNames[currentDemoStep]);
        motorDriver.moveForward(SPEED_FAST);
      }
      break;
      
    case 1: // Backward movement
      stepDuration = DEMO_MOVEMENT_TIME;
      if (currentTime - demoStepStartTime < 100) {
        Serial.print(F("Demo Step: "));
        Serial.println(stepNames[currentDemoStep]);
        motorDriver.moveBackward(SPEED_MEDIUM);
      }
      break;
      
    case 2: // Right turn
      stepDuration = DEMO_TURN_TIME;
      if (currentTime - demoStepStartTime < 100) {
        Serial.print(F("Demo Step: "));
        Serial.println(stepNames[currentDemoStep]);
        motorDriver.turnRight(SPEED_FAST);
      }
      break;
      
    case 3: // Left turn
      stepDuration = DEMO_TURN_TIME;
      if (currentTime - demoStepStartTime < 100) {
        Serial.print(F("Demo Step: "));
        Serial.println(stepNames[currentDemoStep]);
        motorDriver.turnLeft(SPEED_FAST);
      }
      break;
      
    case 4: // Advanced figure-8 demonstration
      stepDuration = 6000; // 6 seconds for complete figure-8
      executeFigure8Demo(currentTime - demoStepStartTime);
      break;
      
    case 5: // Speed ramping demonstration
      stepDuration = 4000; // 4 seconds for speed ramp
      executeSpeedRampDemo(currentTime - demoStepStartTime);
      break;
      
    case 6: // Emergency stop test
      stepDuration = 2000;
      if (currentTime - demoStepStartTime < 100) {
        Serial.println(F("Demo Step: Emergency Stop Test"));
        motorDriver.moveForward(SPEED_FAST);
      } else if (currentTime - demoStepStartTime > 500 && currentTime - demoStepStartTime < 600) {
        motorDriver.emergencyStop();
      } else if (currentTime - demoStepStartTime > 1500) {
        motorDriver.resetEmergencyStop();
      }
      break;
      
    case 7: // Pause
      stepDuration = DEMO_PAUSE_TIME;
      if (currentTime - demoStepStartTime < 100) {
        Serial.print(F("Demo Step: "));
        Serial.println(stepNames[currentDemoStep]);
        motorDriver.stop();
      }
      break;
  }
  
  // Advance to next step when duration expires
  if (currentTime - demoStepStartTime >= stepDuration) {
    currentDemoStep = (currentDemoStep + 1) % 8;
    demoStepStartTime = currentTime;
  }
}

/**
 * @brief Execute figure-8 movement pattern
 * @param elapsedTime Time elapsed since start of figure-8
 */
void executeFigure8Demo(uint32_t elapsedTime) {
  static bool initialized = false;
  if (!initialized) {
    Serial.println(F("Demo Step: Advanced Figure-8 Pattern"));
    initialized = true;
  }
  
  // Create smooth figure-8 pattern using individual motor control
  float phase = (float)elapsedTime / 6000.0 * 4 * PI; // Two complete loops
  int16_t leftSpeed = SPEED_MEDIUM + sin(phase) * 50;
  int16_t rightSpeed = SPEED_MEDIUM + sin(phase + PI/2) * 50;
  
  motorDriver.setMotorSpeeds(leftSpeed, rightSpeed);
  
  if (elapsedTime >= 5900) initialized = false; // Reset for next time
}

/**
 * @brief Execute speed ramping demonstration
 * @param elapsedTime Time elapsed since start of speed ramp
 */
void executeSpeedRampDemo(uint32_t elapsedTime) {
  static bool initialized = false;
  if (!initialized) {
    Serial.println(F("Demo Step: Speed Ramping Profile"));
    initialized = true;
  }
  
  // Smooth acceleration and deceleration
  uint8_t speed;
  if (elapsedTime < 1000) {
    // Accelerate
    speed = map(elapsedTime, 0, 1000, SPEED_CRAWL, SPEED_FAST);
  } else if (elapsedTime < 2000) {
    // Constant high speed
    speed = SPEED_FAST;
  } else if (elapsedTime < 3000) {
    // Decelerate
    speed = map(elapsedTime, 2000, 3000, SPEED_FAST, SPEED_CRAWL);
  } else {
    // Final low speed
    speed = SPEED_CRAWL;
  }
  
  motorDriver.moveForward(speed);
  
  if (elapsedTime >= 3900) initialized = false; // Reset for next time
}

/**
 * @brief Process serial commands for manual control
 */
void processSerialCommands() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    switch (command) {
      case 'h': // Help
      case 'H':
        printHelpMenu();
        break;
        
      case 'q': // Quit demo mode
      case 'Q':
        demoMode = !demoMode;
        Serial.print(F("Demo mode: "));
        Serial.println(demoMode ? F("ON") : F("OFF"));
        if (!demoMode) motorDriver.stop();
        break;
        
      case 'w': // Forward
        if (!demoMode) motorDriver.moveForward(SPEED_MEDIUM);
        break;
        
      case 's': // Backward
        if (!demoMode) motorDriver.moveBackward(SPEED_MEDIUM);
        break;
        
      case 'a': // Left
        if (!demoMode) motorDriver.turnLeft(SPEED_MEDIUM);
        break;
        
      case 'd': // Right
        if (!demoMode) motorDriver.turnRight(SPEED_MEDIUM);
        break;
        
      case 'x': // Stop
        if (!demoMode) motorDriver.stop();
        break;
        
      case 'e': // Emergency stop
        motorDriver.emergencyStop();
        Serial.println(F("Emergency stop activated!"));
        break;
        
      case 'r': // Reset emergency stop
        motorDriver.resetEmergencyStop();
        Serial.println(F("Emergency stop reset"));
        break;
        
      case 'i': // Status info
        motorDriver.printStatus();
        break;
        
      default:
        // Ignore unknown commands
        break;
    }
  }
}

/**
 * @brief Print help menu
 */
void printHelpMenu() {
  Serial.println(F("\n=== COSMO MOTOR DRIVER COMMANDS ==="));
  Serial.println(F("q/Q - Toggle demo mode"));
  Serial.println(F("w   - Move forward (manual mode)"));
  Serial.println(F("s   - Move backward (manual mode)"));
  Serial.println(F("a   - Turn left (manual mode)"));
  Serial.println(F("d   - Turn right (manual mode)"));
  Serial.println(F("x   - Stop motors (manual mode)"));
  Serial.println(F("e   - Emergency stop"));
  Serial.println(F("r   - Reset emergency stop"));
  Serial.println(F("i   - Show status information"));
  Serial.println(F("h   - Show this help menu"));
  Serial.println(F("=====================================\n"));
}

/**
 * @brief Report system status and diagnostics
 */
void reportSystemStatus() {
  Serial.println(F("\n=== PERIODIC STATUS REPORT ==="));
  
  uint32_t commandCount, errorCount;
  motorDriver.getDiagnostics(commandCount, errorCount);
  
  Serial.print(F("Runtime: "));
  Serial.print(millis() / 1000);
  Serial.println(F(" seconds"));
  
  Serial.print(F("Commands processed: "));
  Serial.println(commandCount);
  
  Serial.print(F("Error count: "));
  Serial.println(errorCount);
  
  Serial.print(F("Demo mode: "));
  Serial.println(demoMode ? F("Active") : F("Manual"));
  
  Serial.print(F("Emergency stop: "));
  Serial.println(motorDriver.isEmergencyStopActive() ? F("ACTIVE") : F("Normal"));
  
  Serial.print(F("Free RAM: "));
  Serial.print(freeMemory());
  Serial.println(F(" bytes"));
  
  Serial.println(F("===============================\n"));
}

/**
 * @brief Calculate free memory (diagnostic function)
 * @return Available RAM in bytes
 */
int freeMemory() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}