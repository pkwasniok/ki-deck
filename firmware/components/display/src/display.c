#include "display/display.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"

#define TAG "display"

display_err_t display_init_spi(display_config_t* config, display_handle_t* display);
display_err_t display_init_driver(display_config_t* config, display_handle_t* display);

display_err_t display_init(display_config_t* config, display_handle_t* display) {
    int ret;

    // Initialize handler
    display->gpio_num_res = config->gpio_num_res;
    display->gpio_num_cd = config->gpio_num_cd;

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

    // Initialize driver
    ret = display_init_driver(config, display);

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
        return DISPLAY_ERR_IO;
    }

    return DISPLAY_OK;
}

display_err_t display_init_driver(display_config_t* config, display_handle_t* display) {
    int ret;

    // Reset driver
    gpio_set_level(config->gpio_num_res, 0);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(config->gpio_num_res, 1);
    vTaskDelay(400 / portTICK_PERIOD_MS);

    // Disable display (0xAE)
    ret = display_write_command(display, 0xAE);
    assert(ret == DISPLAY_OK);

    // Charge pump (0x8D, 0x14)
    ret = display_write_command(display, 0x8D);
    assert(ret == DISPLAY_OK);
    ret = display_write_command(display, 0x14);
    assert(ret == DISPLAY_OK);

    // Addressing mode: horizontal (0x20, 0x00)
    ret = display_write_command(display, 0x20);
    assert(ret == DISPLAY_OK);
    ret = display_write_command(display, 0x00);
    assert(ret == DISPLAY_OK);

    // Display mode: normal (0xA6)
    ret = display_write_command(display, 0xA6);
    assert(ret == DISPLAY_OK);

    // Enable display (0xAF)
    ret = display_write_command(display, 0xAF);
    assert(ret == DISPLAY_OK);

    return DISPLAY_OK;
}

display_err_t display_write_command(display_handle_t* display, uint8_t command) {
    int ret;

    // Enable driver command mode
    gpio_set_level(display->gpio_num_cd, 0);

    // Create empty transaction
    spi_transaction_t t;
    memset(&t, 0, sizeof(spi_transaction_t));

    // Initialize transaction
    t.flags = SPI_TRANS_USE_TXDATA;
    t.length = 8;
    (*t.tx_data) = command;

    // Send transaction
    ret = spi_device_polling_transmit(display->spi_device, &t);

    if (ret != ESP_OK) {
        return DISPLAY_ERR_IO;
    }

    return DISPLAY_OK;
}

display_err_t display_write_data(display_handle_t* display, uint8_t* data, int length) {
    int ret;

    // Enable driver data mode
    gpio_set_level(display->gpio_num_cd, 1);

    // Create empty transaction
    spi_transaction_t t;
    memset(&t, 0, sizeof(spi_transaction_t));

    // Initialize transaction
    t.length = length * 8;
    t.tx_buffer = data;

    // Send transaction
    ret = spi_device_polling_transmit(display->spi_device, &t);

    if (ret != ESP_OK) {
        return DISPLAY_ERR_IO;
    }

    return DISPLAY_OK;
}

display_err_t display_clear(display_handle_t* display) {
    int ret;

    // Enable driver data mode
    gpio_set_level(display->gpio_num_cd, 1);

    // Create empty transaction
    spi_transaction_t t;
    memset(&t, 0, sizeof(spi_transaction_t));

    // Initialize transaction
    t.flags = SPI_TRANS_USE_TXDATA;
    t.length = 32; // SPI transaction `tx_data` field is limited to 32 bits.

    // Send transaction
    // Transaction has to be sent mutiple times in order to fill whole display memory.
    for (int i = 0; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT / 32); i++) {
        ret = spi_device_polling_transmit(display->spi_device, &t);

        if (ret != ESP_OK) {
            return DISPLAY_ERR_IO;
        }
    }

    return DISPLAY_OK;
}

