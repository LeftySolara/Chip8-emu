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
        SDL_CreateWindowAndRenderer(ui->screen_width, ui->screen_height,
                                    0, &ui->main_window, &ui->main_renderer);
        SDL_SetRenderDrawColor(ui->main_renderer, 73, 100, 122, 100);
        SDL_RenderClear(ui->main_renderer);
        SDL_RenderPresent(ui->main_renderer);
    }

    return ui;
}

void ui_free(struct ui *ui)
{
    // SDL_FreeSurface(ui->main_surface);
    // ui->main_surface = NULL;
    SDL_DestroyWindow(ui->main_window);
    ui->main_window = NULL;
    SDL_DestroyRenderer(ui->main_renderer);
    ui->main_renderer = NULL;
    SDL_Quit();
    free(ui);
}

void ui_draw(struct ui *ui, unsigned char gfx[64][32])
{
    SDL_RenderClear(ui->main_renderer);
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 32; ++y) {
            if (gfx[x][y] == 0)
                SDL_SetRenderDrawColor(ui->main_renderer, 73, 100, 122, 100);
            else
                SDL_SetRenderDrawColor(ui->main_renderer, 197, 200, 198, 100);
            SDL_RenderDrawPoint(ui->main_renderer, x, y);
        }
    }
    SDL_RenderPresent(ui->main_renderer);
}
