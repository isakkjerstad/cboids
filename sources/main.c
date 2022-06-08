#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "config.h"
#include "screen.h"
#include "boids.h"

/* Init. a SDL2 window and screen, return zero on success. */
int init_rendering(SDL_Window **window, SDL_Surface **screen);

/* Destroy the window, and exit SDL2. */
void destroy_rendering(SDL_Window **window);

int main(int argc, char *argv[]) {

    unsigned int nBirds = 2500;
    unsigned int nHoiks = 0;        // NO-OP!
    unsigned int nBaits = 0;        // NO-OP!
    unsigned int nObstacles = 0;    // NO-OP!

    // Set up SDL2 for displaying and altering content.
    SDL_Window *window; SDL_Surface *screen;
    if (init_rendering(&window, &screen) == EXIT_FAILURE) { return EXIT_FAILURE; }

    SDL_Event queue;
    SDL_PixelFormat *pxF = screen->format;

    // Framerate limiter variables.
    const uint32_t renderDelay = 1000 / FPS;
    uint32_t renderStartTime, renderTime;

    // Create the bird boids as an array of boid structures.
    boid_t *boidArr[nBirds];
    for (unsigned int idx = 0; idx < nBirds; idx++) {
        boidArr[idx] = create_boid(BIRD, screen);
    }

    // Create the hoiks.
    boid_t *hoikArr[nHoiks];
    for (unsigned int idx = 0; idx < nHoiks; idx++) {
        hoikArr[idx] = create_boid(HOIK, screen);
    }

    // Create the bait.
    boid_t *baitArr[nBaits];
    for (unsigned int idx = 0; idx < nBaits; idx++) {
        baitArr[idx] = create_boid(BAIT, screen);
    }

    // Create the obstacles.
    boid_t *obstacleArr[nObstacles];
    for (unsigned int idx = 0; idx < nObstacles; idx++) {
        obstacleArr[idx] = create_boid(OBSTACLE, screen);
    }

    bool running = true;
    while (running) {

        // Start the render timer.
        renderStartTime = SDL_GetTicks();

        // Seed random.
        srand(time(NULL));

        // Clear the screen, with the given RGBA color.
        fill_screen(screen, SDL_MapRGBA(pxF, 0, 0, 0, 0));

        // Simulate boid behaviour with the entire system.
        simulate_boids(nBirds, boidArr, nHoiks, hoikArr, nBaits, baitArr, nObstacles, obstacleArr);

        // Animate the bird boids.
        move_boids(nBirds, boidArr);
        draw_boids(nBirds, boidArr);

        // Animate the hoiks.
        move_boids(nHoiks, hoikArr);
        draw_boids(nHoiks, hoikArr);

        // Animate the bait.
        move_boids(nBaits, baitArr);
        draw_boids(nBaits, baitArr);

        // Animate the obstacles.
        move_boids(nObstacles, obstacleArr);
        draw_boids(nObstacles, obstacleArr);

        // Swap the screen and window buffer.
        SDL_UpdateWindowSurface(window);

        // Handle the SDL2 event queue.
        while (SDL_PollEvent(&queue)) {
            switch (queue.type) {

            case SDL_KEYDOWN:
                if (queue.key.keysym.scancode == SDL_SCANCODE_ESCAPE)       { running = false; }
                if (queue.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)    { running = false; }
                break;

            case SDL_QUIT:
                running = false;
                break;
            
            default:
                break;
            }
        }
        
        // Stop the render timer, get result.
        renderTime = SDL_GetTicks() - renderStartTime;

        // Limit framerate to the set FPS limit.
        if (renderTime < renderDelay) {
            SDL_Delay(renderDelay - renderTime);
        }
    }

    // Free up resources and quit.
    destroy_rendering(&window);
    return EXIT_SUCCESS;
}

int init_rendering(SDL_Window **window, SDL_Surface **screen) {

    // Init. the SDL2 video subsystem.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create a new window in the middle of the screen, with specified dimensions.
    *window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDHT, SCREEN_HEIGHT, false);
    if (window == NULL) {
        fprintf(stderr, "Failed to create a window: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Surface in memory to be updated and later swapped with the window.
    *screen = SDL_GetWindowSurface(*window);
    if (screen == NULL) {
        fprintf(stderr, "Failed to create a screen: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    return 0;
}

void destroy_rendering(SDL_Window **window) {

    SDL_DestroyWindow(*window);
    SDL_Quit();
}