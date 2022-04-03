#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"
#include "screen.h"

/* Init. a SDL2 window and screen, return zero on success. */
int init_rendering(SDL_Window **window, SDL_Surface **screen);

/* Destroy the window, and exit SDL2. */
void destroy_rendering(SDL_Window **window);

int main(int argc, char *argv[]) {

    // Set up SDL2 for displaying and altering content.
    SDL_Window *window; SDL_Surface *screen;
    if (init_rendering(&window, &screen) == EXIT_FAILURE) { return EXIT_FAILURE; }

    SDL_Event queue;
    SDL_PixelFormat *pxF = screen->format;

    bool running = true;
    while (running) {

        // TODO: Time the framerate here!

        // Clear the screen, with the given RGBA color.
        fill_screen(screen, SDL_MapRGBA(pxF, 0, 0, 0, 0));

        // TODO: Animate the boids here!

        // REMOVE ME: This line is only for testing.
        fill_pixel(screen, 50, 50, SDL_MapRGBA(pxF, 255, 0, 0, 255));

        // Swap the screen and window buffer.
        SDL_UpdateWindowSurface(window);

        // Handle the SDL2 event queue.
        while (SDL_PollEvent(&queue)) {
            switch (queue.type) {

            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (queue.key.keysym.scancode == SDL_SCANCODE_ESCAPE)       { running = false; }
                if (queue.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)    { running = false; }
                break;
            
            default:
                break;
            }
        }
        
        // TODO: Limit framerate here!
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
}

void destroy_rendering(SDL_Window **window) {

    SDL_DestroyWindow(*window);
    SDL_Quit();
}