#include <Arduino.h>
#include <SPI.h>

#include "BoardPin.h"
#include "DispU8G2.h"
#include "IrRemCtrl.h"
#include "Weather.h"
#include "GY521.h"
#include "WifiTime.h"

void blink();
void statusLed(uint8_t but);
void PageInit(void);
uint8_t readPot(uint8_t pot);
uint8_t readBut(void);
int HallRead(void);

uint8_t hour=8, minute=48, second=12;
dispRow row[4];

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
  GY521Init();
  PageInit();
  WifiTimeInit();
}

void loop() {
  static uint32_t task1, task1Interval=100;
  static uint32_t task2, task2Interval=100;
  static uint32_t task3, task3Interval=1000;
  static IrKey irOld;
  static uint8_t cnt;
  static char dMess0[40], dMess1[40], dMess2[40], dMess3[40];

  //------------------------//
  //      T A S K   1       //
  //------------------------//
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
      strcpy(row[0].message, "INPUT");
      strcpy(row[1].message, "");
      sprintf(row[2].message, "LoopIntv= %d", task1Interval);
      strcpy(row[3].message, dMess3);
      DispPage(row);
    } else if (irOld==IR_MENU) {
      strcpy(row[0].message, "BMP180");
      strcpy(row[1].message, "");
      PresCheck(row[2].message);
      TempCheck(row[3].message);
      DispPage(row);
    } else if (irOld==IR_TEST) {
      strcpy(row[0].message, "HALL sensor");
      strcpy(row[1].message, "");
      sprintf(row[2].message, "Magnetic= %d", HallRead());
      strcpy(row[3].message, "");
      DispPage(row);
    } else if (irOld==IR_BACK) {
      uint32_t chipId = 0;
	    for(int i=0; i<17; i=i+8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
      }
      sprintf(row[0].message, "%s", ESP.getChipModel());
      strcpy(row[1].message, " ");
      sprintf(row[2].message, "Rev: %d has %d cores", ESP.getChipRevision(), ESP.getChipCores());
      sprintf(row[3].message, "Chip ID: %d", chipId);
      DispPage(row);
    } else if (irOld==IR_1) {
      GY521Read();
      strcpy(row[0].message, "Angle:");
      strcpy(row[1].message, "");
      sprintf(row[2].message, "Pitch= %5.1f", GY521Pitch());
      sprintf(row[3].message, "Roll = %5.1f", GY521Roll());
      DispPage(row);
    } else if (irOld==IR_2) {
      GY521Read();
      strcpy(row[0].message, "Accelerometer:");
      sprintf(row[1].message, "X = %d", GY521AcX());
      sprintf(row[2].message, "Y = %d", GY521AcY());
      sprintf(row[3].message, "Z = %d", GY521AcZ());
      DispPage(row);
    } else if (irOld==IR_3) {
      GY521Read();
      strcpy(row[0].message, "Gyroscope:");
      sprintf(row[1].message, "X = %d", GY521GyX());
      sprintf(row[2].message, "Y = %d", GY521GyY());
      sprintf(row[3].message, "Z = %d", GY521GyZ());
      DispPage(row);
    } else if (irOld==IR_C) {
      DispRow("ncenB10_tf", 0, 0, (cnt%4==0));
      DispRow("ncenR10_tf", 1, 1, (cnt%4==1));
      DispRow("helvR10_tf", 2, 2, (cnt%4==2));
      DispRow("courR12_tf", 3, 3, (cnt%4==3));
    } else
      DispClock(hour, minute, second);

    DispBuffer();

    blink();
  }

  //------------------------//
  //      T A S K   2       //
  //------------------------//
  if (millis() > task2) {
    task2 = millis() + task2Interval;

    uint8_t but = readBut();
    statusLed(but);
    sprintf(dMess3, "Button  = %d", but);
  }

  //------------------------//
  //      T A S K   3       //
  //------------------------//
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

void PageInit(void) {
  for (uint8_t i=0; i<4; i++) {
     strcpy(row[i].message, "");
    row[i].row = i;
    row[i].font= 2;
    row[i].selec= false;
  }
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
