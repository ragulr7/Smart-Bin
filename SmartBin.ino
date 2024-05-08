#include <Servo.h>

Servo myservo;

int proximityPin = A0; // Analog pin for the proximity sensor
int moisturePin = A1; // Analog pin for the soil moisture sensor
int proximityValue = 0;
int moistureValue = 0;
const int proximityThreshold = 200; // Adjust this threshold based on your sensor readings for a reduced range

#define trigPin 4
#define echoPin 5
#define trigPin1 3
#define echoPin1 2
#define buzz 8

void setup() {
  myservo.attach(10); // Attach the servo to pin 10
  myservo.write(90); // Initialize the servo at 90 degrees (center position)
  Serial.begin(9600); // Initialize serial communication at 9600 bps

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(buzz, OUTPUT);
}

void loop() {
  // Ultrasonic sensor code to measure distance
  long duration, duration1, inches1, cm1, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin1, LOW);

  // Take a reading on echo pin
  duration1 = pulseIn(echoPin1, HIGH);

  // Convert the time into a distance
  inches1 = microsecondsToInches(duration1);
  cm1 = microsecondsToCentimeters(duration1);

  Serial.print("Distance (Sensor 1): ");
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm | ");

  Serial.print("Distance (Sensor 2): ");
  Serial.print(inches1);
  Serial.print("in, ");
  Serial.print(cm1);
  Serial.print("cm");
  Serial.println();

  proximityValue = analogRead(proximityPin); // Read the proximity sensor (analog)

  if (proximityValue < proximityThreshold) {
    // Object detected by the proximity sensor
    Serial.println("Object detected. Initiating moisture sensor...");

    // Read the soil moisture sensor (analog pin)
    moistureValue = analogRead(moisturePin);

    Serial.print("Moisture Level: ");
    Serial.println(moistureValue);

    if (moistureValue < 1000) {
      Serial.println("Soil is wet.");
      myservo.write(180); 
      delay(1300);// Turn servo left (counter-clockwise) by 90 degrees
      // Turn servo right (clockwise) by 90 degrees
    } else if (moistureValue >= 1000) {
      Serial.println("Soil is dry.");
      myservo.write(0); 
      delay(1300);// Turn servo left (counter-clockwise) by 90 degrees
    }
  } else {
    // No object detected, skip moisture sensor reading
    Serial.println("No object detected. Moisture sensor not active.");
    myservo.write(90); // Keep the servo at 90 degrees (center position)
  }

  // Check ultrasonic sensor readings and activate the buzzer if required
  if (inches < 3 || inches1 < 3) {
    digitalWrite(buzz, HIGH);
    delay(500);
  } else {
    digitalWrite(buzz, LOW);
    delay(500);
  }
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
