#include "SevSeg.h"

SevSeg sevseg;
float timer = 02.00f;

//time_loss && speed_up_time
int buttonPinA0 = A0;
int buttonPinA1 = A1;
int buttonPinA2 = A2;
int buttonState = 0;
int A1_pressed = 0;
int A2_pressed = 0;
float base_time_decrementation = 00.01f;

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

  Serial.begin(115200);
}

void is_valid_decrementation(float val) {
  if (timer - val <= 0) {
    timer = 0;
  } else {
    int a = (int)(timer * 100);
    int b = (int)(val * 100);

    Serial.print((a - b) % 100);
    Serial.print("\n");
    Serial.print(a - b);
    Serial.print("= ");
    Serial.print(a);
    Serial.print("-");
    Serial.print(b);
    Serial.print("\n");

    if ((a - b) % 100 > 60)
      timer -= 00.41f;

    //a - b  % 100
    //(102 - 5) % 100 > 60
    //timer -= 00.41f;
    timer -= val;
  }
}

void time_loss() {
  buttonState = digitalRead(buttonPinA1);
  if (buttonState == HIGH && A1_pressed == 0) {
    is_valid_decrementation(00.05f);
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

void loop() {
  unsigned long currentMillis = millis();

  if ((int)(timer * 100) > 0 && (currentMillis - previousMillis >= interval)) {
    previousMillis = currentMillis;
    if (timer == 0)
      timer -= 00.41f;
    timer -= base_time_decrementation;
    sevseg.setNumberF(timer, 2);
  }

  speed_up_time();
  time_loss();

  sevseg.refreshDisplay();
}


/*
void stop_time() {
  buttonState = digitalRead(buttonPinA2);
  if (buttonState == HIGH) {
    T = 00.00f;
  } else
    T = 00.01f;
}
*/
