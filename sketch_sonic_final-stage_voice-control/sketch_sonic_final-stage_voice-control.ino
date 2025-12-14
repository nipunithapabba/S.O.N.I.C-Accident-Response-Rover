#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

// === Pin Definitions === //
#define TRIG 5
#define ECHO 18
#define GAS_AO 34 

#define BUZZER 19
#define LED_RED 22
#define LED_GREEN 23

#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

#define ENA 13 
#define ENB 12

long duration;
int distance;
int gasDetected;

bool autonomous = false;   

const int ALARM_THRESHOLD = 500; 
int Speed = 200; // Motor PWM speed (0-255)

// ---- Function Prototypes ---- //
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void turnRight();
void stopRobot();
void autonomousDrive();

void setup(){
  Serial.begin(115200);
  SerialBT.begin("SONICbtl");  // Bluetooth name
  delay(500);
  pinMode(TRIG, OUTPUT); 
  pinMode(ECHO, INPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);

  Serial.println("Accident Rover Online");
}

void loop(){
  delay(50);
  // GAS SENSOR
  gasDetected = analogRead(GAS_AO);
  if(gasDetected >= ALARM_THRESHOLD){
    stopRobot();
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    tone(BUZZER, 2000);
    delay(200); // keep buzzing
    return;
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    noTone(BUZZER);
  }

  // BT Manual Control
  if(SerialBT.available()){
    char c = SerialBT.read();

    if(c=='M'){autonomous=false; stopRobot();}
    if(c=='A'){autonomous=true;}

    if(!autonomous){
      if(c=='F') moveForward();
      if(c=='B') moveBackward();
      if(c=='L') turnLeft();
      if(c=='R') turnRight();
      if(c=='S') stopRobot();
    }
  }

  // Autonomous driving
  if(autonomous) autonomousDrive();
}

// =================== AUTONOMOUS AVOIDANCE =================== //
void autonomousDrive(){
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);
  distance = duration*0.034/2;

  if(distance < 20){   // Obstacle detected
    stopRobot(); delay(200);
    moveBackward(); delay(500);
    turnRight(); delay(700);
    stopRobot(); delay(100);
  }
  else{
    moveForward();
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER);
  }
}

// ==================== MOTOR CONTROLS ======================= //
void moveForward(){
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
}
void moveBackward(){
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
}
void turnLeft(){
  digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);
}
void turnRight(){
  digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);
}
void stopRobot(){
  digitalWrite(IN1,LOW); digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);
}