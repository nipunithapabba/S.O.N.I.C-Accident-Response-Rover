#define TRIG 5
#define ECHO 18
#define GAS_AO 34 // Connected to the Analog Output (A0) of the MQ-2 module

#define BUZZER 19
#define LED_RED 22
#define LED_GREEN 23

// Motor pins
#define IN1 27 // Left Motor Input 1
#define IN2 26 // Left Motor Input 2
#define IN3 25 // Right Motor Input 1
#define IN4 33 // Right Motor Input 2

#define ENA 13 // Left Motor Enable (PWM)
#define ENB 12 // Right Motor Enable (PWM)

long duration;
int distance;
int gasDetected; // Now stores the 0-4095 analog value
const int ALARM_THRESHOLD = 500; // Threshold for gas sensor (adjust this value based on testing)

void setup() {

// Ultrasonic
 pinMode(TRIG, OUTPUT);
 pinMode(ECHO, INPUT);

// MQ2 - Input is analog
// pinMode(GAS_AO, INPUT); // This line is not needed for analogRead()

// Indicators
 pinMode(BUZZER, OUTPUT);
 pinMode(LED_RED, OUTPUT);
 pinMode(LED_GREEN, OUTPUT);

 // Motor pins
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);

 // PWM Setup
 analogWrite(ENA, 200); // motor speed (0–255)
 analogWrite(ENB, 200);

 Serial.begin(9600);
  Serial.println("Accident Response Rover Initialized!");
}

void loop() {

 // -------- Ultrasonic --------
 digitalWrite(TRIG, LOW);
 delayMicroseconds(2);
 digitalWrite(TRIG, HIGH);
 delayMicroseconds(10);
 digitalWrite(TRIG, LOW);

 duration = pulseIn(ECHO, HIGH);
 distance = duration * 0.034 / 2;

 // -------- MQ2 Gas (READ ANALOG VALUE) --------
 gasDetected = analogRead(GAS_AO); // FIX 1: Use analogRead() for concentration

 Serial.print("Distance: ");
 Serial.print(distance);
 Serial.print(" cm   |   Current Gas Reading: ");
 Serial.println(gasDetected);

 // -------- GAS EMERGENCY --------
 // FIX 2: Compare analog value to the defined threshold
 if (gasDetected >= ALARM_THRESHOLD) { 
   stopRobot();
   Serial.println("⚠ GAS LEAK DETECTED! ");
   digitalWrite(LED_RED, HIGH);
   digitalWrite(LED_GREEN, LOW);
   tone(BUZZER, 2000); // Continuous high-frequency alarm
   return; // Immediately stop the loop iteration and wait here
 }

 // -------- OBSTACLE AVOIDANCE (Fixes Rotation Loop) --------
 if (distance < 20) {
   stopRobot();
   delay(300); // Pause

   moveBackward();
   delay(500); // Move back 0.5 seconds

   turnRight();
   delay(700); // FIX 3: Increased turn time (0.7 seconds) for better angle clearance
    stopRobot(); // Important: Stop after the turn to re-evaluate in the next loop
    delay(100);
 }

 // -------- SAFE → MOVE FORWARD --------
 else {
   moveForward();
   digitalWrite(LED_GREEN, HIGH);
   digitalWrite(LED_RED, LOW);
   noTone(BUZZER); // FIX 4: Use noTone() to properly turn off the buzzer
  }
}


// ---------------- MOTOR FUNCTIONS ----------------

void moveForward() {
 // Left Motor Forward
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);

 // Right Motor Forward
 digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW);

}

void moveBackward() {
 // Left Motor Backward
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);

 // Right Motor Backward
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);

}


void turnRight() {
 // Left Wheel Forward, Right Wheel Backward (Differential Turn)
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);

 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);

}
//stopping robot
void stopRobot() {
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, LOW);

 digitalWrite(IN3, LOW);
 digitalWrite(IN4, LOW);

}
