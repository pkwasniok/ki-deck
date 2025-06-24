#include "graphics/graphics.h"
#include "graphics/geometry.h"

#include <stdlib.h>

int radius_error(int x0, int y0, int r);

void g_circle(int x0, int y0, int r) {
    int x = r;
    int y = 0;

    while (x >= y) {
        g_matrix[x0 + x][y0 + y] = 255;
        g_matrix[x0 + x][y0 - y] = 255;
        g_matrix[x0 - x][y0 + y] = 255;
        g_matrix[x0 - x][y0 - y] = 255;
        g_matrix[x0 - y][y0 + x] = 255;
        g_matrix[x0 - y][y0 - x] = 255;
        g_matrix[x0 + y][y0 + x] = 255;
        g_matrix[x0 + y][y0 - x] = 255;

        if (radius_error(x-1, y+1, r) < radius_error(x, y+1, r)) {
            x -= 1;
        }

        y += 1;
    }
}

int radius_error(int x0, int y0, int r) {
    return abs((x0 * x0) + (y0 * y0) - (r * r));
}

