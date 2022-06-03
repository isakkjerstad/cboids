#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "screen.h"
#include "boids.h"

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
        newBoid->color = SDL_MapRGBA(pxF, 0, 255, 0, 255);
        break;

    case HOIK:
        newBoid->color = SDL_MapRGBA(pxF, 255, 0, 0, 255);
        break;

    case BAIT:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 255, 255);
        break;
    
    default:
        newBoid->color = SDL_MapRGBA(pxF, 0, 0, 0, 0);
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

        // Draw vision range.
        if (DRAW_VRANGE == true) {

            unsigned int circleColor = SDL_MapRGBA(boid->screen->format, 64, 64, 64, 255);

            // Draw a circle around the boid to represent the boid's vision range.
            draw_circle(boid->screen, boid->xPos, boid->yPos, BOID_SIZE + RANGE, circleColor);
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
        if (boid->xPos > boid->screen->w) boid->xPos = BOID_SIZE;
        if (boid->xPos < -BOID_SIZE) boid->xPos = boid->screen->w;

        // Wrap around screen in Y direction.
        if (boid->yPos > boid->screen->h) boid->yPos = BOID_SIZE;
        if (boid->yPos < -BOID_SIZE) boid->yPos = boid->screen->h;
    }
}

void simulate_boids(int nb, boid_t **boidArr, int nh, boid_t **hoikArr, int no, boid_t **obstArr) {

    // TODO: Implement Me!
}