#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "screen.h"
#include "boids.h"

/* Return a positive/negative velocity within the given range. */
static int rand_velocity(unsigned int minVel, unsigned int maxVel) {

    // Calculate a random velocity with a minimum and maximum value.
    int velocity = minVel + (rand() % ((maxVel + 1) - minVel));

    // Set velocity sign.
    if (rand() % 2) {
        return -velocity;
    } else {
        return velocity;
    }
}

boid_t *create_boid(int type, SDL_Surface *screen) {

    // Preserved, i.e. stored in the data segment.
    static int uid = 1;

    SDL_PixelFormat *pxF = screen->format;

    boid_t *newBoid = malloc(sizeof(boid_t));
    if (newBoid == NULL) {
        fprintf(stderr, "Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    // Set UID, and increment.
    newBoid->uid = uid++;

    // Set SDL2 screen.
    newBoid->screen = screen;

    // Set boid type.
    newBoid->type = type;

    // Set boid color.
    switch (newBoid->type) {
    case BIRD:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 255, 255);
        newBoid->size = BOID_SIZE;
        break;

    case HOIK:
        newBoid->color = SDL_MapRGBA(pxF, 255, 0, 0, 255);
        newBoid->size = BOID_SIZE * RESIZE_FACTOR;
        break;

    case BAIT:
        newBoid->color = SDL_MapRGBA(pxF, 255, 255, 0, 255);
        newBoid->size = BOID_SIZE / RESIZE_FACTOR;
        break;

    case OBSTACLE:
        newBoid->color = SDL_MapRGBA(pxF, 255, 255, 255, 255);
        newBoid->size = OBSTACLE_SIZE;
        break;
    
    default:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 0, 0);
        newBoid->size = 0;
        break;
    }

    // Set random position on screen.
    newBoid->xPos = rand() % screen->w;
    newBoid->yPos = rand() % screen->h;

    // Set random velocity.
    if ((newBoid->type != BAIT) && (newBoid->type != OBSTACLE)) {
        
        // Only boids and hoiks move on the screen.
        newBoid->xVel = rand_velocity(MIN_VEL, MAX_VEL);
        newBoid->yVel = rand_velocity(MIN_VEL, MAX_VEL);

    } else {

        // Bait and obstacles is stationary.
        newBoid->xVel = 0;
        newBoid->yVel = 0;
    }
    
    return newBoid;
}

void draw_boids(int n, boid_t **boidArr) {

    // Iterate all avail. boids.
    for (int idx = 0; idx < n; idx++) {

        // Get current boid.
        boid_t *boid = boidArr[idx];

        // Draw vision range.
        if (DRAW_VRANGE == true) {

            unsigned int circleColor = SDL_MapRGBA(boid->screen->format, 64, 64, 64, 255);

            // Draw a circle around the boid to represent the boid's vision range.
            draw_circle(boid->screen, boid->xPos, boid->yPos, boid->size + RANGE, circleColor);
        }

        // Draw velocity arrow.
        if (DRAW_VEL == true) {

            unsigned int velLineColor = SDL_MapRGBA(boid->screen->format, 128, 128, 128, 255);

            // End position of velocity vector, i.e. pos. after one second.
            int velX = boid->xPos + (boid->xVel * FPS);
            int velY = boid->yPos + (boid->yVel * FPS);

            // Draw line from the center of the boid to represent the velocity.
            draw_line(boid->screen, boid->xPos, boid->yPos, velX, velY, velLineColor);
        }

        // Draw the boid on the screen, with a given size.
        draw_point(boid->screen, boid->xPos, boid->yPos, boid->size, boid->color);
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
        if (boid->xPos > boid->screen->w) boid->xPos = boid->size;
        if (boid->xPos < -boid->size) boid->xPos = boid->screen->w;

        // Wrap around screen in Y direction.
        if (boid->yPos > boid->screen->h) boid->yPos = boid->size;
        if (boid->yPos < -boid->size) boid->yPos = boid->screen->h;
    }
}

/* Checks if point (x0, y0) is within range of point (x1, y1). */
static bool within_range(unsigned int range, int x0, int y0, int x1, int y1) {

    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    // Calculate distance between points in 2D space using the Pythagorean theorem.
    unsigned int distance = (unsigned int)(sqrt((deltaX * deltaX) + (deltaY * deltaY)));

    // Check if points is within range.
    if (distance <= range) {
        return true;
    } else {
        return false;
    }
}

