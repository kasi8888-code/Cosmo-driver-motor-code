# Cosmo-driver-motor-code

Arduino code (driver-motor-code) for the Cozmo competition robot.

## 📌 Overview

This Arduino code controls 2 DC motors using a motor driver module (L298N / L293D / TB6612FNG or similar).  
It provides functions to move the robot:

- Forward
- Backward
- Left turn
- Right turn
- Stop

It uses:

- Digital pins → to set motor direction
- PWM pins → to set motor speed

## ⚡ Hardware Requirements

- Arduino (Uno, Nano, Mega, etc.)
- Motor driver module (L298N / L293D / TB6612FNG or equivalent)
- 2 DC motors
- Power supply (e.g., 7.4V–12V battery pack, depending on motor rating)
- Robot chassis + wheels

## 🖥️ Pin Connections

Since pins are not fixed yet, here’s the generic structure you must follow:

| Motor Driver | Arduino Pin              | Purpose                      |
|--------------|-------------------------|------------------------------|
| ENA          | PWM pin (e.g., ~5)      | Motor A speed control        |
| IN1          | Digital pin             | Motor A direction            |
| IN2          | Digital pin             | Motor A direction            |
| ENB          | PWM pin (e.g., ~6)      | Motor B speed control        |
| IN3          | Digital pin             | Motor B direction            |
| IN4          | Digital pin             | Motor B direction            |
| +12V         | Battery +               | Motor power                  |
| GND          | Battery – & Arduino GND | Common ground                |

⚠️ **Important:** Choose ENA & ENB as PWM-capable pins (marked with ~ on Arduino Uno).

## ⚙️ Code Features

- Functions for each movement:
  - `forward(speed)`
  - `backward(speed)`
  - `left(speed)`
  - `right(speed)`
  - `stop()`
- Adjustable speed control using PWM (0–255)
- Demo sequence in `loop()` to show all movements

## 🚦 Example Code (with placeholder pins)
```cpp
/*********************************************************************
 * Motor Driver Code (Template)
 * Replace PIN NUMBERS below with your actual connections.
 *********************************************************************/

// Motor A (Left motor)
#define ENA 5    // PWM pin for speed (change if needed)
#define IN1 7    // Direction pin 1
#define IN2 8    // Direction pin 2

// Motor B (Right motor)
#define ENB 6    // PWM pin for speed (change if needed)
#define IN3 9    // Direction pin 3
#define IN4 11   // Direction pin 4

// Speed presets
#define FAST_SPEED 200
#define SLOW_SPEED 100

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stop();
}

void loop() {
  forward(FAST_SPEED);
  delay(2000);

  backward(SLOW_SPEED);
  delay(2000);

  left(FAST_SPEED);
  delay(2000);

  right(SLOW_SPEED);
  delay(2000);

  stop();
  delay(3000);
}

void forward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void backward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void left(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void right(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
```

## 🛠️ How to Use

1. Decide which Arduino pins you will connect to ENA, ENB, IN1–IN4.
2. Make sure ENA & ENB are PWM-capable (~).
3. Update the `#define` values at the top of the code.
4. Upload the sketch to Arduino.
5. Connect motors and battery.
6. Robot will perform demo sequence.
7. Replace `loop()` with wireless control commands later if needed.

## 📌 Notes

- Always connect Arduino GND and Motor driver GND together.
- Don’t power motors directly from Arduino 5V pin. Use an external battery.
- You can adjust `FAST_SPEED` and `SLOW_SPEED` between 0–255.
- Later, you can replace the demo loop with Bluetooth / Wi-Fi / IR remote control.
