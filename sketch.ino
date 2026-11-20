#include <Arduino.h>

const uint8_t ledPins[] = {16, 17, 18, 19, 21, 22};
const uint8_t randomButtonPin = 32;
const uint8_t resetButtonPin = 33;
const uint8_t ledCount = sizeof(ledPins) / sizeof(ledPins[0]);
const unsigned long debounceDelay = 50;

uint8_t diceValue = 0;

void displayDiceValue(uint8_t value) {
  for (uint8_t index = 0; index < ledCount; index++) {
    digitalWrite(ledPins[index], index < value ? HIGH : LOW);
  }
}

void printDiceValue(const char* action) {
  Serial.print(action);
  Serial.println(diceValue);
}

bool waitForButtonPress(uint8_t buttonPin) {
  if (digitalRead(buttonPin) != LOW) {
    return false;
  }

  delay(debounceDelay);
  if (digitalRead(buttonPin) != LOW) {
    return false;
  }

  while (digitalRead(buttonPin) == LOW) {
    delay(1);
  }

  delay(debounceDelay);
  return true;
}

void setup() {
  Serial.begin(115200);

  for (uint8_t index = 0; index < ledCount; index++) {
    pinMode(ledPins[index], OUTPUT);
  }

  pinMode(randomButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);

  displayDiceValue(0);
  Serial.println("Electronic Dice ready");
}

void loop() {
  if (waitForButtonPress(randomButtonPin)) {
    diceValue = random(1, 7);
    displayDiceValue(diceValue);
    printDiceValue("Random Dice = ");
    return;
  }

  if (waitForButtonPress(resetButtonPin)) {
    diceValue = 0;
    displayDiceValue(diceValue);
    printDiceValue("Reset Dice = ");
  }
}