/* ... */
static void separation(boid_t *boid, int n, boid_t **arr) {

    boid_t *otherBoid = NULL;
    float sumX = 0; float sumY = 0;

    // Iterate trough all provided boids.
    for (int idx = 0; idx < n; idx++) {

        // Select one boid.
        otherBoid = arr[idx];

        // Only use boid if not the current.
        if (boid->uid != otherBoid->uid) {

            // Perform calculations only on boids within the given vision range.
            if (within_range(ANTI_COLLISION, boid->xPos, boid->yPos, otherBoid->xPos, otherBoid->yPos)) {

                // Steer away from the sum of other boids.
                sumX += boid->xPos - otherBoid->xPos;
                sumY += boid->yPos - otherBoid->yPos;
            }
        }
    }

    // Alter velocity of the given boid.
    boid->xVel += sumX * SEPARATION;
    boid->yVel += sumY * SEPARATION;
}

/* ... */
static void alignment(boid_t *boid, int n, boid_t **arr) {

    boid_t *otherBoid = NULL;

    // Values used to calculate avg. velocity.
    float avgVelX = 0; float avgVelY = 0;
    int boidsInRange = 0;

    // Iterate trough all provided boids.
    for (int idx = 0; idx < n; idx++) {

        otherBoid = arr[idx];

        // Only use boid if not the current.
        if (boid->uid != otherBoid->uid) {

            // Perform calculations only on boids within the given vision range.
            if (within_range(RANGE, boid->xPos, boid->yPos, otherBoid->xPos, otherBoid->yPos)) {
                
                // Unique boid in range found.
                avgVelX += otherBoid->xVel;
                avgVelY += otherBoid->yVel;
                boidsInRange++;
            }
        }
    }

    if (boidsInRange != 0) {

        // Calculate avg. velocity for found boids.
        avgVelX = avgVelX / boidsInRange;
        avgVelY = avgVelY / boidsInRange;

        // Set new velocity to match boids in range.
        boid->xVel += (avgVelX - boid->xVel) * VELMATCH;
        boid->yVel += (avgVelY - boid->yVel) * VELMATCH;
    }
}

/* ... */
static void cohesion(boid_t *boid, int n, boid_t **arr) {

    boid_t *otherBoid = NULL;

    float avgPosX = 0; float avgPosY = 0;
    int boidsInRange = 0;

    // Iterate trough all provided boids.
    for (int idx = 0; idx < n; idx++) {

        otherBoid = arr[idx];

        // Only use boid if not the current.
        if (boid->uid != otherBoid->uid) {

            // Perform calculations only on boids within the given vision range.
            if (within_range(RANGE, boid->xPos, boid->yPos, otherBoid->xPos, otherBoid->yPos)) {
                
                // Unique boid in range found.
                avgPosX += otherBoid->xPos;
                avgPosY += otherBoid->yPos;
                boidsInRange++;
            }
        }
    }

    if (boidsInRange != 0) {

        // Calculate avg. position for found boids.
        avgPosX = avgPosX / boidsInRange;
        avgPosY = avgPosY / boidsInRange;

        // Set new velocity in order to apply cohesion.
        boid->xVel += (avgPosX - boid->xPos) * COHESION;
        boid->yVel += (avgPosY - boid->yPos) * COHESION;
    }
}

/* ... */
static void velocity_limit(boid_t *boid) {

    float vel = sqrtf((boid->xVel * boid->xVel) + (boid->yVel * boid->yVel));

    if (vel > MAX_VEL) {
        boid->xVel = (boid->xVel / vel) * MAX_VEL;
        boid->yVel = (boid->yVel / vel) * MIN_VEL;
    }

    if (vel < MIN_VEL) {
        boid->xVel = (boid->xVel / vel) * MIN_VEL;
        boid->yVel = (boid->yVel / vel) * MAX_VEL;
    }
}

void simulate_boids(int nb, boid_t **boidArr, int nh, boid_t **hoikArr, int nba, boid_t **baitArr, int no, boid_t **obstArr) {

    for (int idx = 0; idx < nb; idx++) {
        separation(boidArr[idx], nb, boidArr);
        alignment(boidArr[idx], nb, boidArr);
        cohesion(boidArr[idx], nb, boidArr);
        velocity_limit(boidArr[idx]);
    }
}