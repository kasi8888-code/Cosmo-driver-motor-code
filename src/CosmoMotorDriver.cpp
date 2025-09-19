/**
 * @file CosmoMotorDriver.cpp
 * @brief Implementation of professional motor driver library
 * @version 2.0.0
 * @date 2024
 * @author Cosmo Robot Team
 * @copyright MIT License
 */

#include "CosmoMotorDriver.h"

// Constructor
CosmoMotorDriver::CosmoMotorDriver(uint8_t pinENA, uint8_t pinIN1, uint8_t pinIN2,
                                   uint8_t pinENB, uint8_t pinIN3, uint8_t pinIN4)
    : _pinENA(pinENA), _pinIN1(pinIN1), _pinIN2(pinIN2),
      _pinENB(pinENB), _pinIN3(pinIN3), _pinIN4(pinIN4),
      _currentSpeedA(0), _currentSpeedB(0),
      _lastCommandTime(0), _emergencyStopActive(false),
      _initialized(false), _maxSpeed(CosmoMotorConfig::PWM_MAX),
      _accelerationRate(CosmoMotorConfig::DEFAULT_ACCELERATION),
      _safetyTimeout(CosmoMotorConfig::SAFETY_TIMEOUT_MS),
      _commandCount(0), _errorCount(0) {
}

// Initialize the motor driver
CosmoMotorConfig::StatusCode CosmoMotorDriver::begin() {
    // Validate pin assignments
    if (!validatePins()) {
        _errorCount++;
        return CosmoMotorConfig::STATUS_INVALID_PIN;
    }
    
    // Configure pins as outputs
    pinMode(_pinENA, OUTPUT);
    pinMode(_pinIN1, OUTPUT);
    pinMode(_pinIN2, OUTPUT);
    pinMode(_pinENB, OUTPUT);
    pinMode(_pinIN3, OUTPUT);
    pinMode(_pinIN4, OUTPUT);
    
    // Initialize in stopped state
    stop();
    
    // Reset diagnostic counters
    _commandCount = 0;
    _errorCount = 0;
    _lastCommandTime = millis();
    _emergencyStopActive = false;
    _initialized = true;
    
    // Print initialization message
    Serial.println(F("=== Cosmo Motor Driver v2.0 Initialized ==="));
    Serial.print(F("Build: "));
    Serial.print(CosmoMotorConfig::BUILD_DATE);
    Serial.print(F(" "));
    Serial.println(CosmoMotorConfig::BUILD_TIME);
    printStatus();
    
    return CosmoMotorConfig::STATUS_OK;
}

// Move forward
CosmoMotorConfig::StatusCode CosmoMotorDriver::moveForward(uint8_t speed) {
    if (!validateSpeed(speed)) {
        _errorCount++;
        return CosmoMotorConfig::STATUS_INVALID_SPEED;
    }
    
    return executeMovement(speed, speed);
}

// Move backward
CosmoMotorConfig::StatusCode CosmoMotorDriver::moveBackward(uint8_t speed) {
    if (!validateSpeed(speed)) {
        _errorCount++;
        return CosmoMotorConfig::STATUS_INVALID_SPEED;
    }
    
    return executeMovement(-speed, -speed);
}

// Turn left
CosmoMotorConfig::StatusCode CosmoMotorDriver::turnLeft(uint8_t speed) {
    if (!validateSpeed(speed)) {
        _errorCount++;
        return CosmoMotorConfig::STATUS_INVALID_SPEED;
    }
    
    // Left turn: left motor backward, right motor forward
    return executeMovement(-speed, speed);
}

// Turn right
CosmoMotorConfig::StatusCode CosmoMotorDriver::turnRight(uint8_t speed) {
    if (!validateSpeed(speed)) {
        _errorCount++;
        return CosmoMotorConfig::STATUS_INVALID_SPEED;
    }
    
    // Right turn: left motor forward, right motor backward
    return executeMovement(speed, -speed);
}

// Stop motors
CosmoMotorConfig::StatusCode CosmoMotorDriver::stop() {
    return executeMovement(0, 0);
}

