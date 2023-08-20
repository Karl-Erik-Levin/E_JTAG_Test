#include <Arduino.h>
#include <SPI.h>

#include "BoardPin.h"
#include "DispU8G2.h"
#include "IrRemCtrl.h"
#include "Weather.h"

void blink();
void statusLed(uint8_t but);
uint8_t readPot(uint8_t pot);
uint8_t readBut(void);
uint8_t hour=8, minute=48, second=12;

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
  PresInit();
}

void loop() {
  static uint32_t task1, task1Interval=100;
  static uint32_t task2, task2Interval=100;
  static uint32_t task3, task3Interval=1000;
  static IrKey irOld;
  static uint8_t cnt;
  static char dMess0[40], dMess1[40], dMess2[40], dMess3[40];


  //      T A S K   1       //
 if (millis() > task1) {
    task1 = millis() + task1Interval;

    task1Interval = readPot(0) + 10;
    IrKey ir = IrCheck();

    if (ir==IR_DOWN) cnt++;
    else if (ir==IR_UP) cnt--;
    else if (ir > 0) irOld = ir;

    DispClear();
    
    if (irOld==IR_ON) {
//      sprintf(dMess2, "IrKey   = %d", irOld);
      strcpy(dMess0, "Input");
      strcpy(dMess1, "");
      sprintf(dMess2, "LoopIntv= %d", task1Interval);
      DispPage(dMess0, 0, 0, 0);
      DispPage(dMess1, 1, 2, 0);
      DispPage(dMess2, 2, 2, 0);
      DispPage(dMess3, 3, 2, 0);
    } else if (irOld==IR_MENU) {
      PresCheck(dMess2);
      TempCheck(dMess3);
      DispPage("BMP180", 0, 0, 0);
      DispPage("", 1, 2, 0);
      DispPage(dMess2, 2, 2, 0);
      DispPage(dMess3, 3, 2, 0);
    } else if (irOld==IR_C) {
      DispPage("ncenB10_tf", 0, 0, (cnt%4==0));
      DispPage("ncenR10_tf", 1, 1, (cnt%4==1));
      DispPage("helvR10_tf", 2, 2, (cnt%4==2));
      DispPage("courR12_tf", 3, 3, (cnt%4==3));
    } else
      DispClock(hour, minute, second);

    DispBuffer();

    blink();
  }

  //      T A S K   2       //
 if (millis() > task2) {
    task2 = millis() + task2Interval;

    uint8_t but = readBut();
    statusLed(but);
    sprintf(dMess3, "Button  = %d", but);
  }

  //      T A S K   3       //
 if (millis() > task3) {
    task3 = millis() + task3Interval;

    if (++second > 59) {
      second = 0;
      if (++minute > 59) {
        minute = 0;
        if (++hour > 23) {
          hour = 0;
        }
      }
    }
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
  if (but==1)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);

  if (but==2)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);

  if (but==3)
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
