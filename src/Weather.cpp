#include "Weather.h"

Adafruit_BMP085 bmp;
uint32_t Pressure[] = {101300, 101300, 101300, 101300};
const uint8_t noSamp = 4;

void PresInit(void) {
  bmp.begin();
}

void PresCheck(char mess[]) {
  static uint8_t inpIdx;
  uint32_t totPres = 0;

  Pressure[inpIdx%noSamp] = bmp.readPressure();
  for (uint8_t i=0; i<noSamp; i++) totPres += Pressure[i];
  totPres /= noSamp;
  inpIdx++;

  sprintf(mess, "Pres = %d.%d Pa", totPres/100, (totPres/10)%10);
}

void TempCheck(char mess[]) {
  sprintf(mess, "Temp = %4.1f C", bmp.readTemperature());
}

