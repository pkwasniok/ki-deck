#include "graphics/graphics.h"

#include <stdlib.h>

g_matrix_t g_matrix;

g_event_handler_t g_event_handler_update;

void g_init(void) {
    g_event_handler_update = NULL;
}

void g_register_event_handler(g_event_t event, g_event_handler_t handler) {
    switch (event) {
        case G_EVENT_UPDATE:
            g_event_handler_update = handler;
            break;
    }
}

void g_update(void) {
    if (g_event_handler_update != NULL) {
        g_event_handler_update();
    }
}

void g_clear(void) {
    for (int x = 0; x < G_MATRIX_WIDTH; x++) {
        for (int y = 0; y < G_MATRIX_HEIGHT; y++) {
            g_matrix[x][y] = 0;
        }
    }
}

