#ifndef _GY521_
#define _GY521_

void GY521Init(void);
void GY521Read(void);

double GY521Pitch(void);
double GY521Roll(void);

int32_t GY521AcX(void);
int32_t GY521AcY(void);
int32_t GY521AcZ(void);

int32_t GY521GyX(void);
int32_t GY521GyY(void);
int32_t GY521GyZ(void);

void GY521Print(void);

#endif // _GY521_
