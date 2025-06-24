#include "graphics/graphics.h"
#include "graphics/geometry.h"

#include <stdlib.h>

void line_horizontal(int, int, int, int);
void line_vertical(int, int, int, int);
void line_bresenham(int x0, int y0, int x1, int y1);
void line_bresenham_high(int x0, int y0, int x1, int y1);
void line_bresenham_low(int x0, int y0, int x1, int y1);

void g_line(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dx == 0 && dy == 0) {
        g_matrix[x0][y0] = 255;
    } else if (dy == 0) {
        line_horizontal(x0, y0, x1, y1);
    } else if (dx == 0) {
        line_vertical(x0, y0, x1, y1);
    } else {
        line_bresenham(x0, y0, x1, y1);
    }
}

void line_bresenham(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    if (abs(dx) >= abs(dy)) {
        // Octect 1, 8, 4, 5
        if (dx > 0) {
            line_bresenham_high(x0, y0, x1, y1);
        } else {
            line_bresenham_high(x1, y1, x0, y0);
        }
    } else {
        // Octet 2, 3, 6, 7
        if (dy > 0) {
            line_bresenham_low(x0, y0, x1, y1);
        } else {
            line_bresenham_low(x1, y1, x0, y0);
        }
    }
}

void line_horizontal(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;

    if (dx >= 0) {
        for (int x = x0; x <= x1; x++) {
            g_matrix[x][y0] = 255;
        }
    } else {
        for (int x = x1; x <= x0; x++) {
            g_matrix[x][y0] = 255;
        }
    }
}

void line_vertical(int x0, int y0, int x1, int y1) {
    int dy = y1 - y0;

    if (dy >= 0) {
        for (int y = y0; y <= y1; y++) {
            g_matrix[x0][y] = 255;
        }
    } else {
        for (int y = y1; y <= y0; y++) {
            g_matrix[x0][y] = 255;
        }
    }
}

void line_bresenham_high(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int yi = 1;
    if (dy < 0) {
        dy = -dy;
        yi = -1;
    }

    int x = x0;
    int y = y0;
    int D = (2 * dy) - dx;
    for (; x <= x1; x++) {
        g_matrix[x][y] = 255;

        if (D > 0) {
            y += yi;
            D += (2 * dy) - (2 * dx);
        } else {
            D += (2 * dy);
        }
    }
}

void line_bresenham_low(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int xi = 1;
    if (dx < 0) {
        dx = -dx;
        xi = -1;
    }

    int x = x0;
    int y = y0;
    int D = (2 * dx) - dy;
    for (; y <= y1; y++) {
        g_matrix[x][y] = 255;

        if (D > 0) {
            x += xi;
            D += (2 * dx) - (2 * dy);
        } else {
            D += (2 * dx);
        }
    }
}

