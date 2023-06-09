#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "screen.h"
#include "boids.h"

/* Applies boid physics to any given bird based on the flock. */
static void bird_physics(boid_t *bird, int nb, boid_t **birdArr, int nh, boid_t **hoikArr);

/* Applies hoik hunting physics, and alters the birds. */
static void hunting_hoiks_physics(boid_t *hoik, int nb, boid_t **birdArr);

/* Applies screen margin steering to any type of boid. */
static void screen_border_avoidance(boid_t *boid);

/* Applies a velocity limit to any type of boid. */
static void velocity_limit(boid_t *boid);

/* Checks if point (x0, y0) is within range of point (x1, y1). */
static bool within_range(unsigned int range, int x0, int y0, int x1, int y1);

/* Return a positive/negative velocity within the given range. */
static int rand_velocity(unsigned int minVel, unsigned int maxVel);

unsigned int killed_birds = 0;

boid_t *create_boid(int type, SDL_Surface *screen) {

    // Preserved, i.e. stored in the data segment.
    static int uid = 1;

    SDL_PixelFormat *pxF = screen->format;

    boid_t *newBoid = calloc(1, sizeof(boid_t));
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
    
    default:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 0, 0);
        newBoid->size = 0;
        break;
    }

    // Set random position on screen.
    newBoid->xPos = rand() % screen->w;
    newBoid->yPos = rand() % screen->h;

    // Set random velocity.
    newBoid->xVel = rand_velocity(MIN_VEL, MAX_VEL);
    newBoid->yVel = rand_velocity(MIN_VEL, MAX_VEL);

    return newBoid;
}

