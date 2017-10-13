#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

struct ui {
    SDL_Window *main_window;
    SDL_Surface *main_surface;

    const int screen_width;
    const int screen_height;

    SDL_Event event;
};

struct ui *ui_init();
void ui_free(struct ui *ui);
