#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

const int pingPin = 7;
const int buttonPin = 2;
const int circonference = 3; // in meters

Adafruit_7segment display = Adafruit_7segment();

long totalDistance = 0;
float speed = 0; // in m/s
unsigned long lastRotationTime = 0;

bool spokePreviouslyDetected = false;
bool displaySpeed = false;

unsigned long lastButtonPress = 0;
const int debounceDelay = 250;
bool lastButtonState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  lastButtonState = digitalRead(buttonPin); 
  display.begin(0x70);
  Serial.begin(9600);
}

void loop() {
  handleButton();
  float distance = readDistance();
  detectRotation(distance);
  updateDisplay(distance);

  delay(50); //refresh time
}

// Detect button input with release
void handleButton() {
  bool buttonState = digitalRead(buttonPin);
  unsigned long now = millis();

  if (lastButtonState == LOW && buttonState == HIGH && (now - lastButtonPress > debounceDelay)) {
    displaySpeed = !displaySpeed;
    Serial.println(displaySpeed ? "Switched to speed" : "Switched to distance");
    lastButtonPress = now;
  }

  lastButtonState = buttonState;
}


float readDistance() {
  long duration;

  // emit sound wave
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // Listen for echo
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH, 30000); 

  // Convert to meters
  return (duration > 0) ? (duration / 29.0 / 2) / 100.0 : -1;
}

void detectRotation(float distance) {
  bool spokeNowDetected = (distance > 0 && distance < 1.5);

  if (spokeNowDetected && !spokePreviouslyDetected) {
    //update distance
    totalDistance += circonference;

    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - lastRotationTime;
	
    //upadate speed
    if (deltaTime > 0) {
      speed = circonference / (deltaTime / 1000.0); // m/s
    }

    lastRotationTime = currentTime;
  }

  spokePreviouslyDetected = spokeNowDetected;
}

void updateDisplay(float distance) {
  display.clear();

  int valueToDisplay = displaySpeed ? (int)speed : (int)totalDistance;
  int modeDigit = displaySpeed ? 2 : 1;

  // Cap display up to 999
  valueToDisplay = min(valueToDisplay, 999);

  display.writeDigitNum(0, modeDigit, true);

  int hundreds = (valueToDisplay / 100) % 10;
  int tens     = (valueToDisplay / 10)  % 10;
  int ones     = valueToDisplay % 10;

  // Display value digits
  display.writeDigitNum(1, hundreds);
  display.writeDigitNum(3, tens);
  display.writeDigitNum(4, ones);

  display.writeDisplay();

  // Debug 
  Serial.print("Sensor: ");
  Serial.print(distance, 2);
  Serial.print(" m | Speed: ");
  Serial.print(speed, 2);
  Serial.print(" m/s | Mode: ");
  Serial.println(displaySpeed ? "2 (Speed)" : "1 (Distance)");
}
