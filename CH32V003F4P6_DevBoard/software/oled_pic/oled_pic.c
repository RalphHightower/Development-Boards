// ===================================================================================
// Project:   Example for CH32V003
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
// Connect an SSD1306 128x64 Pixels I2C OLED to PC1 (SDA) and PC2 (SCL). MCU sends
// a picture to the OLED via I2C utilizing DMA.
//
// References:
// -----------
// - CNLohr ch32v003fun: https://github.com/cnlohr/ch32v003fun
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-riscv64-unknown-elf, newlib) and Python3 with PyUSB
//   is installed. In addition, Linux requires access rights to WCH-LinkE programmer.
// - Connect the WCH-LinkE programmer to the MCU board.
// - Run 'make flash'.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include "system.h"               // system functions
#include "i2c_dma.h"              // functions with DMA

// Picture
const uint8_t pic[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xF0, 0xFE, 0xFE,
0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xF0, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
0x00, 0x10, 0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x0E, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0E, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xF0, 0xEC,
0xCE, 0xE2, 0xF0, 0x78, 0x38, 0x1C, 0x14, 0x14, 0x1C, 0x18,
0x38, 0x70, 0xE0, 0xE0, 0xC0, 0xC0, 0xE0, 0xE0, 0x60, 0x60,
0xE0, 0xE0, 0xE0, 0xC0, 0x83, 0x87, 0xE6, 0xF0, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF8,
0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0x3F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03,
0x07, 0x07, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xF8, 0xEB, 0xFA, 0xFF, 0xFF, 0xFF,
0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xF8,
0xF1, 0xFF, 0xFF, 0x71, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0xC7, 0xFF, 0xFF, 0xFF, 0x3F, 0xF9, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0x3F, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,
0xF0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0,
0xF0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0x3F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xBE, 0x7F, 0xFF, 0xFF, 0xFF, 0x63, 0x07, 0x0F, 0x0E,
0x04, 0x00, 0x00, 0x08, 0x1E, 0x3F, 0x7F, 0xFC, 0xFC, 0xF8,
0xF8, 0xF0, 0x71, 0x10, 0x0E, 0x00, 0x00, 0x02, 0x07, 0x01,
0x1F, 0x3F, 0xFF, 0xF5, 0xEE, 0xEF, 0xFF, 0xF8, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xF8, 0xFE, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0xFF, 0xFF, 0x7F, 0x0F, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x7F,
0xFF, 0xFF, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
0xF8, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0,
0x80, 0xC0, 0xE0, 0x32, 0x38, 0x38, 0x3E, 0xFF, 0xFF, 0xF3,
0xF8, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x0F, 0x3F, 0x87, 0x9F, 0xC0, 0xFE, 0x3C, 0x38,
0x70, 0x30, 0x64, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0xC0, 0xC0, 0x80, 0x00, 0x80, 0x80, 0x80,
0xC0, 0x80, 0x00, 0x00, 0x80, 0xC1, 0xC3, 0x83, 0x01, 0x81,
0xC0, 0x80, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80,
0xC0, 0xC0, 0x80, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0x80, 0x00,
0x80, 0xC0, 0x00, 0x01, 0x81, 0x83, 0x03, 0xC1, 0x80, 0xC0,
0xC0, 0x00, 0x80, 0xC0, 0xC0, 0x80, 0x00, 0x80, 0x80, 0xC0,
0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x63, 0xF5, 0xFF, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x8F, 0xE7, 0x7F, 0x0F, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
0x1F, 0x18, 0x18, 0x0F, 0x07, 0xFF, 0xFF, 0x18, 0x18, 0x1F,
0x0F, 0x07, 0x1F, 0x1A, 0x1A, 0x1B, 0x03, 0x0F, 0x1F, 0x00,
0x00, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0xC9, 0xDB, 0x1A, 0x1E,
0x1E, 0x0C, 0x0F, 0x1F, 0x18, 0x18, 0x1F, 0x07, 0x07, 0x1F,
0x18, 0x18, 0x1F, 0x1F, 0x00, 0x1F, 0x1F, 0x00, 0x00, 0x0F,
0x1F, 0x18, 0x18, 0x18, 0x06, 0x0F, 0x1A, 0x1A, 0x1B, 0x0B,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0x40, 0x20, 0x11, 0x13, 0x08, 0x08, 0x08,
0x18, 0x18, 0x18, 0x38, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xE0, 0xF0,
0xF0, 0xF8, 0x78, 0x38, 0x18, 0x18, 0x08, 0x08, 0x08, 0x0F,
0x13, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x3F, 0x3F, 0x03, 0x03, 0x3F, 0x3E, 0x30,
0x3B, 0x2B, 0x2B, 0x3F, 0x3E, 0x00, 0x3F, 0x03, 0x03, 0x03,
0x3E, 0x3F, 0x23, 0x23, 0x3F, 0x3F, 0x07, 0x3F, 0x38, 0x3E,
0x07, 0x3E, 0x38, 0x1F, 0x03, 0x38, 0x3B, 0x2B, 0x2F, 0x3E,
0x00, 0x3F, 0x3F, 0x03, 0x03, 0x01, 0x3E, 0x3F, 0x2B, 0x2F,
0x2E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
0x3E, 0x3E, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1F, 0x0F,
0x04, 0x00, 0x04, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3E, 0x3E, 0x3E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
0x1E, 0x1E, 0x1F, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00 };

// ===================================================================================
// SSD1306 128x64 Pixels OLED Definitions
// ===================================================================================

// OLED definitions
#define OLED_ADDR         0x78    // OLED write address (0x3C << 1)
#define OLED_CMD_MODE     0x00    // set command mode
#define OLED_DAT_MODE     0x40    // set data mode

// OLED initialisation sequence
const uint8_t OLED_INIT_CMD[] = {
  0xA8, 0x3F,                     // set multiplex ratio  
  0x8D, 0x14,                     // set DC-DC enable  
  0x20, 0x00,                     // set horizontal addressing mode
  0x21, 0x00, 0x7F,               // set start and end column
  0x22, 0x00, 0x3F,               // set start and end page
  0xDA, 0x12,                     // set com pins
  0xA1, 0xC8,                     // flip screen
  0xAF                            // display on
};

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Init OLED
  I2C_init();                             // initialize I2C first
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_CMD_MODE);               // set command mode
  I2C_writeBuffer((uint8_t*)OLED_INIT_CMD, sizeof(OLED_INIT_CMD)); // send init sequence
  
  // Write picture
  I2C_start(OLED_ADDR);                   // start transmission to OLED
  I2C_write(OLED_DAT_MODE);               // set command mode
  I2C_writeBuffer((uint8_t*)pic, 1024);   // send picture using DMA and stop

  // Loop
  while(1);                               // nothing to be done
}
