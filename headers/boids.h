#ifndef BOIDS_H_
#define BOIDS_H_

#include <SDL2/SDL.h>

/* Sim. params. */
#define COHESION    0.007   // Cohesion factor.
#define SEPARATION  0.036   // Separation factor.
#define VELMATCH    0.022   // Velocity match factor.
#define MARGIN      120     // Screen margin in pixels.
#define TURN_ACCL   0.4     // Apply turn acceleration outside margin.
#define HOIK_EAT    100     // Eating/re-scaling factor for hoiks.

enum boidConfig {
    BOID_SIZE = 4,          // Size of birds. Hoiks are relative to this size.
    RESIZE_FACTOR = 3,      // Size diff. of hoiks relative to boids (incl. behaviour).
    MIN_VEL = 2,            // Min. velocity for boids/hoiks.
    MAX_VEL = 7,            // Max. velocity for boids/hoiks.
    RANGE = 80,             // Vision range for boids/hoiks in pixels.
    ANTI_COLLISION = 14,    // Anti collision range.
};

// Avail. boid types.
enum boidType {
    BIRD,
    HOIK,
};

// Boid structure, invisible boids are dead and not handled.
typedef struct boid {
    int uid;                // Unique ID.
    int type;               // Boid type (enum "boidType").
    int size;               // Radius of the given boid.
    float xPos;             // Horizontal position.
    float yPos;             // Vertical position.
    float xVel;             // Horizontal velocity.
    float yVel;             // Vertical velocity.
    unsigned int color;     // SDL2 RGBA color value (0 == dead).
    SDL_Surface *screen;    // SDL2 screen for boid placement.
} boid_t;

// Number of dead birds since the program started.
extern unsigned int killed_birds;

/* Returns init. boid of given type. */
boid_t *create_boid(int type, SDL_Surface *screen);

/* Draw 'n' boids, stored as pointers in an array. */
void draw_boids(int n, boid_t **boidArr);

/* Add boid behaviour to the system for birds and hoiks. */
void simulate_boids(int nb, boid_t **birdArr, int nh, boid_t **hoikArr);

/* Move 'n' boids, stored as pointers in an array. */
void move_boids(int n, boid_t **boidArr);

#endif