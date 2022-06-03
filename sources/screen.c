#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "screen.h"

void fill_screen(SDL_Surface *screen, unsigned int color) {

    SDL_LockSurface(screen);

    // Fill the screen rectangle with the given color.
    if (SDL_FillRect(screen, NULL, color) != 0) {
        if (DEBUG) { fprintf(stderr, "Failed to fill the screen: %s\n", SDL_GetError()); }
    }

    SDL_UnlockSurface(screen);
}

int fill_pixel(SDL_Surface *screen, int px, int py, unsigned int color) {

    // Calculate the index to modify in the pixel array.
    unsigned int idx = px + (py * screen->w);

    // Check if the requested pixel is within a valid area.
    if (px >= screen->w || py >= screen->h || px < 0 || py < 0) {

        if (DEBUG) {
            fprintf(stderr, "Failed to set pixel (%d, %d) outside of the surface!\n", px, py);
        }

        return -1;
    }

    // Get an array of pixels pointing to color values.
    unsigned int *pixels = (unsigned int *)(screen->pixels);

    SDL_LockSurface(screen);

    // Set pixel value.
    pixels[idx] = color;

    SDL_UnlockSurface(screen);

    return 0;
}

void draw_point(SDL_Surface *screen, int cx, int cy, int size, unsigned int color) {

    // Adjust circle curve.
    const int adjust = 0.8;

    // Iterate over all possible pixels in a square.
    for (int py = -size; py <= size; py++) {
        for (int px = -size; px <= size; px++) {
            
            // Only draw pixels inside a circle, with the given color.
            if (px * px + py * py <= size * size + (size * adjust)) {
                fill_pixel(screen, (px + cx), (py + cy), color);
            }
        }
    }
}

void draw_circle(SDL_Surface *screen, int cx, int cy, int radius, unsigned int color) {

    // Thickness of line to draw.
    int thickness = radius - 1;

    // Iterate over all possible pixels in a square.
    for (int py = -radius; py <= radius; py++) {
        for (int px = -radius; px <= radius; px++) {
            
            // Only draw pixels on the edge of the circle, with the given thickness.
            if ((px * px + py * py <= radius * radius) && (px * px + py * py >= thickness * thickness)) {
                fill_pixel(screen, (px + cx), (py + cy), color);
            }
        }
    }
}

void draw_line(SDL_Surface *screen, int x0, int y0, int x1, int y1, unsigned int color) {

    // Draw using Bresenham's line algorithm.
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            plotLineLow(screen, x1, y1, x0, y0, color);
        } else {
            plotLineLow(screen, x0, y0, x1, y1, color);
        }
    } else {
        if (y0 > y1) {
            plotLineHigh(screen, x1, y1, x0, y0, color);
        } else {
            plotLineHigh(screen, x0, y0, x1, y1, color);
        }
    }
}

// Partial function req. for Bresenham's algorithm.
static void plotLineHigh(SDL_Surface *surface, int x0, int y0, int x1, int y1, unsigned int color) {

    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);
    int xi = 1;

    if (delta_x < 0) {
        xi = -1;
        delta_x = -(delta_x);
    }

    int D = ((2 * delta_x) - delta_y);
    int x = x0;

    for (int i = y0; i <= y1; i++) {

        fill_pixel(surface, x, i, color);

        if (D > 0) {
            x = x + xi;
            D = D - (2 * delta_y);
        }

        D = D + (2 * delta_x);
    }
}

// Partial function req. for Bresenham's algorithm.
static void plotLineLow(SDL_Surface *surface, int x0, int y0, int x1, int y1, unsigned int color) {

    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);
    int yi = 1;

    if (delta_y < 0) {
        yi = -1;
        delta_y = -(delta_y);
    }

    int D = ((2 * delta_y) - delta_x);
    int y = y0;

    for (int i = x0; i <= x1; i++) {

        fill_pixel(surface, i, y, color);

        if (D > 0) {
            y = y + yi;
            D = D - (2 * delta_x);
        }

        D = D + (2 * delta_y);
    }
}