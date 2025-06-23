#include "config.h"
#include "utils/delay.h"

#include "display/display.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"

void app_main(void) {
    int ret;

    // Initialize SPI master

    spi_bus_config_t spi_config = {
        .mosi_io_num = CONFIG_OLED_IONUM_SDA,
        .miso_io_num = -1,
        .sclk_io_num = CONFIG_OLED_IONUM_SCK,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
    };

    ret = spi_bus_initialize(SPI2_HOST, &spi_config, SPI_DMA_DISABLED);
    ESP_ERROR_CHECK(ret);

    // Initialize display

    display_handle_t display;
    display_config_t display_config = {
        .spi_host = SPI2_HOST,
        .gpio_num_cs = CONFIG_OLED_IONUM_CS,
        .gpio_num_res = CONFIG_OLED_IONUM_RES,
        .gpio_num_cd = CONFIG_OLED_IONUM_CD,
    };

    ret = display_init(&display_config, &display);
    assert(ret == DISPLAY_OK);

    ret = display_clear(&display);
    assert(ret == DISPLAY_OK);

    while (1) {
        delay_s(1);
    }
}

