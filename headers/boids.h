#ifndef BOIDS_H_
#define BOIDS_H_

#include <SDL2/SDL.h>

// Avail. boid types.
enum boidType {
    bird,
    hoik,
    bait,
};

// Boid structure.
typedef struct boid {
    int type;               // Boid type (enum "boidType").
    int xPos;               // Horizontal position.
    int yPos;               // Vertical position.
    int xVel;               // Horizontal velocity.
    int yVel;               // Vertical velocity.
    unsigned int color;     // SDL2 RGBA color value.
} boid_t;

/* Returns init. boid of given type. */
boid_t *create_boid(int type, SDL_PixelFormat *pxF);

#endif