#ifndef _Weather_
#define _Weather_

#include <Adafruit_BMP085.h>

void PresInit(void);
void PresCheck(char mess[]);
void TempCheck(char mess[]);

#endif // _Weather_
