#include <Arduino.h>
#include "Pin.h"

void PinInit(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(KEY_0, INPUT_PULLDOWN);
  pinMode(KEY_1, INPUT_PULLDOWN);
  pinMode(KEY_2, INPUT_PULLDOWN);
}

void PinStatusLed(uint8_t leds) {
  if (leds==1)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);

  if (leds==2)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);

  if (leds==3)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);
}

uint8_t PinReadBut(void) {
  return digitalRead(KEY_2)*4 + digitalRead(KEY_1)*2 + digitalRead(KEY_0);
}

uint8_t PinReadPot(uint8_t pot) {
  uint8_t portPin = POT0;
  if (pot == 1) portPin = POT1;

  long val = analogRead(portPin);
  return map(val , 0, 4095, 0, 255);
}

int HallRead(void) {
  static int hall[] = {25, 25, 25, 25, 25, 25, 25, 25};
  static uint8_t inpIdx;
  const uint8_t numHall = 8;
  int totHall = 0;

  hall[inpIdx%numHall] = hallRead();
  for (uint8_t i=0; i<numHall; i++) totHall += hall[i];
  inpIdx++;

  return  (totHall / numHall);
}

