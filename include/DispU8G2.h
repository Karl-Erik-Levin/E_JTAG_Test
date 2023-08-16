#ifndef _DispU8G2_
#define _DispU8G2_

void DispInit(void);
void DispPrint(uint8_t line, char *message);
void DispPage(char *text, uint8_t row, uint8_t font, uint8_t selected);
void DispBuffer(void);
void DispClear(void);

#endif // _DispU8G2_
