#ifndef BOIDS_H_
#define BOIDS_H_

#include <SDL2/SDL.h>

enum boidConfig {
    BOID_SIZE = 3,
};

// Avail. boid types.
enum boidType {
    BIRD,
    HOIK,
    BAIT,
};

// Boid structure.
typedef struct boid {
    int type;               // Boid type (enum "boidType").
    int xPos;               // Horizontal position.
    int yPos;               // Vertical position.
    int xVel;               // Horizontal velocity.
    int yVel;               // Vertical velocity.
    unsigned int color;     // SDL2 RGBA color value.
    SDL_Surface *screen;    // SDL2 screen for boid placement.
} boid_t;

/* Returns init. boid of given type. */
boid_t *create_boid(int type, SDL_Surface *screen);

/* Draw 'n' boids, stored as pointers in an array. */
void draw_boids(int n, boid_t **boidArr);

/* Move 'n' boids, stored as pointers in an array. */
void move_boids(int n, boid_t **boidArr);

#endif