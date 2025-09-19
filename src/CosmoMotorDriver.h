/**
 * @file CosmoMotorDriver.h
 * @brief Professional motor driver library for Cosmo competition robot
 * @version 2.0.0
 * @date 2024
 * @author Cosmo Robot Team
 * @copyright MIT License
 * 
 * This header file defines the CosmoMotorDriver class, providing professional-grade
 * motor control functionality for dual DC motor robotics applications.
 */

#ifndef COSMO_MOTOR_DRIVER_H
#define COSMO_MOTOR_DRIVER_H

#include <Arduino.h>

/**
 * @brief Motor driver configuration constants
 */
namespace CosmoMotorConfig {
    // Version information
    static const char* LIBRARY_VERSION = "2.0.0";
    static const char* BUILD_DATE = __DATE__;
    static const char* BUILD_TIME = __TIME__;
    
    // PWM and timing constants
    static const uint8_t PWM_MIN = 0;
    static const uint8_t PWM_MAX = 255;
    static const uint16_t SAFETY_TIMEOUT_MS = 5000;
    static const uint8_t DEFAULT_ACCELERATION = 50;
    static const uint8_t EMERGENCY_STOP_TIME_MS = 100;
    
    // Motor identification
    enum MotorID {
        MOTOR_A = 0,    ///< Left motor (typically)
        MOTOR_B = 1,    ///< Right motor (typically)
        BOTH_MOTORS = 2 ///< Both motors simultaneously
    };
    
    // Movement directions
    enum Direction {
        FORWARD = 0,
        BACKWARD = 1,
        LEFT = 2,
        RIGHT = 3,
        STOP = 4
    };
    
    // Motor driver status codes
    enum StatusCode {
        STATUS_OK = 0,
        STATUS_INVALID_PIN = 1,
        STATUS_INVALID_SPEED = 2,
        STATUS_MOTOR_FAULT = 3,
        STATUS_EMERGENCY_STOP = 4
    };
}

/**
 * @class CosmoMotorDriver
 * @brief Professional dual motor driver class with advanced features
 * 
 * This class provides comprehensive motor control functionality including:
 * - Dual DC motor control with independent speed control
 * - Safety features and emergency stop capability
 * - Speed profiles and acceleration control
 * - Hardware abstraction for multiple motor driver types
 * - Diagnostic and telemetry capabilities
 * - Input validation and error handling
 */
class CosmoMotorDriver {
private:
    // Hardware pin assignments
    uint8_t _pinENA, _pinENB;           ///< PWM speed control pins
    uint8_t _pinIN1, _pinIN2;           ///< Motor A direction pins
    uint8_t _pinIN3, _pinIN4;           ///< Motor B direction pins
    
    // Current motor state
    int16_t _currentSpeedA, _currentSpeedB; ///< Current motor speeds (-255 to +255)
    uint32_t _lastCommandTime;              ///< Timestamp of last motor command
    bool _emergencyStopActive;              ///< Emergency stop state
    bool _initialized;                      ///< Initialization status
    
    // Safety and control parameters
    uint8_t _maxSpeed;                      ///< Maximum allowed speed
    uint8_t _accelerationRate;              ///< Acceleration/deceleration rate
    uint16_t _safetyTimeout;                ///< Safety timeout in milliseconds
    
    // Diagnostic counters
    uint32_t _commandCount;                 ///< Total commands processed
    uint32_t _errorCount;                   ///< Total errors encountered
    
    // Private methods
    bool validatePins();
    bool validateSpeed(int16_t speed);
    void setMotorDirection(CosmoMotorConfig::MotorID motor, bool forward);
    void setMotorSpeed(CosmoMotorConfig::MotorID motor, uint8_t speed);
    void updateLastCommandTime();
    CosmoMotorConfig::StatusCode executeMovement(int16_t speedA, int16_t speedB);

public:
    /**
     * @brief Constructor for CosmoMotorDriver
     * @param pinENA PWM pin for Motor A speed control
     * @param pinIN1 Direction pin 1 for Motor A
     * @param pinIN2 Direction pin 2 for Motor A
     * @param pinENB PWM pin for Motor B speed control
     * @param pinIN3 Direction pin 1 for Motor B
     * @param pinIN4 Direction pin 2 for Motor B
     */
    CosmoMotorDriver(uint8_t pinENA, uint8_t pinIN1, uint8_t pinIN2,
                     uint8_t pinENB, uint8_t pinIN3, uint8_t pinIN4);
    
