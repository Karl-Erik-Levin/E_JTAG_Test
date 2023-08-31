#ifndef _DispU8G2_
#define _DispU8G2_

typedef struct {
  char message[30];
  uint8_t row;
  uint8_t font;
  bool selec;
} dispRow;

void DispInit(void);
void DispPrint(uint8_t line, char *message);
//void DispRow(char *text, uint8_t row, uint8_t font, uint8_t selected);
void DispPage(dispRow row[]);
void DispClock(uint8_t hour, uint8_t min, uint8_t sec);
void DispBuffer(void);
void DispClear(void);

#endif // _DispU8G2_
