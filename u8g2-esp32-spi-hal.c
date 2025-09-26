#include "u8g2-esp32-spi-hal.h"

// MARK: Globals

static u8g2_esp32_spi_hal_t u8g2_esp32_hal = U8G2_ESP32_SPI_HAL__DEFAULT;
static spi_device_handle_t spi_handle;

void u8g2_esp32_spi_hal_init(u8g2_esp32_spi_hal_t hal) { u8g2_esp32_hal = hal; }

// MARK: Callbacks

uint8_t u8g2_spi_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                         void *arg_ptr) {
  switch (msg) {
  case U8X8_MSG_BYTE_SET_DC:
    gpio_set_level(u8g2_esp32_hal.dc, arg_int);
    break;

  case U8X8_MSG_BYTE_INIT: {
    spi_bus_config_t bus_config = {
        .sclk_io_num = u8g2_esp32_hal.clk,
        .mosi_io_num = u8g2_esp32_hal.mosi,
        .miso_io_num = GPIO_NUM_NC,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = U8G2_ESP32_SPI_HAL__MAX_TRANSFER_SZ,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(U8G2_ESP32_SPI_HAL__SPI_HOST,
                                       &bus_config, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t dev_config = {
        .address_bits = 0,
        .command_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .duty_cycle_pos = 0,
        .cs_ena_posttrans = 0,
        .cs_ena_pretrans = 0,
        .clock_speed_hz = U8G2_ESP32_SPI_HAL__CLOCK_SPEED_HZ,
        .spics_io_num = u8g2_esp32_hal.cs,
        .flags = 0,
        .queue_size = 200,
        .pre_cb = NULL,
        .post_cb = NULL};

    ESP_ERROR_CHECK(spi_bus_add_device(U8G2_ESP32_SPI_HAL__SPI_HOST,
                                       &dev_config, &spi_handle));
    break;
  }

  case U8X8_MSG_BYTE_SEND: {
    spi_transaction_t trans = {
        .length = 8 * arg_int, // Length in bits
        .tx_buffer = arg_ptr,
        .rx_buffer = NULL,
    };

    ESP_ERROR_CHECK(spi_device_transmit(spi_handle, &trans));
    break;
  }
  }
  return 0;
}

uint8_t u8g2_gpio_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                           void *arg_ptr) {
  switch (msg) {
  case U8X8_MSG_GPIO_AND_DELAY_INIT: {
    // Initialize GPIO pins
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << u8g2_esp32_hal.dc) |
                        (1ULL << u8g2_esp32_hal.reset) |
                        (1ULL << u8g2_esp32_hal.cs),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);
    break;
  }

  case U8X8_MSG_GPIO_RESET:
    gpio_set_level(u8g2_esp32_hal.reset, arg_int);
    break;

  case U8X8_MSG_GPIO_CS:
    gpio_set_level(u8g2_esp32_hal.cs, arg_int);
    break;

  case U8X8_MSG_DELAY_MILLI:
    // Assuming running in a FreeRTOS environment
    vTaskDelay(pdMS_TO_TICKS(arg_int));
    break;
  }
  return 0;
}
