#include <Arduino.h>
#include <SPI.h>

#include "Pin.h"
#include "DispU8G2.h"
#include "IrRemCtrl.h"
#include "Weather.h"
#include "GY521.h"
#include "WifiTime.h"

void blink();
void clearPage(dispRow row[]);


uint8_t hour=8, minute=48, second=12;   // Also ref in WifiTime.cpp
dispRow row[4];

void setup() {
  Serial.begin(115200);

  PinInit();
  DispInit();
  IrInit();
  PresInit();
  GY521Init();
  WifiTimeInit();
  clearPage(row);
}

void loop() {
  static uint32_t task1, task1Interval=100;
  static uint32_t task2, task2Interval=100;
  static uint32_t task3, task3Interval=1000, task3cnt;
  static IrKey irOld;
  static uint8_t rowCnt;
  static char butMessage[20];

  //------------------------//
  //      T A S K   1       //
  //------------------------//
  if (millis() > task1) {
    task1 = millis() + task1Interval;

    task1Interval = PinReadPot(0) + 10;
    IrKey ir = IrCheck();

    if (ir==IR_DOWN) rowCnt++;
    else if (ir==IR_UP) rowCnt--;
    else if (ir > 0) {
      irOld = ir;
      clearPage(row);
    }
    DispClear();
    
    if (irOld==IR_ON) {
//      sprintf(dMess2, "IrKey   = %d", irOld);
      strcpy(row[0].message, "INPUT");
      strcpy(row[1].message, "");
      sprintf(row[2].message, "LoopIntv= %d", task1Interval);
      strcpy(row[3].message, butMessage);
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
      sprintf(row[2].message, "Rev: %d has %d cores", ESP.getChipRevision(), ESP.getChipCores());
      sprintf(row[3].message, "Chip ID: %d", chipId);
      DispPage(row);
    } else if (irOld==IR_1) {
      GY521Read();
      strcpy(row[0].message, "Angle:");
      sprintf(row[1].message, "Pitch = %5.1f", GY521Pitch());
      sprintf(row[2].message, "Roll  = %5.1f", GY521Roll());
      sprintf(row[3].message, "Resultant= %5.2f", GY521Resultant());
      DispPage(row);
    } else if (irOld==IR_2) {
      GY521Read();
      strcpy(row[0].message, "Accelerometer:");
      sprintf(row[1].message, "X = %5.2f", GY521AcX());
      sprintf(row[2].message, "Y = %5.2f", GY521AcY());
      sprintf(row[3].message, "Z = %5.2f", GY521AcZ());
      DispPage(row);
    } else if (irOld==IR_3) {
      GY521Read();
      strcpy(row[0].message, "Gyroscope:");
      sprintf(row[1].message, "X = %d", GY521GyX());
      sprintf(row[2].message, "Y = %d", GY521GyY());
      sprintf(row[3].message, "Z = %d", GY521GyZ());
      DispPage(row);
    } else if (irOld==IR_4) {
      GY521Read();
      strcpy(row[0].message, "Temperatur:");
      sprintf(row[2].message, "Temp =%4.1f C", GY521TempC());
      sprintf(row[3].message, "Temp =%4.1f F", GY521TempF());
      DispPage(row);
    } else if (irOld==IR_C) {
      strcpy(row[0].message, "ncenB10_tf"), row[0].font=0, row[0].selec=(rowCnt%4==0);
      strcpy(row[1].message, "ncenR10_tf"), row[1].font=1, row[1].selec=(rowCnt%4==1);
      strcpy(row[2].message, "helvR10_tf"), row[2].font=2, row[2].selec=(rowCnt%4==2);
      strcpy(row[3].message, "courR12_tf"), row[3].font=3, row[3].selec=(rowCnt%4==3);
      DispPage(row);
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

    uint8_t but = PinReadBut();
    PinStatusLed(but);
    sprintf(butMessage, "Button  = %d", but);
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

    if (task3cnt < 300)
      task3cnt++;
    else {
      task3cnt = 0;
      WifiTimeRead();         // Update time from Internet
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

void clearPage(dispRow row[]) {
  for (uint8_t i=0; i<4; i++) {
    strcpy(row[i].message, "");
    row[i].row  = i;
    row[i].font = 2;
    row[i].selec= false;
  }
}

