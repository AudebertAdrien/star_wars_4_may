#include "SevSeg.h"

SevSeg sevseg;
float timer = 03.00f;
int count = 0;

//time_loss && speed_up_time
int buttonPinA0 = A0;
int buttonPinA1 = A1;
int buttonPinA2 = A2;
int buttonS    is_active = 1;

//buzzer
int buzzer = 0;
int is_active = 0;

unsigned long previousMillis = 0;  // will store last time sevseg was updated
long interval = 1000;              // interval at which to blink (milliseconds)

void setup() {
  byte numDigits = 4;
  byte digitPins[] = { 10, 11, 12, 13 };
  byte segmentPins[] = { 9, 2, 3, 5, 6, 8, 7, 4 };

  bool resistorsOnSegments = true;       // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE;  // See README.md for options
  bool updateWithDelays = false;         // Default 'false' is Recommended
  bool leadingZeros = true;              // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false;          // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);

  //time_loss && speed_up_time
  pinMode(buttonPinA0, INPUT);
  pinMode(buzzer, OUTPUT);
  //Serial.begin(115200);
}

void is_valid_decrementation(float val) {
  if (timer - val <= 0) {
    timer = 0;
  } else {
    int a = (int)(timer * 100);
    int b = (int)(val * 100);
    if ((a - b) % 100 > 60)
      timer -= 00.41f;
    timer -= val;
  }
}

void time_loss() {
  buttonState = digitalRead(buttonPinA1);
  if (buttonState == HIGH && A1_pressed == 0) {
    is_valid_decrementation(01.00f);
    A1_pressed = 1;
    sevseg.setNumberF(timer, 2);
  }
  if (buttonState == LOW && A1_pressed == 1)
    A1_pressed = 0;
}

void speed_up_time() {
  buttonState = digitalRead(buttonPinA0);
  if (buttonState == HIGH)
    interval = 100;
  else
    interval = 1000;
}

int stop_time() {
  buttonState = digitalRead(buttonPinA2);
  if (buttonState == HIGH) {
    return (0);
  } else
    return (1);
}

void loop() {
  unsigned long currentMillis = millis();
  //Serial.print("\n");

  if ((int)(timer * 100) > 0 && (currentMillis - previousMillis >= interval) && stop_time()) {
    previousMillis = currentMillis;
    if (count == 0)
      is_valid_decrementation(00.41f);
    else
      is_valid_decrementation(00.01f);
    count++;
    digitalWrite(buzzer, HIGH);

    sevseg.setNumberF(timer, 2);
  }
  speed_up_time();
  time_loss();
  stop_time();

  sevseg.refreshDisplay();
}