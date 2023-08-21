#include <Wire.h>
#include <math.h>
#include "GY521.h"

void getAngle(int Ax, int Ay, int Az);

const int MPU = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int AcXcal, AcYcal, AcZcal, GyXcal, GyYcal, GyZcal, tcal;
double t, tx, tf, pitch, roll;

void GY521Init(void) {
//  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void GY521Read(void) {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);
  AcXcal = -75;
  AcYcal = 270;
  AcZcal = -8300;
  tcal = -1600;
  GyXcal = 650;
  GyYcal = -140;
  GyZcal = -50;
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  tx = Tmp + tcal;
  t = tx / 340 + 36.53;
  tf = (t * 9 / 5) + 32;
  getAngle(AcX, AcY, AcZ);
}

void getAngle(int Ax, int Ay, int Az) {
  double x = Ax;
  double y = Ay;
  double z = Az;
  pitch = atan(x / sqrt((y * y) + (z * z)));
  roll = atan(y / sqrt((x * x) + (z * z)));
  pitch = pitch * (180.0 / 3.14);
  roll = roll * (180.0 / 3.14) ;
}

double GY521Pitch(void) {
  return pitch;
}

double GY521Roll(void) {
  return roll;
}

int32_t GY521AcX(void) {
  return (AcX + AcXcal);
}

int32_t GY521AcY(void) {
  return (AcY + AcYcal);
}

int32_t GY521AcZ(void) {
  return (AcZ + AcZcal);
}

int32_t GY521GyX(void) {
  return (GyX + GyXcal);
}

int32_t GY521GyY(void) {
  return (GyY + GyYcal);
}

int32_t GY521GyZ(void) {
  return (GyZ + GyZcal);
}

/*
void GY521Print(void) {
  Serial.print("Angle: ");
  Serial.print("Pitch = ");
  Serial.print(pitch);
  Serial.print(" Roll = ");
  Serial.println(roll);
  Serial.print("Accelerometer: ");
  Serial.print("X = ");
  Serial.print(AcX + AcXcal);
  Serial.print(" Y = ");
  Serial.print(AcY + AcYcal);
  Serial.print(" Z = ");
  Serial.println(AcZ + AcZcal);
  Serial.print("Temperature in celsius = ");
  Serial.print(t);
  Serial.print(" fahrenheit = ");
  Serial.println(tf);
  Serial.print("Gyroscope: ");
  Serial.print("X = ");
  Serial.print(GyX + GyXcal);
  Serial.print(" Y = ");
  Serial.print(GyY + GyYcal);
  Serial.print(" Z = ");
  Serial.println(GyZ + GyZcal);
} */
