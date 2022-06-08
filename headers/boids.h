#ifndef BOIDS_H_
#define BOIDS_H_

#include <SDL2/SDL.h>

/* Sim. params. */
#define COHESION    0.004   // Cohesion factor.
#define SEPARATION  0.032   // Separation factor.
#define VELMATCH    0.027   // Velocity match factor.

enum boidConfig {
    BOID_SIZE = 4,          // Size of boids. Other items are relative to this size.
    RESIZE_FACTOR = 2,      // Size diff. of hoiks and bait relative to boids.
    OBSTACLE_SIZE = 12,     // Size of obstacles.
    MIN_VEL = 2,            // Min. velocity for boids/hoiks.
    MAX_VEL = 7,            // Max. velocity for boids/hoiks.
    RANGE = 80,             // Vision range for boids/hoiks in pixels.
    ANTI_COLLISION = 12,    // Anti collision range.
};

// Avail. boid types.
enum boidType {
    BIRD,
    HOIK,
    BAIT,
    OBSTACLE,
};

// Boid structure.
typedef struct boid {
    int uid;                // Unique ID.
    int type;               // Boid type (enum "boidType").
    int size;               // Radius of the given boid.
    int xPos;               // Horizontal position.
    int yPos;               // Vertical position.
    float xVel;             // Horizontal velocity.
    float yVel;             // Vertical velocity.
    unsigned int color;     // SDL2 RGBA color value.
    SDL_Surface *screen;    // SDL2 screen for boid placement.
} boid_t;

/* Returns init. boid of given type. */
boid_t *create_boid(int type, SDL_Surface *screen);

/* Draw 'n' boids, stored as pointers in an array. */
void draw_boids(int n, boid_t **boidArr);

/* Add boid behaviour to the system for boids, hoiks bait and obstacles. */
void simulate_boids(int nb, boid_t **boidArr, int nh, boid_t **hoikArr, int nba, boid_t **baitArr, int no, boid_t **obstArr);

/* Move 'n' boids, stored as pointers in an array. */
void move_boids(int n, boid_t **boidArr);

#endif