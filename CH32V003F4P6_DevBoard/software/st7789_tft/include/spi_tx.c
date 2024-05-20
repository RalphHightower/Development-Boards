// ===================================================================================
// Basic SPI Master Functions (TX only) for CH32V003                          * v1.0 *
// ===================================================================================
// 2023 by Stefan Wagner:   https://github.com/wagiminator

#include"spi_tx.h"

// Init SPI
void SPI_init(void) {
  // Enable GPIO and SPI module clock
  RCC->APB2PCENR |= RCC_AFIOEN | RCC_IOPCEN | RCC_SPI1EN;
  
  // Setup GPIO pins PC5 (SCK) and PC6 (MOSI)
  GPIOC->CFGLR  = (GPIOC->CFGLR & ~(((uint32_t)0b1111<<(5<<2)) | ((uint32_t)0b1111<<(6<<2)) ))
                                |  (((uint32_t)0b1001<<(5<<2)) | ((uint32_t)0b1001<<(6<<2)) );

  // Setup and enable SPI master, standard configuration
  SPI1->CTLR1 = (SPI_PRESC << 3)      // set prescaler
              | SPI_CTLR1_MSTR        // master configuration
              | SPI_CTLR1_BIDIMODE    // one-line mode
              | SPI_CTLR1_BIDIOE      // transmit only
              | SPI_CTLR1_SSM         // software control of NSS
              | SPI_CTLR1_SSI         // set internal NSS high
              | SPI_CTLR1_SPE;        // enable SPI
}

// Transmit one data byte
void SPI_write(uint8_t data) {
  while(!SPI_ready());                // wait for ready to write
  SPI1->DATAR = data;                 // send data byte
}
