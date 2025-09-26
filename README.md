# u8g2-esp-spi-hal

A simple abstraction layer to easily use the [u8g2](https://github.com/olikraus/u8g2)
library for monochrome displays with esp32 micro-controllers and the esp-idf
framework.

Tested on the following drivers and displays:

- SH1122: GME25664-65

## Getting Started

First, add the *u8g2 library* as a component to your *esp-idf* project, by
copying the repository contents to `components/u8g2` in your project, or by
adding the [u8g2 repository](https://github.com/olikraus/u8g2) as a
*git submodule*:

```shell
git submodule add https://github.com/olikraus/u8g2.git components/u8g2
```

Then, similarly, add `u8g2-esp32-spi-hal` as a component to your *esp-idf* project:

```shell
git submodule add https://github.com/reala10n/u8g2-esp32-spi-hal.git components/u8g2-esp32-spi-hal
```

Now, update your main `CMakeLists.txt` (or the `CMakeLists.txt` of another
component that controls the `u8g2` display) to add both libraries as dependencies:

```cmake
idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES u8g2 u8g2-esp32-spi-hal)
```

Finally, `#include` both libraries in your `.c` file, configure the
`u8g2_esp32_spi_hal_t` according to your pin configuration, and start using
`u8g2` natively!

```c
#include <stdio.h>

#include "u8g2-esp32-spi-hal.h"
#include "u8g2.h"

void app_main(void) {
  u8g2_esp32_spi_hal_t u8g2_spi_hal = {
      .dc = 33,    // Data/Command pin (Usually labeled DC)
      .reset = 25, // Reset pin (Usually labeled RST)
      .clk = 27,   // SPI Clock pin (Sometimes labeled SCK or SCL)
      .mosi = 26,  // SPI MOSI pin (Sometimes labeled SDA or SDI)
      .cs = 32,    // SPI Chip Select pin (Sometimes labeled SS or CS)
  };

  // Initialize the HAL configuration internally.
  u8g2_esp32_spi_hal_init(u8g2_spi_hal);

  // Initialize the your u8g2 display!
  // IMPORTANT: make sure to use the 'u8g2_spi_byte_cb' and 'u8g2_gpio_delay_cb'
  // functions here: they are defined in the u8g2-esp32-spi-hal component.
  u8g2_t u8g2;
  u8g2_Setup_sh1122_256x64_f(&u8g2, U8G2_R0, u8g2_spi_byte_cb,
                             u8g2_gpio_delay_cb);

  // From now on, you can control the display as usual using the u8g2 API!

  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_ClearDisplay(&u8g2);

  u8g2_SetFont(&u8g2, u8g2_font_ncenB18_tr);

  u8g2_DrawStr(&u8g2, 0, 24, "Hello World!");
  u8g2_SendBuffer(&u8g2);
}
```
