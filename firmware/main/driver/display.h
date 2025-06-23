#pragma once

#include "driver/gpio.h"
#include "driver/spi_master.h"

#define DISPLAY_OK       0
#define DISPLAY_ERR_INIT 1

typedef int display_err_t;

typedef struct {
    spi_host_device_t spi_host;
    gpio_num_t gpio_num_res;
    gpio_num_t gpio_num_cd;
    gpio_num_t gpio_num_cs;
} display_config_t;

typedef struct {
    spi_device_handle_t spi_device;
} display_handle_t;

display_err_t display_init(display_config_t* config, display_handle_t* display);