// Emergency stop
CosmoMotorConfig::StatusCode CosmoMotorDriver::emergencyStop() {
    _emergencyStopActive = true;
    
    // Immediately stop all motors
    digitalWrite(_pinIN1, LOW);
    digitalWrite(_pinIN2, LOW);
    digitalWrite(_pinIN3, LOW);
    digitalWrite(_pinIN4, LOW);
    analogWrite(_pinENA, 0);
    analogWrite(_pinENB, 0);
    
    _currentSpeedA = 0;
    _currentSpeedB = 0;
    updateLastCommandTime();
    _commandCount++;
    
    Serial.println(F("*** EMERGENCY STOP ACTIVATED ***"));
    
    return CosmoMotorConfig::STATUS_EMERGENCY_STOP;
}

// Set individual motor speeds
CosmoMotorConfig::StatusCode CosmoMotorDriver::setMotorSpeeds(int16_t speedA, int16_t speedB) {
    if (!validateSpeed(speedA) || !validateSpeed(speedB)) {
        _errorCount++;
        return CosmoMotorConfig::STATUS_INVALID_SPEED;
    }
    
    return executeMovement(speedA, speedB);
}

// Set maximum speed
void CosmoMotorDriver::setMaxSpeed(uint8_t maxSpeed) {
    _maxSpeed = constrain(maxSpeed, CosmoMotorConfig::PWM_MIN, CosmoMotorConfig::PWM_MAX);
    Serial.print(F("Max speed set to: "));
    Serial.println(_maxSpeed);
}

// Set acceleration rate
void CosmoMotorDriver::setAccelerationRate(uint8_t rate) {
    _accelerationRate = constrain(rate, 1, CosmoMotorConfig::PWM_MAX);
    Serial.print(F("Acceleration rate set to: "));
    Serial.println(_accelerationRate);
}

// Set safety timeout
void CosmoMotorDriver::setSafetyTimeout(uint16_t timeoutMs) {
    _safetyTimeout = timeoutMs;
    Serial.print(F("Safety timeout set to: "));
    Serial.print(_safetyTimeout);
    Serial.println(F(" ms"));
}

// Get current speeds
void CosmoMotorDriver::getCurrentSpeeds(int16_t& speedA, int16_t& speedB) {
    speedA = _currentSpeedA;
    speedB = _currentSpeedB;
}

// Get diagnostics
void CosmoMotorDriver::getDiagnostics(uint32_t& commandCount, uint32_t& errorCount) {
    commandCount = _commandCount;
    errorCount = _errorCount;
}

// Check emergency stop status
bool CosmoMotorDriver::isEmergencyStopActive() {
    return _emergencyStopActive;
}

// Reset emergency stop
void CosmoMotorDriver::resetEmergencyStop() {
    if (_emergencyStopActive) {
        _emergencyStopActive = false;
        Serial.println(F("Emergency stop reset"));
    }
}

// Check safety timeout
bool CosmoMotorDriver::checkSafetyTimeout() {
    if (_safetyTimeout == 0) return false; // Timeout disabled
    
    return (millis() - _lastCommandTime) > _safetyTimeout;
}

// Update function (call in main loop)
void CosmoMotorDriver::update() {
    if (!_initialized) return;
    
    // Check for safety timeout
    if (checkSafetyTimeout() && (_currentSpeedA != 0 || _currentSpeedB != 0)) {
        Serial.println(F("Safety timeout - stopping motors"));
        stop();
    }
}

// Get version
const char* CosmoMotorDriver::getVersion() {
    return CosmoMotorConfig::LIBRARY_VERSION;
}

// Print status
void CosmoMotorDriver::printStatus() {
    Serial.println(F("--- Motor Driver Status ---"));
    Serial.print(F("Version: "));
    Serial.println(CosmoMotorConfig::LIBRARY_VERSION);
    Serial.print(F("Pins - ENA:"));
    Serial.print(_pinENA);
    Serial.print(F(" ENB:"));
    Serial.print(_pinENB);
    Serial.print(F(" IN1:"));
    Serial.print(_pinIN1);
    Serial.print(F(" IN2:"));
    Serial.print(_pinIN2);
    Serial.print(F(" IN3:"));
    Serial.print(_pinIN3);
    Serial.print(F(" IN4:"));
    Serial.println(_pinIN4);
    Serial.print(F("Current speeds - A:"));
    Serial.print(_currentSpeedA);
    Serial.print(F(" B:"));
    Serial.println(_currentSpeedB);
    Serial.print(F("Max speed: "));
    Serial.print(_maxSpeed);
    Serial.print(F(" Accel rate: "));
    Serial.println(_accelerationRate);
    Serial.print(F("Commands: "));
    Serial.print(_commandCount);
    Serial.print(F(" Errors: "));
    Serial.println(_errorCount);
    Serial.print(F("Emergency stop: "));
    Serial.println(_emergencyStopActive ? F("ACTIVE") : F("Inactive"));
    Serial.println(F("---------------------------"));
}

