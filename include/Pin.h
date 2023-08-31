#ifndef _Pin_
#define _Pin_

#include "BoardPin.h"

void PinInit(void);
void PinStatusLed(uint8_t leds);
uint8_t PinReadBut(void);
uint8_t PinReadPot(uint8_t pot);
int HallRead(void);

#endif // _Pin_
