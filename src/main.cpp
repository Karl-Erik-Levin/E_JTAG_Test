#include <Arduino.h>

#include "BoardPin.h"
#include "DispU8G2.h"
#include "IrRemCtrl.h"

void blink();
void statusLed(uint8_t but);
uint8_t readPot(uint8_t pot);
uint8_t readBut(void);

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(KEY_0, INPUT);
  pinMode(KEY_1, INPUT);

  DispInit();
  IrInit();
}

void loop() {
  static uint32_t task1, task1Interval=100;
  static uint32_t task2, task2Interval=100;
  static char dispMes2[40];
  static IrKey irOld;
  static uint8_t cnt;
  char dispMes1[40], dispMes3[40];


  //      T A S K   1       //
 if (millis() > task1) {
    task1 = millis() + task1Interval;

    task1Interval = readPot(0) + 10;
    IrKey ir = IrCheck();

    if (ir==IR_DOWN) cnt++;
    else if (ir==IR_UP) cnt--;
    else if (ir > 0) irOld = ir;

    DispClear();
    
    if (irOld==IR_MENU) {
      DispPage(dispMes2, 0, 1, (cnt%4==0));
      DispPage("ncenB10_tr", 1, 1, (cnt%4==1));
      DispPage("KalleLevin", 2, 1, (cnt%4==2));
      DispPage("ncenB10_te", 3, 1, (cnt%4==3));
    } else {
      sprintf(dispMes1, "LoopIntv = %d", task1Interval);
      sprintf(dispMes3, "IrKey= %d", irOld);
      DispPrint(0, dispMes1);
      DispPrint(1, dispMes2);
      DispPrint(2, dispMes3);
      DispPrint(3, dispMes3);
    }
    DispBuffer();

    blink();
  }

  //      T A S K   2       //
 if (millis() > task2) {
    task2 = millis() + task2Interval;

    uint8_t but = readBut();
    statusLed(but);
    sprintf(dispMes2, "Button = %d", but);
  }
}

void blink() {
  static uint8_t count;

  if (count%32 == 0)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);

  count++;
}

void statusLed(uint8_t but) {
  if (but&1)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);

  if (but&2)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);

  if (but&4)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);
}

uint8_t readPot(uint8_t pot) {
  uint8_t portPin = POT0;
  if (pot == 1) portPin = POT1;

  long val = analogRead(portPin);
  return map(val , 0, 4095, 0, 255);
}

uint8_t readBut(void) {
  return digitalRead(KEY_0)*2 + digitalRead(KEY_1);
}
