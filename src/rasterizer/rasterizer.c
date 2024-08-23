
#include "rasterizer.h"

color_t screen[LCD_HEIGHT][LCD_WIDTH] = {{0x00}};

void plot_horizontal_line(int x0, int x1, int y, color_t color) {
    for (int x = x0; x <= x1; x++) {
        screen[y][x] = color;
    }
}

void plot_vertical_line(int y0, int y1, int x, color_t color) {
    for (int y = y0; y <= y1; y++) {
        screen[y][x] = color;
    }
}

void plot_line_low(int x0, int y0, int x1, int y1, color_t color) {    
    int dx = x1 - x0; // 120
    int dy = y1 - y0; // 0
    int yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int d = 2*dy - dx; // -120
    int y = y0; // 180

    for (int x = x0; x <= x1; x++) {
        screen[y][x] = color;
        if (d > 0) {
            y += yi;
            d += 2 * (dy - dx);
        }
        else {
            d += 2*dy;
        }
    }
}

void plot_line_high(int x0, int y0, int x1, int y1, color_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int d = 2*dx - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++) {
        screen[y][x] = color;
        if (d > 0) {
            x += xi;
            d += 2 * (dx - dy);
        }
        else {
            d += 2*dx;
        }
    }
}

void plot_line(int x0, int y0, int x1, int y1, color_t color) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            plot_line_low(x1, y1, x0, y0, color);
        }
        else {
            plot_line_low(x0, y0, x1, y1, color);
        }
    }
    else {
        if (y0 > y1) {
            plot_line_high(x1, y1, x0, y0, color);
        }
        else {
            plot_line_high(x0, y0, x1, y1, color);
        }
    }
}

void plot_wire_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color) {
    plot_line(x0, y0, x1, y1, color);
    plot_line(x0, y0, x2, y2, color);
    plot_line(x1, y1, x2, y2, color);
}

void plot_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color) {
    if (y1 < y0) {
        const int tempx = x1;
        const int tempy = y1;
        x1 = x0;
        y1 = y0;
        x0 = tempx;
        y0 = tempy;
    }
    if (y2 < y1) {
        const int tempx = x2;
        const int tempy = y2;
        x2 = x1;
        y2 = y1;
        x1 = tempx;
        y1 = tempy;
    }
    if (y1 < y0) {
        const int tempx = x1;
        const int tempy = y1;
        x1 = x0;
        y1 = y0;
        x0 = tempx;
        y0 = tempy;
    }

    // Check whether the neighbor triangles have gap between them. If there is a gap, uncomment.
    // plot_wire_triangle(x0, y0, x1, y1, x2, y2, color);

    for (int y = y0; y <= y2; y++) {
        int x_left;
        int x_right;

        if (y >= y1) {
            const int dxl = (y2 != y1) ? (y - y1) * (x2 - x1) / (y2 - y1) : 0;
            const int dxr = (y2 != y0) ? (y - y0) * (x2 - x0) / (y2 - y0) : 0;
            x_left  = x1 + dxl;
            x_right = x0 + dxr;
        } 
        else {
            const int dxl = (y1 != y0) ? (y - y0) * (x1 - x0) / (y1 - y0) : 0;
            const int dxr = (y2 != y0) ? (y - y0) * (x2 - x0) / (y2 - y0) : 0;
            x_left  = x0 + dxl;
            x_right = x0 + dxr;
        }

        if (x_left > x_right) {
            const int temp = x_left;
            x_left = x_right;
            x_right = temp;
        }

        for (int x = x_left; x <= x_right; x++) {
            screen[y][x] = color;
        }
    }
}
