#ifndef _BoardPin_
#define _BoardPin_

// For 0.96" OLED display, I2C_CL: GPIO22, I2C_DA: GPIO21
// Infrared receiving pin
#define recvPin 2    

#ifndef LED_BUILTIN
  #define LED_BUILTIN 27
#endif
#define LED_RED 25
#define LED_GREEN 33
#define LED_BLUE 32

#define KEY_0 16
#define KEY_1 17

#define POT0 A6
#define POT1 A7
#endif // _BoardPin_