// Private methods

bool CosmoMotorDriver::validatePins() {
    // Check if PWM pins are valid (basic validation)
    return (_pinENA < 20 && _pinENB < 20 && _pinIN1 < 20 && 
            _pinIN2 < 20 && _pinIN3 < 20 && _pinIN4 < 20);
}

bool CosmoMotorDriver::validateSpeed(int16_t speed) {
    return (speed >= -CosmoMotorConfig::PWM_MAX && speed <= CosmoMotorConfig::PWM_MAX);
}

void CosmoMotorDriver::setMotorDirection(CosmoMotorConfig::MotorID motor, bool forward) {
    if (motor == CosmoMotorConfig::MOTOR_A || motor == CosmoMotorConfig::BOTH_MOTORS) {
        digitalWrite(_pinIN1, forward ? HIGH : LOW);
        digitalWrite(_pinIN2, forward ? LOW : HIGH);
    }
    if (motor == CosmoMotorConfig::MOTOR_B || motor == CosmoMotorConfig::BOTH_MOTORS) {
        digitalWrite(_pinIN3, forward ? HIGH : LOW);
        digitalWrite(_pinIN4, forward ? LOW : HIGH);
    }
}

void CosmoMotorDriver::setMotorSpeed(CosmoMotorConfig::MotorID motor, uint8_t speed) {
    speed = constrain(speed, CosmoMotorConfig::PWM_MIN, _maxSpeed);
    
    if (motor == CosmoMotorConfig::MOTOR_A || motor == CosmoMotorConfig::BOTH_MOTORS) {
        analogWrite(_pinENA, speed);
    }
    if (motor == CosmoMotorConfig::MOTOR_B || motor == CosmoMotorConfig::BOTH_MOTORS) {
        analogWrite(_pinENB, speed);
    }
}

void CosmoMotorDriver::updateLastCommandTime() {
    _lastCommandTime = millis();
}

CosmoMotorConfig::StatusCode CosmoMotorDriver::executeMovement(int16_t speedA, int16_t speedB) {
    if (!_initialized) {
        _errorCount++;
        return CosmoMotorConfig::STATUS_MOTOR_FAULT;
    }
    
    if (_emergencyStopActive) {
        return CosmoMotorConfig::STATUS_EMERGENCY_STOP;
    }
    
    // Handle motor A
    if (speedA == 0) {
        digitalWrite(_pinIN1, LOW);
        digitalWrite(_pinIN2, LOW);
        analogWrite(_pinENA, 0);
    } else {
        bool forwardA = speedA > 0;
        uint8_t absSpeedA = abs(speedA);
        absSpeedA = constrain(absSpeedA, CosmoMotorConfig::PWM_MIN, _maxSpeed);
        
        setMotorDirection(CosmoMotorConfig::MOTOR_A, forwardA);
        setMotorSpeed(CosmoMotorConfig::MOTOR_A, absSpeedA);
    }
    
    // Handle motor B
    if (speedB == 0) {
        digitalWrite(_pinIN3, LOW);
        digitalWrite(_pinIN4, LOW);
        analogWrite(_pinENB, 0);
    } else {
        bool forwardB = speedB > 0;
        uint8_t absSpeedB = abs(speedB);
        absSpeedB = constrain(absSpeedB, CosmoMotorConfig::PWM_MIN, _maxSpeed);
        
        setMotorDirection(CosmoMotorConfig::MOTOR_B, forwardB);
        setMotorSpeed(CosmoMotorConfig::MOTOR_B, absSpeedB);
    }
    
    // Update state
    _currentSpeedA = speedA;
    _currentSpeedB = speedB;
    updateLastCommandTime();
    _commandCount++;
    
    return CosmoMotorConfig::STATUS_OK;
}