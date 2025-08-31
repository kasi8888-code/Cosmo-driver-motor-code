
// Motor A (e.g., Left Motor) Pins
#define ENA 5  // Speed Pin (must be a PWM pin '~')
#define IN1 7  // Direction Pin 1
#define IN2 8  // Direction Pin 2

// Motor B (e.g., Right Motor) Pins
#define ENB 6  // Speed Pin (must be a PWM pin '~')
#define IN3 9  // Direction Pin 3
#define IN4 11 // Direction Pin 4

// --- Speed Definitions ---
#define FAST_SPEED 200 // For general movement (Value 0-255)
#define SLOW_SPEED 90  // For "Precision Mode" [cite: 19] (Value 0-255)


void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  Serial.println("Motor driver setup complete. Starting demo...");

  // Set all motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ensure motors are stopped at startup
  stop();
}

void loop() {
  // This loop demonstrates the functions.
  // Later, this will be replaced with code to read remote control commands.

  Serial.println("Moving forward (fast)...");
  forward(FAST_SPEED);
  delay(2000); // Wait 2 seconds

  Serial.println("Moving backward (slow)...");
  backward(SLOW_SPEED);
  delay(2000); // Wait 2 seconds

  Serial.println("Turning right...");
  right(FAST_SPEED);
  delay(1000); // Wait 1 second

  Serial.println("Turning left...");
  left(FAST_SPEED);
  delay(1000); // Wait 1 second
  
  Serial.println("Stopping.");
  stop();
  delay(3000); // Wait 3 seconds before repeating the demo
}

// --- MOVEMENT FUNCTIONS ---

/**
 * @brief Drives the robot forward at a given speed.
 * @param speed PWM value from 0 (off) to 255 (full speed).
 */
void forward(int speed) {
  // Motor A: Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Motor B: Forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Set speed for both motors
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

/**
 * @brief Drives the robot backward at a given speed.
 * @param speed PWM value from 0 (off) to 255 (full speed).
 */
void backward(int speed) {
  // Motor A: Backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Motor B: Backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Set speed for both motors
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

/**
 * @brief Turns the robot left on the spot (pivot turn).
 * @param speed PWM value from 0 (off) to 255 (full speed).
 */
void left(int speed) {
  // Motor A: Backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Motor B: Forward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Set speed for both motors
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

/**
 * @brief Turns the robot right on the spot (pivot turn).
 * @param speed PWM value from 0 (off) to 255 (full speed).
 */
void right(int speed) {
  // Motor A: Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Motor B: Backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Set speed for both motors
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

/**
 * @brief Stops both motors.
 */
void stop() {
  // A robust stop sets direction pins to LOW and speed to 0.
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}