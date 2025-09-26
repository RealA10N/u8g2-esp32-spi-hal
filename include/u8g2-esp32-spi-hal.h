#ifndef U8G2_ESP32_SPI_HAL_H
#define U8G2_ESP32_SPI_HAL_H

#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "soc/gpio_num.h"
#include "u8g2.h"

#include <stdint.h>

// MARK: User Defines

#ifndef U8G2_ESP32_SPI_HAL__MAX_TRANSFER_SZ
#define U8G2_ESP32_SPI_HAL__MAX_TRANSFER_SZ 4096
#endif

#ifndef U8G2_ESP32_SPI_HAL__SPI_HOST
#define U8G2_ESP32_SPI_HAL__SPI_HOST SPI2_HOST
#endif

#ifndef U8G2_ESP32_SPI_HAL__CLOCK_SPEED_HZ
#define U8G2_ESP32_SPI_HAL__CLOCK_SPEED_HZ 1000000 /* 1MHz - safe speed */
#endif

// MARK: Types

typedef struct {
  uint8_t dc;    // Data/Command pin (Usually labeled DC)
  uint8_t reset; // Reset pin (Usually labeled RST)
  uint8_t clk;   // SPI Clock pin (Sometimes labeled SCK or SCL)
  uint8_t mosi;  // SPI MOSI pin (Sometimes labeled SDA or SDI)
  uint8_t cs;    // SPI Chip Select pin (Sometimes labeled SS or CS)
} u8g2_esp32_spi_hal_t;

#define U8G2_ESP32_SPI_HAL__UNDEFINED GPIO_NUM_NC

#define U8G2_ESP32_SPI_HAL__DEFAULT                                            \
  {                                                                            \
      .dc = U8G2_ESP32_SPI_HAL__UNDEFINED,                                     \
      .reset = U8G2_ESP32_SPI_HAL__UNDEFINED,                                  \
      .clk = U8G2_ESP32_SPI_HAL__UNDEFINED,                                    \
      .mosi = U8G2_ESP32_SPI_HAL__UNDEFINED,                                   \
      .cs = U8G2_ESP32_SPI_HAL__UNDEFINED,                                     \
  }

// MARK: Prototypes

void u8g2_esp32_spi_hal_init(u8g2_esp32_spi_hal_t hal);

uint8_t u8g2_spi_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                         void *arg_ptr);

uint8_t u8g2_gpio_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                           void *arg_ptr);

#endif // U8G2_ESP32_SPI_HAL_H