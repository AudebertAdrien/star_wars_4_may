#include "SevSeg.h"

SevSeg sevseg;
float timer = 10.00f;

//time_loss && speed_up_time
int buttonPinA0 = A0;
int buttonPinA1 = A1;
int buttonPinA2 = A2;
int buttonState = 0;
int A1_pressed = 0;
int A2_pressed = 0;

//buzzer
int buzzer = 0;
int buzzer_time = 0;

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

  Serial.begin(115200);
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
  if (buttonState == HIGH) {
    buzzer_time = 600;
    interval = 20;
  } else {
    buzzer_time = 0;
    interval = 1000;
  }
}

int stop_time() {
  buttonState = digitalRead(buttonPinA2);
  if (buttonState == HIGH) {
    return (0);
  } else
    return (1);
}

void is_valid_decrementation(float val) {
  if (timer - val <= 0.0) {
    timer = 0;
    return;
  }
  int a = (int)(timer * 100);
  int b = (int)(val * 100);
  
  Serial.println("================");
  Serial.print(a);
  Serial.print(" : ");
  Serial.print(timer);
  Serial.print(" : ");
  Serial.println((a - b) % 100);
  Serial.println("================");

  Serial.print("\n");
  

  if (((a + 1) % 100) == 0) {
    
    Serial.println("1////1");
    Serial.println(timer);
    Serial.println("1////1");
    

    timer -= 00.41f;
  } else {
    
    Serial.println("2////2");
    Serial.println(timer);
    Serial.println("2////2");
    
    timer -= val;
  }
}

void loop() {
  unsigned long currentMillis = millis();
  int i;
  int bip_len = interval + buzzer_time;

  speed_up_time();
  time_loss();
  stop_time();

  sevseg.setNumberF(timer, 2);

  if ((int)(timer * 100) > 0 && (currentMillis - previousMillis > interval) && stop_time()) {
    previousMillis = currentMillis;

    /*
    for (i = 0; i < bip_len; i++) {
      digitalWrite(buzzer, HIGH);
    }
    */

    if (timer == 10.00f)
      is_valid_decrementation(00.41f);
    else
      is_valid_decrementation(00.01f);
    sevseg.setNumberF(timer, 2);
  }

  //digitalWrite(buzzer, LOW);

  sevseg.refreshDisplay();
}