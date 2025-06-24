#include "graphics/geometry.h"

void g_rect(int x0, int y0, int x1, int y1) {
    g_line(x0, y0, x1, y0);
    g_line(x0, y1, x1, y1);
    g_line(x0, y0, x0, y1);
    g_line(x1, y0, x1, y1);
}

