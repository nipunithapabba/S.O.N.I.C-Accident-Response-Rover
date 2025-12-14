#define MQ2_AO 34        // Digital output from MQ2
#define GREEN_LED 18
#define RED_LED 19
#define BUZZER 23
const int ALARM_THRESHOLD = 200; 

void setup() {
  Serial.begin(9600);
  pinMode(MQ2_DO, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, HIGH);  // Default safe state
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  int gasState = digitalRead(MQ2_AO);  // HIGH = safe, LOW = gas detected
  Serial.print("Current Gas Reading: ");
  Serial.println(gasValue);

  if (gasState == LOW) {     // Gas detected
    Serial.println("⚠ GAS LEAK DETECTED!");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    tone(BUZZER, 2000);
  } 
  else {                      // Safe
    Serial.println("✓ AIR SAFE");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    noTone(BUZZER);
  }

  delay(300);
}




