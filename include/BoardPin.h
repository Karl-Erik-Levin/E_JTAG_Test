#ifndef _BoardPin_
#define _BoardPin_

// I2C_CL: GPIO 22, I2C_DA: GPIO 21
// On I2C bus is 0.96/1.30" OLED display 
// BMP180, Air pressure and tempertur sensor
// GY521, MPU-6050 3-axis gyroscope and acceleration_

// Infrared receiving pin
#define RECV_PIN 26     // GPIO 26

#ifndef LED_BUILTIN
#define LED_BUILTIN 27  // GPIO 27
#endif
#define LED_RED 25      // GPIO 25
#define LED_GREEN 33    // GPIO 33
#define LED_BLUE 32     // GPIO 32

#define KEY_0 16        // GPIO 16
#define KEY_1 17        // GPIO 17
#define KEY_2  4        // GPIO 4

#define POT0 A6         // ADC1-6 = GPIO 34
#define POT1 A7         // ADC1-7 = GPIO 35
#endif // _BoardPin_
