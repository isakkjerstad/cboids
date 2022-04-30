#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "screen.h"
#include "boids.h"

boid_t *create_boid(int type, SDL_Surface *screen) {

    SDL_PixelFormat *pxF = screen->format;

    boid_t *newBoid = malloc(sizeof(boid_t));
    if (newBoid == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    // Set SDL2 screen.
    newBoid->screen = screen;

    // Set boid type.
    newBoid->type = type;

    // Set boid color.
    switch (newBoid->type) {
    case BIRD:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 255, 255);
        break;

    case HOIK:
        newBoid->color = SDL_MapRGBA(pxF, 255, 0, 0, 255);;
        break;

    case BAIT:
        newBoid->color = SDL_MapRGBA(pxF, 0, 255, 0, 255);;
        break;
    
    default:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 0, 0);;
        break;
    }

    // Set random position on screen.
    newBoid->xPos = rand() % screen->w;
    newBoid->yPos = rand() % screen->h;

    // Set random velocity.
    newBoid->xVel = rand() % 20;
    newBoid->yVel = rand() % 20;

    return newBoid;
}

void draw_boids(int n, boid_t **boidArr) {

    // Iterate all avail. boids.
    for (int idx = 0; idx < n; idx++) {

        // Get current boid.
        boid_t *boid = boidArr[idx];

        // Draw the boid on the screen, with a given size.
        draw_point(boid->screen, boid->xPos, boid->yPos, BOID_SIZE, boid->color);
    }
}

void move_boids(int n, boid_t **boidArr) {

    // Iterate all avail. boids.
    for (int idx = 0; idx < n; idx++) {

        // Get current boid.
        boid_t *boid = boidArr[idx];

        // Update boid position.
        boid->xPos += boid->xVel;
        boid->yPos += boid->yVel;

        // Wrap around screen in X direction.
        if (boid->xPos > boid->screen->w) boid->xPos = 0;
        if (boid->xPos < 0) boid->xPos = boid->screen->w;

        // Wrap around screen in Y direction.
        if (boid->yPos > boid->screen->h) boid->yPos = 0;
        if (boid->yPos < 0) boid->yPos = boid->screen->h;
    }
}
