#include <U8g2lib.h>
#include "DispU8G2.h"

// For U8g2lib, I2C_CL: GPIO22, I2C_DA: GPIO21
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);            // For 1.30" display
  U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);           // For 0.96" display


void DispInit(void) {
  u8g2.begin();                             // start display
  u8g2.setFont(u8g2_font_ncenR10_tf);       // choose a suitable font
}

void DispPrint(uint8_t line, char *message) {
  int yPos = line*16 + 13;

  if (yPos > 63) yPos = 63;
  u8g2.drawStr(0, yPos, message);
}

void DispBuffer(void) {
  u8g2.sendBuffer();
}

void DispClear(void) {
  u8g2.clearBuffer();
}

void DispPage(char *text, uint8_t row, uint8_t font, uint8_t selected) {
  if (font==0)
    u8g2.setFont(u8g2_font_ncenB10_tf);
  else if (font==1)
    u8g2.setFont(u8g2_font_ncenR10_tf);
  else if (font==2)
    u8g2.setFont(u8g2_font_helvR10_tf);
  else if (font==3)
    u8g2.setFont(u8g2_font_courR10_tf);
  
  if (selected == 1) {
    u8g2.drawBox(0, row*16, 128, 16);
    u8g2.setFontMode(1);
    u8g2.setDrawColor(2);
  }

  DispPrint(row, text);
  
  if (selected == 1) {
    u8g2.setDrawColor(1);
    u8g2.setFontMode(0);
  }
}

void DispClock(uint8_t hour, uint8_t minute, uint8_t sec) {
  char mess[20];

  sprintf(mess, "%2d:%02d:%02d", hour, minute, sec);
  u8g2.setFont(u8g2_font_mystery_quest_36_tn);
  u8g2.drawStr(0, 62, mess);
}


#if 0
char mess[] = {197, 32, 196, 32, 214, 32, 229, 32, 228, 32, 246, 0};     // Å Ä Ö å ä ö
u8g2.drawStr( 0, 0, mess);
#endif
