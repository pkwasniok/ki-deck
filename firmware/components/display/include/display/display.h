#pragma once

#include <stdint.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define DISPLAY_OK       0
#define DISPLAY_ERR_IO   1

#define DISPLAY_FLAG_FLIP_HORIZONTAL 0b00000001
#define DISPLAY_FLAG_FLIP_VERTICAL   0b00000010

typedef int display_err_t;

typedef struct {
    spi_host_device_t spi_host;
    gpio_num_t gpio_num_res;
    gpio_num_t gpio_num_cd;
    gpio_num_t gpio_num_cs;
    int flags;
} display_config_t;

typedef struct {
    spi_device_handle_t spi_device;
    gpio_num_t gpio_num_res;
    gpio_num_t gpio_num_cd;
} display_handle_t;

display_err_t display_init(display_config_t* config, display_handle_t* display);

display_err_t display_write_command(display_handle_t* display, uint8_t command);
display_err_t display_write_data(display_handle_t* display, uint8_t* data, int length);

display_err_t display_clear(display_handle_t* display);

