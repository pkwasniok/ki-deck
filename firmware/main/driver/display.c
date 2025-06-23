#include "display.h"

#include "esp_log.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"

#define TAG "display"

display_err_t display_init_spi(display_config_t* config, display_handle_t* display);

display_err_t display_init(display_config_t* config, display_handle_t* display) {
    int ret;

    // Initialize RES gpio
    gpio_set_direction(config->gpio_num_res, GPIO_MODE_OUTPUT);
    gpio_set_level(config->gpio_num_res, 0);

    // Initialize C/D gpio
    gpio_set_direction(config->gpio_num_cd, GPIO_MODE_OUTPUT);
    gpio_set_level(config->gpio_num_cd, 0);

    // Initialize SPI device
    ret = display_init_spi(config, display);

    if (ret != DISPLAY_OK) {
        ESP_LOGE(TAG, "Error occured during display initialization");
        return ret;
    }

    ESP_LOGI(TAG, "Finished display initialization");
    return DISPLAY_OK;
}

display_err_t display_init_spi(display_config_t* config, display_handle_t* display) {
    int ret;

    spi_device_interface_config_t spi_config = {
        .spics_io_num = config->gpio_num_cs,
        .clock_speed_hz = 10 * 1000 * 1000,
        .mode = 0,
        .queue_size = 1,
    };

    ret = spi_bus_add_device(config->spi_host, &spi_config, &display->spi_device);

    if (ret != ESP_OK) {
        return DISPLAY_ERR_INIT;
    }

    return DISPLAY_OK;
}

