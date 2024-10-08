// ===================================================================================
// Project:   Example for PY32F0xx
// Version:   v1.0
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// SSD1306 128x64 Pixels I2C OLED graphics demo. 
// Connect an SSD1306 128x64 Pixels I2C OLED to PF0 (SDA) and PF1 (SCL).
//
// References:
// -----------
// - Neven Boyanov:         https://github.com/tinusaur/ssd1306xled
// - Stephen Denne:         https://github.com/datacute/Tiny4kOLED
// - David Johnson-Davies:  http://www.technoblogy.com/show?3AJ7
// - OLED Font Editor:      http://sourpuss.net/projects/fontedit/
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-arm-none-eabi) and Python3 with puyaisp is
//   installed. If necessary, a driver for the USB-to-serial converter used must
//   be installed.
// - Connect the USB-to-serial converter to the MCU board.
// - Run 'make flash'.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <ssd1306_gfx.h>                            // SSD1306 OLED graphics functions

// ===================================================================================
// Pseudo Random Number Generator
// ===================================================================================
uint32_t random(uint32_t max) {
  static uint32_t rnval = 0xACE1DFEE;
  rnval = rnval << 16 | (rnval << 1 ^ rnval << 2) >> 16;
  return(rnval % max);
}

// ===================================================================================
// Bitmaps
// ===================================================================================
const uint8_t UFO[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x30, 0x08, 0x04, 0x3A, 0xFA, 0x39, 0x01,
  0x01, 0x31, 0xF8, 0xFA, 0x12, 0x04, 0x18, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0C, 0x3C, 0x7E, 0x7A, 0xF9, 0xFF, 0xFF, 0xCF, 0xC7, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0x8E,
  0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFA, 0x7A, 0x7E, 0x3C, 0x08
};

const uint8_t BAT_OK[] = {
  0x7C, 0x04, 0x07, 0x01, 0x07, 0x84, 0xDC, 0x73, 0x46, 0x4C, 0x46, 0x43, 0x41, 0x7C
};

const uint8_t ANT[] = {
  0x01, 0x02, 0x04, 0x7F, 0x04, 0x02, 0x01
};

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Variables
  uint16_t i;
  
  // Setup
  OLED_init();

  // Loop
  while(1) {
    OLED_clear();
    for(i=200; i; i--) OLED_setPixel(random(OLED_WIDTH), random(OLED_HEIGHT), 1);
    for(i=32; i; i--) OLED_drawCircle(100, 50, i, 1);
    OLED_drawSprite(20, 10, 32, 16, UFO);
    OLED_refresh();
    DLY_ms(2000);

    OLED_clear();
    OLED_cursor( 0,  0); OLED_textsize(1);            OLED_print("Hello World");
    OLED_cursor( 0, 32); OLED_textsize(4);            OLED_print("TEST");
    OLED_cursor( 0, 12); OLED_textsize(OLED_STRETCH); OLED_print("1234567890");
    OLED_cursor(64, 12); OLED_textsize(OLED_SMOOTH);  OLED_print("12345");
    OLED_refresh();
    DLY_ms(2000);
    for(i=0; i<=64; i++) {
      OLED_vscroll(i);
      DLY_ms(25);
    }
    DLY_ms(1000);

    uint8_t strength = 32;
    uint8_t volume = 10;
    OLED_clear();
    OLED_cursor(0, 0); OLED_print("FM Radio");
    OLED_drawBitmap(121, 0, 7, 16, BAT_OK);
    OLED_cursor(-10, 20); OLED_printSegment(10885, 5, 1, 2);
    OLED_cursor( 94, 36); OLED_print("MHz");
    OLED_drawBitmap(94, 20, 7, 8, ANT);
    OLED_drawRect(104, 20, 24, 7, 1);
    if(strength > 64) strength = 64;
    strength = (strength >> 2) + (strength >> 3);
    if(strength) OLED_fillRect(104, 20, strength, 7, 1);
    OLED_cursor(0, 56); OLED_textsize(1); OLED_print("Volume:");
    OLED_drawRect(50, 56, 78, 7, 1);
    uint8_t xpos = 47;
    while(volume--) OLED_fillRect(xpos+=5, 58, 4, 3, 1);
    OLED_refresh();
    DLY_ms(2000);

    OLED_clear();
    OLED_fillCircle(32, 32, 30, 1);
    OLED_drawCircle(64 + 32, 32, 30, 1);
    OLED_refresh();
    DLY_ms(2000); OLED_invert(!OLED_INVERT);
    DLY_ms(2000); OLED_invert( OLED_INVERT);
    DLY_ms(2000); OLED_flip(0, 1);
    DLY_ms(2000); OLED_flip(1, 1);

    OLED_clear();
    OLED_fillRect(0, 0, 60, 60, 1);
    OLED_drawRect(64, 0, 60, 60, 1);
    OLED_refresh();
    DLY_ms(2000);

    OLED_clear();
    for(i = 200; i; i--) {
      OLED_drawLine(random(OLED_WIDTH), random(OLED_HEIGHT), random(OLED_WIDTH), random(OLED_HEIGHT), 1);
      OLED_refresh();
    }

    OLED_clear();
    for(i = 200; i; i--) {
      OLED_drawRect(random(OLED_WIDTH), random(OLED_HEIGHT), random(OLED_WIDTH/2), random(OLED_HEIGHT/2), 1);
      OLED_refresh();
    }

    OLED_clear();
    for(i = 200; i; i--) {
      OLED_drawCircle(random(OLED_WIDTH), random(OLED_HEIGHT), random(16), 1);
      OLED_refresh();
    }

    for(i = 200; i; i--) {
      OLED_cursor(random(OLED_WIDTH), random(OLED_HEIGHT));
      OLED_textinvert(random(2)); OLED_textsize(random(3) + 1);
      OLED_print("Hello");
      OLED_refresh();
    }
    OLED_textinvert(0);
  }
}
