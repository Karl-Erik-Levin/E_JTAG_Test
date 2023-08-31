#include <Wire.h>
#include <math.h>
#include "GY521.h"

void getAngle(int Ax, int Ay, int Az);

const int MPU = 0x68;
const uint8_t numSamp = 8;
//const int AcXcal=-75, AcYcal=270, AcZcal=-7700;
const int AcXcal=0, AcYcal=0, AcZcal=-1391;
const int GyXcal=610, GyYcal=-140, GyZcal=-60, tcal=-6370;
int16_t AcX, AcY, AcZ;
int16_t Tmp, GyX, GyY, GyZ;
int16_t AcXsamp[numSamp], AcYsamp[numSamp], AcZsamp[numSamp];
int AcXtot, AcYtot, AcZtot;
uint8_t samp;
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

  AcXsamp[samp%numSamp]=AcX, AcYsamp[samp%numSamp]=AcY, AcZsamp[samp%numSamp]=AcZ;
  AcXtot = AcYtot = AcZtot = 0;
  for (uint8_t i=0; i<numSamp; i++) {
    AcXtot += AcXsamp[i], AcYtot += AcYsamp[i], AcZtot += AcZsamp[i];
  }
  AcX = AcXtot / numSamp, AcY = AcYtot /numSamp, AcZ = AcZtot /numSamp;
  samp++;
}

void getAngle(int Ax, int Ay, int Az) {
  double x = Ax, y = Ay, z = Az;
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

double GY521Resultant(void) {
  double x = GY521AcX(), y = GY521AcY(), z = GY521AcZ();
  double r = sqrt((x * x) + (y * y));
  return (sqrt((z * z) + (r * r)));
}

double GY521AcX(void) {
  return ((double) AcX + AcXcal) / 1638;
}

double GY521AcY(void) {
  return ((double) AcY + AcYcal) / 1638;
}

double GY521AcZ(void) {
  return ((double) AcZ + AcZcal) / 1638;
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

float GY521TempC(void) {
  return (t);
}

float GY521TempF(void) {
  return (tf);
}

