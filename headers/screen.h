#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL2/SDL.h>

/* Fill the entire screen with the given color. */
void fill_screen(SDL_Surface *screen, unsigned int color);

/* Set specified pixel (px, py) on surface to the given color. Returns zero on success. */
int fill_pixel(SDL_Surface *screen, int px, int py, unsigned int color);

/* Fill point at center (cx, cy) with given radius and color. */
void draw_point(SDL_Surface *screen, int cx, int cy, int size, unsigned int color);

/* Draws a circle at center (cx, cy) with given radius and color. */
void draw_circle(SDL_Surface *screen, int cx, int cy, int radius, unsigned int color);

/* Draws a line from (x0, y0) to (x1, y1) with the given color, using Bresenham's algorithm. */
void draw_line(SDL_Surface *screen, int x0, int y0, int x1, int y1, unsigned int color);

/* Partial functions req. for the Bresenham's line drawing algorithm. */
static void plotLineHigh(SDL_Surface *surface, int x0, int y0, int x1, int y1, unsigned int color);
static void plotLineLow(SDL_Surface *surface, int x0, int y0, int x1, int y1, unsigned int color);

#endif