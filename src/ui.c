#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

struct ui *ui_init()
{
    struct ui *ui = malloc(sizeof(*ui));
    *(int *)&ui->screen_width = 640;
    *(int *)&ui->screen_height = 320;

    if ((SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        printf("Could not initialize SDL. SDL_Error: %s\n", SDL_GetError());
    else {
        ui->main_window = SDL_CreateWindow("Chip8-emu",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           ui->screen_width, ui->screen_height,
                                           SDL_WINDOW_SHOWN);
        if (ui->main_window == NULL)
            printf("Could not create SDL window. SDL_Error: %s\n", SDL_GetError());
        else {
            ui->main_surface = SDL_GetWindowSurface(ui->main_window);
            SDL_FillRect(ui->main_surface, NULL, SDL_MapRGB(ui->main_surface->format,
                                                            0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(ui->main_window);
        }
    }

    return ui;
}

void ui_free(struct ui *ui)
{
    SDL_FreeSurface(ui->main_surface);
    ui->main_surface = NULL;
    SDL_DestroyWindow(ui->main_window);
    ui->main_window = NULL;
    SDL_Quit();
    free(ui);
}
