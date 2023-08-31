#ifndef _GY521_
#define _GY521_

void GY521Init(void);
void GY521Read(void);

double GY521Pitch(void);
double GY521Roll(void);
double GY521Resultant(void);

double GY521AcX(void);
double GY521AcY(void);
double GY521AcZ(void);

int32_t GY521GyX(void);
int32_t GY521GyY(void);
int32_t GY521GyZ(void);

float GY521TempC(void);
float GY521TempF(void);

void GY521Print(void);

#endif // _GY521_
