#include "Chip8Config.h"
#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

int main (int argc, char **argv)
{
    printf("Chip8-emu version %d.%d\n%s\n\n",
           Chip8_VERSION_MAJOR, Chip8_VERSION_MINOR, Chip8_COPYRIGHT_LINE);

    if (argc < 2) {
        printf("No Chip-8 program specified. Exiting...\n");
        exit(1);
    }

    /* Sample SDL stuff to test that the library is working properly */
    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;

    const int screen_width = 640;
    const int screen_height = 480;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        printf("Could not initialize SDL. Error: %s\n", SDL_GetError());
    else {
        window = SDL_CreateWindow("Chip8-emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
        if (window == NULL)
            printf("Could not create window. Error: %s\n", SDL_GetError());
        else {
            surface = SDL_GetWindowSurface(window);
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(window);
            SDL_Delay(5000);
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    srand(time(NULL));
    const char *prog = argv[1];
    struct Chip8 *chip8 = chip8_init();
    chip8_load_prog(chip8, prog);
    chip8_free(chip8);

    return 0;
}
