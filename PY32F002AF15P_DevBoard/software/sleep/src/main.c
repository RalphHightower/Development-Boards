// ===================================================================================
// Project:   Example for PY32F002A
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
// Blink built-in LED using deep sleep with LPTIM as wake-up timer.
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-arm-none-eabi) and Python3 with puyaisp is
//   installed. If necessary, a driver for the USB-to-serial converter used must
//   be installed.
// - Connect your MCU board via USB to your PC.
// - Set the MCU to boot mode by holding down the BOOT key and then pressing and 
//   releasing the RESET key. Finally release the BOOT key.
// - Run 'make flash'.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include "system.h"               // system functions
#include "gpio.h"                 // GPIO functions

#define PIN_LED   PB0             // define LED pin

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Setup
  PIN_output(PIN_LED);            // set LED pin as output
  LPT_init();                     // init low-power timer

  // Loop
  while(1) {
    PIN_toggle(PIN_LED);          // toggle LED on/off
    LPT_stop(100);                // set device into STOP (deep sleep) for 100ms
  }
}
