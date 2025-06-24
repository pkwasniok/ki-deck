#include "config.h"
#include "utils/delay.h"

#include "display/display.h"
#include "graphics/graphics.h"
#include "graphics/geometry.h"
#include "graphics/image.h"

#include <stdio.h>
#include <math.h>

#include "esp_littlefs.h"
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

    // Initialize LittleFS

    esp_vfs_littlefs_conf_t littlefs_config = {
        .base_path = "/files",
        .partition_label = "files",
        .format_if_mount_failed = true,
        .dont_mount = false,
    };

    ret = esp_vfs_littlefs_register(&littlefs_config);
    assert(ret == ESP_OK);

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

    g_clear();

    g_image(16, 16, "/files/logo.pbm", G_IMAGE_FORMAT_PBM);

    g_update();

    // int t = 0;
    
    while (1) {
        // g_clear();
        //
        // for (int x = 0; x < 127; x++) {
        //     if (x % 2 == 0)
        //         g_point(x, 32);
        // }
        //
        // for (int y = 0; y < 64; y++) {
        //     if (y % 2 == 0)
        //         g_point(64, y);
        // }
        //
        // int a = 24 * sin(t * 0.022);
        // int prev_x = 0;
        // int prev_y = 32;
        // for (int x = 0; x < 127; x++) {
        //     int y = (a * sin((x + t) * 0.0498)) + 32;
        //
        //     if (x == 0)
        //         g_point(x, y);
        //     else
        //         g_line(prev_x, prev_y, x, y);
        //
        //     prev_x = x;
        //     prev_y = y;
        // }
        //
        // g_update();
        //
        // t += 1;
        // if (t > 286)
        //     t = 0;

        delay_ms(10);
    }
}

