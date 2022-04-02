#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "config.h"

int main(int argc, char *argv[]) {

    // Init. the SDL2 timer and video subsystem.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create a new window in the middle of the screen, with specified dimensions.
    SDL_Window *window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDHT, SCREEN_HEIGHT, false);
    if (window == NULL) {
        fprintf(stderr, "Failed to create a window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Only for testing:
    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}