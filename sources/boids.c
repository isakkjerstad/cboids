#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "screen.h"
#include "boids.h"

boid_t *create_boid(int type, SDL_PixelFormat *pxF) {

    boid_t *newBoid = malloc(sizeof(boid_t));
    if (newBoid == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    // Set boid type.
    newBoid->type = type;

    // Set boid color.
    switch (newBoid->type) {
    case bird:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 255, 255);
        break;

    case hoik:
        newBoid->color = SDL_MapRGBA(pxF, 255, 0, 0, 255);;
        break;

    case bait:
        newBoid->color = SDL_MapRGBA(pxF, 0, 255, 0, 255);;
        break;
    
    default:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 0, 0);;
        break;
    }

    // Set random position on screen.
    newBoid->xPos = 0;
    newBoid->yPos = 0;

    // Set random velocity.
    newBoid->xVel = 0;
    newBoid->yVel = 0;

    return newBoid;
}