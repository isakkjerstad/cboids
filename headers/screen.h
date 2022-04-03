#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL2/SDL.h>

/* Fill the entire screen with the given color. */
void fill_screen(SDL_Surface *screen, unsigned int color);

/* Set specified pixel (px, py) on surface to the given color. Returns zero on success. */
int fill_pixel(SDL_Surface *screen, int px, int py, unsigned int color);

#endif