void draw_boids(int n, boid_t **boidArr) {

    // Iterate all avail. boids.
    for (int idx = 0; idx < n; idx++) {

        // Get current boid.
        boid_t *boid = boidArr[idx];

        // Do not attempt to draw invisible boids.
        if (boid->color == 0) continue;

        // Draw vision range.
        if (DRAW_VRANGE == true) {

            unsigned int circleColor = SDL_MapRGBA(boid->screen->format, 64, 64, 64, 255);

            // Draw a circle around the boid to represent the boid's vision range.
            if (boid->type == HOIK) {
                draw_circle(boid->screen, boid->xPos, boid->yPos, boid->size + (RANGE * RESIZE_FACTOR), circleColor);
            } else {
                draw_circle(boid->screen, boid->xPos, boid->yPos, boid->size + RANGE, circleColor);
            }
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
        draw_point(boid->screen, round(boid->xPos), round(boid->yPos), boid->size, boid->color);
    }
}

void move_boids(int n, boid_t **boidArr) {

    // Iterate all avail. boids.
    for (int idx = 0; idx < n; idx++) {

        // Get current boid.
        boid_t *boid = boidArr[idx];

        // Do not attempt to move invisible boids.
        if (boid->color == 0) continue;

        // Update boid position.
        boid->xPos += boid->xVel;
        boid->yPos += boid->yVel;
    }
}

void simulate_boids(int nb, boid_t **birdArr, int nh, boid_t **hoikArr) {
    
    // Simulate all of the birds as a flock.
    for (int idx = 0; idx < nb; idx++) {
        if (birdArr[idx]->color == 0) continue;
        bird_physics(birdArr[idx], nb, birdArr, nh, hoikArr);
    }

    // Simulate all hoiks in relation to the birds.
    for (int idx = 0; idx < nh; idx++) {
        if (hoikArr[idx]->color == 0) continue;
        hunting_hoiks_physics(hoikArr[idx], nb, birdArr);
    }
}

static void bird_physics(boid_t *bird, int nb, boid_t **birdArr, int nh, boid_t **hoikArr) {

    boid_t *otherBird = NULL;
    float sumX = 0; float sumY = 0;
    float avgVelX = 0; float avgVelY = 0;
    float avgPosX = 0; float avgPosY = 0;
    int birdsInRange = 0;

    // Iterate trough all provided birds.
    for (int idx = 0; idx < nb; idx++) {

        // Select one bird.
        otherBird = birdArr[idx];

        // Ignore all invisible birds.
        if (otherBird->color == 0) continue;

        // Only use birds if not the current.
        if (bird->uid != otherBird->uid) {

            // Perform collision calculations only on birds within the given anti collision range.
            if (within_range(ANTI_COLLISION, bird->xPos, bird->yPos, otherBird->xPos, otherBird->yPos)) {

                // Steer away from the sum of other birds.
                sumX += bird->xPos - otherBird->xPos;
                sumY += bird->yPos - otherBird->yPos;
            }

            // Perform other calculations only on birds within the given vision range.
            if (within_range(RANGE, bird->xPos, bird->yPos, otherBird->xPos, otherBird->yPos)) {
                
                // Unique bird in range found.
                avgVelX += otherBird->xVel;
                avgVelY += otherBird->yVel;
                avgPosX += otherBird->xPos;
                avgPosY += otherBird->yPos;
                birdsInRange++;
            }
        }
    }

    // Apply anti collison physics.
    bird->xVel += sumX * SEPARATION;
    bird->yVel += sumY * SEPARATION;

    // Apply alignment and cohesion.
    if (birdsInRange != 0) {

        // Calculate avg. velocity for found birds.
        avgVelX = avgVelX / birdsInRange;
        avgVelY = avgVelY / birdsInRange;

        // Calculate avg. position for found birds.
        avgPosX = avgPosX / birdsInRange;
        avgPosY = avgPosY / birdsInRange;

        // Set new velocity to match birds in range.
        bird->xVel += (avgVelX - bird->xVel) * VELMATCH;
        bird->yVel += (avgVelY - bird->yVel) * VELMATCH;

        boid_t *hoik = NULL;
        bool scatter = false;

        // Decide on scattering, based on nearby hoiks.
        for (int idx = 0; idx < nh; idx++) {
            hoik = hoikArr[idx];
            if (within_range(200, bird->xPos, bird->yPos, hoik->xPos, hoik->yPos)) {
                scatter = true;
            }
        }

        if (!scatter) {

            // Set new velocity in order to apply cohesion.
            bird->xVel += (avgPosX - bird->xPos) * COHESION;
            bird->yVel += (avgPosY - bird->yPos) * COHESION;

        } else {

            // Scatter to avoid hoiks.
            bird->xVel -= (avgPosX - bird->xPos) * COHESION;
            bird->yVel -= (avgPosY - bird->yPos) * COHESION;
        }
    }

    // Apply general boid rules.
    screen_border_avoidance(bird);
    velocity_limit(bird);
}

static void hunting_hoiks_physics(boid_t *hoik, int nb, boid_t **birdArr) {

    boid_t *bird = NULL;
    int birdsInRange = 0;
    float avgPosX = 0; float avgPosY = 0;

    // Iterate over all birds.
    for (int idx = 0; idx < nb; idx++) {

        // Select one bird.
        bird = birdArr[idx];

        // Ignore invisible birds.
        if (bird->color == 0) continue;

        // Get values needed to calculate the average position of the nearest birds.
        if (within_range(RANGE * RESIZE_FACTOR, hoik->xPos, hoik->yPos, bird->xPos, bird->yPos)) {
            avgPosX += bird->xPos;
            avgPosY += bird->yPos;
            birdsInRange++;
        }

        // Kill birds trapped within the hoik, by changing the color.
        if (within_range(hoik->size, hoik->xPos, hoik->yPos, bird->xPos, bird->yPos)) {

            // Re-scale hoiks as they consume birds.
            if (killed_birds % HOIK_EAT == 0) hoik->size++;

            bird->color = 0;
            killed_birds++;
        }
    }

    // Calc. and steer the hoik towards the pray.
    if (birdsInRange != 0) {
        avgPosX = avgPosX / birdsInRange;
        avgPosY = avgPosY / birdsInRange;
        hoik->xVel += (avgPosX - hoik->xPos) * (COHESION / RESIZE_FACTOR);
        hoik->yVel += (avgPosY - hoik->yPos) * (COHESION / RESIZE_FACTOR);
    }

    // Apply general boid rules.
    screen_border_avoidance(hoik);
    velocity_limit(hoik);
}

static void screen_border_avoidance(boid_t *boid) {

    // Apply left screen margin steering.
    if (boid->xPos < MARGIN) {
        boid->xVel += TURN_ACCL;
    }

    // Apply right screen margin steering.
    if (boid->xPos > boid->screen->w - MARGIN) {
        boid->xVel -= TURN_ACCL;
    }

    // Apply top screen margin steering.
    if (boid->yPos < MARGIN) {
        boid->yVel += TURN_ACCL;
    }

    // Apply bottom screen margin steering.
    if (boid->yPos > boid->screen->h - MARGIN) {
        boid->yVel -= TURN_ACCL;
    }
}

static void velocity_limit(boid_t *boid) {

    // Calculate the current velocity of the given boid.
    float vel = sqrtf((boid->xVel * boid->xVel) + (boid->yVel * boid->yVel));

    // Apply upper velocity limit.
    if (vel > MAX_VEL) {
        boid->xVel = (boid->xVel / vel) * MAX_VEL;
        boid->yVel = (boid->yVel / vel) * MIN_VEL;
    }

    // Apply lower velocity limit.
    if (vel < MIN_VEL) {
        boid->xVel = (boid->xVel / vel) * MIN_VEL;
        boid->yVel = (boid->yVel / vel) * MAX_VEL;
    }
}

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