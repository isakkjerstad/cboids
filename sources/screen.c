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

        return 1;
    }

    // Get an array of pixels pointing to color values.
    unsigned int *pixels = (unsigned int *)(screen->pixels);

    SDL_LockSurface(screen);

    // Set pixel value.
    pixels[idx] = color;

    SDL_UnlockSurface(screen);

    return 0;
}

int draw_point(SDL_Surface *screen, int cx, int cy, int size, unsigned int color) {

    // Adjust circle curve.
    const int adjust = 0.8;

    // Iterate over all possible pixels in a square.
    for (int py = -size; py <= size; py++) {
        for (int px = -size; px <= size; px++) {
            
            // Only draw pixels inside a circle, with the given color.
            if (px * px + py * py <= size * size + (size * adjust)) {
                if (fill_pixel(screen, (px + cx), (py + cy), color) != 0) {
                    return 1;
                }
            }
        }
    }

    return 0;
}