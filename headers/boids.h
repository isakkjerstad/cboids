#ifndef BOIDS_H_
#define BOIDS_H_

#include <SDL2/SDL.h>

enum boidConfig {
    BOID_SIZE = 10,
    MIN_VEL = 2,
    MAX_VEL = 8,
    RANGE = 120,
    ALIGN = 0,
    COHESION = 0,
    SEPARATION = 0,
    VELMATCH = 0,
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

/* Add boid behaviour to the system for boids, hoiks and obstacles. */
void simulate_boids(int nb, boid_t **boidArr, int nh, boid_t **hoikArr, int no, boid_t **obstArr);

/* Move 'n' boids, stored as pointers in an array. */
void move_boids(int n, boid_t **boidArr);

/* Return a positive/negative velocity within the given range. */
static int rand_velocity(unsigned int minVel, unsigned int maxVel);

#endif