    /**
     * @brief Initialize the motor driver
     * @return StatusCode indicating success or failure
     */
    CosmoMotorConfig::StatusCode begin();
    
    /**
     * @brief Move robot forward at specified speed
     * @param speed Motor speed (0-255)
     * @return StatusCode indicating success or failure
     */
    CosmoMotorConfig::StatusCode moveForward(uint8_t speed);
    
    /**
     * @brief Move robot backward at specified speed
     * @param speed Motor speed (0-255)
     * @return StatusCode indicating success or failure
     */
    CosmoMotorConfig::StatusCode moveBackward(uint8_t speed);
    
    /**
     * @brief Turn robot left (pivot turn)
     * @param speed Motor speed (0-255)
     * @return StatusCode indicating success or failure
     */
    CosmoMotorConfig::StatusCode turnLeft(uint8_t speed);
    
    /**
     * @brief Turn robot right (pivot turn)
     * @param speed Motor speed (0-255)
     * @return StatusCode indicating success or failure
     */
    CosmoMotorConfig::StatusCode turnRight(uint8_t speed);
    
    /**
     * @brief Stop all motors immediately
     * @return StatusCode indicating success or failure
     */
    CosmoMotorConfig::StatusCode stop();
    
    /**
     * @brief Emergency stop - immediate halt of all motors
     * @return StatusCode indicating success or failure
     */
    CosmoMotorConfig::StatusCode emergencyStop();
    
    /**
     * @brief Set individual motor speeds (advanced control)
     * @param speedA Speed for Motor A (-255 to +255, negative = reverse)
     * @param speedB Speed for Motor B (-255 to +255, negative = reverse)
     * @return StatusCode indicating success or failure
     */
    CosmoMotorConfig::StatusCode setMotorSpeeds(int16_t speedA, int16_t speedB);
    
    /**
     * @brief Set maximum allowed speed
     * @param maxSpeed Maximum speed (0-255)
     */
    void setMaxSpeed(uint8_t maxSpeed);
    
    /**
     * @brief Set acceleration/deceleration rate
     * @param rate Acceleration rate (0-255)
     */
    void setAccelerationRate(uint8_t rate);
    
    /**
     * @brief Set safety timeout
     * @param timeoutMs Timeout in milliseconds
     */
    void setSafetyTimeout(uint16_t timeoutMs);
    
    /**
     * @brief Get current motor speeds
     * @param speedA Reference to store Motor A speed
     * @param speedB Reference to store Motor B speed
     */
    void getCurrentSpeeds(int16_t& speedA, int16_t& speedB);
    
    /**
     * @brief Get diagnostic information
     * @param commandCount Reference to store total command count
     * @param errorCount Reference to store total error count
     */
    void getDiagnostics(uint32_t& commandCount, uint32_t& errorCount);
    
    /**
     * @brief Check if emergency stop is active
     * @return True if emergency stop is active
     */
    bool isEmergencyStopActive();
    
    /**
     * @brief Reset emergency stop state
     */
    void resetEmergencyStop();
    
    /**
     * @brief Check if safety timeout has occurred
     * @return True if timeout occurred
     */
    bool checkSafetyTimeout();
    
    /**
     * @brief Update motor driver (call regularly in main loop)
     * Should be called regularly to handle safety timeouts and smooth acceleration
     */
    void update();
    
    /**
     * @brief Get library version information
     * @return Version string
     */
    static const char* getVersion();
    
    /**
     * @brief Print status information to Serial
     */
    void printStatus();
};

#endif // COSMO_MOTOR_DRIVER_H