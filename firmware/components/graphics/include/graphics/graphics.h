#pragma once

#include <stdint.h>

#define G_MATRIX_WIDTH 128
#define G_MATRIX_HEIGHT 64

typedef uint8_t g_matrix_t[G_MATRIX_WIDTH][G_MATRIX_HEIGHT];

typedef enum {
    G_EVENT_UPDATE,
} g_event_t;

typedef void (*g_event_handler_t)(void);

extern g_matrix_t g_matrix;

void g_init(void);

void g_register_event_handler(g_event_t event, g_event_handler_t handler);

void g_update(void);

void g_clear(void);

