#include "config.h"
#include "utils/delay.h"

#include "display/display.h"
#include "graphics/graphics.h"
#include "graphics/geometry.h"

#include "driver/spi_common.h"
#include "driver/spi_master.h"

display_handle_t display;
uint8_t display_buffer[1024];

void display_update_handler(void) {
    for (int page = 0; page < 8; page++) {
        for (int seg = 0; seg < 128; seg++) {
            uint8_t com = 0;

            for (int bit = 0; bit < 8; bit++) {
                if (g_matrix[seg][page * 8 + bit] == 255) {
                    com |= 1 << bit;
                }
            }

            display_buffer[(page * 128) + seg] = com;
        }
    }

    display_write_data(&display, display_buffer, 1024);
}

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

    ret = spi_bus_initialize(SPI2_HOST, &spi_config, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    // Initialize display

    display_config_t display_config = {
        .spi_host = SPI2_HOST,
        .gpio_num_cs = CONFIG_OLED_IONUM_CS,
        .gpio_num_res = CONFIG_OLED_IONUM_RES,
        .gpio_num_cd = CONFIG_OLED_IONUM_CD,
        .flags = DISPLAY_FLAG_FLIP_HORIZONTAL | DISPLAY_FLAG_FLIP_VERTICAL,
    };

    ret = display_init(&display_config, &display);
    assert(ret == DISPLAY_OK);

    ret = display_clear(&display);
    assert(ret == DISPLAY_OK);

    // Initialize graphics

    g_init();
    g_register_event_handler(G_EVENT_UPDATE, display_update_handler);

    // Main loop

    while (1) {
        g_clear();

        g_line(20, 0, 20, 63);
        g_line(60, 0, 60, 63);

        g_line(22, 32, 58, 32 - 6);
        g_line(58, 34 + 6, 22, 34);

        g_update();

        delay_ms(10);
    }
}

