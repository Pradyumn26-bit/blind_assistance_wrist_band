// Author: Pradyumn Parashar
// Project: Blind Assistance Wearable Device
// Year: 2025

/*
  Blind Assistance Wearable Device
  --------------------------------
  Detects obstacles ahead and elevation changes using two ultrasonic sensors.
  Provides audio alerts through a buzzer with different tones.

  Components:
  - Arduino Uno
  - 2x HC-SR04 Ultrasonic Sensors
  - Buzzer
*/

// Pin configuration
#define TRIG_FORWARD 4
#define ECHO_FORWARD 5
#define BUZZER 6
#define TRIG_DOWNWARD 7
#define ECHO_DOWNWARD 8

// Distance thresholds (cm)
const int VERY_CLOSE = 20;
const int FORWARD_RANGE = 80;

const int DOWNWARD_RAISED = 50;
const int DOWNWARD_DROP = 110;

// Function to read distance from ultrasonic sensor
long readDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // timeout ~30ms

  if (duration == 0)
    return 400; // treat as no obstacle (sensor timeout)

  return duration * 0.034 / 2;
}

void setup()
{
  Serial.begin(9600);

  pinMode(TRIG_FORWARD, OUTPUT);
  pinMode(ECHO_FORWARD, INPUT);

  pinMode(TRIG_DOWNWARD, OUTPUT);
  pinMode(ECHO_DOWNWARD, INPUT);

  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, LOW);
}

void loop()
{
  long forwardDist = readDistance(TRIG_FORWARD, ECHO_FORWARD);
  long downwardDist = readDistance(TRIG_DOWNWARD, ECHO_DOWNWARD);

  Serial.print("Forward: ");
  Serial.print(forwardDist);
  Serial.print(" cm | Downward: ");
  Serial.print(downwardDist);
  Serial.println(" cm");

  bool forwardTrig = (forwardDist <= FORWARD_RANGE);
  bool downwardTrig = (downwardDist <= DOWNWARD_RAISED || downwardDist > DOWNWARD_DROP);

  // 1. Both conditions triggered
  if (forwardTrig && downwardTrig)
  {
    tone(BUZZER, 2000);
    delay(200);
    noTone(BUZZER);
    delay(200);
  }

  // 2. Very close obstacle ahead
  else if (forwardDist <= VERY_CLOSE)
  {
    tone(BUZZER, 1000);
    delay(1000);
    noTone(BUZZER);
    delay(100);
  }

  // 3. Object detected ahead
  else if (forwardTrig)
  {
    tone(BUZZER, 1200);
    delay(300);
    noTone(BUZZER);
    delay(300);
  }

  // 4. Deep drop detected
  else if (downwardDist > DOWNWARD_DROP)
  {
    tone(BUZZER, 1500);
    delay(200);
    noTone(BUZZER);
    delay(800);
  }

  // 5. Raised object detected
  else if (downwardDist <= DOWNWARD_RAISED)
  {
    tone(BUZZER, 1700);
    delay(100);
    noTone(BUZZER);
    delay(100);
  }

  delay(100); // stability delay
}
