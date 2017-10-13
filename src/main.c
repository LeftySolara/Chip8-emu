#include "Chip8Config.h"
#include "ui.h"
#include "chip8.h"
#include <stdlib.h>
#include <time.h>

int main (int argc, char **argv)
{
    printf("Chip8-emu version %d.%d\n%s\n\n",
           Chip8_VERSION_MAJOR, Chip8_VERSION_MINOR, Chip8_COPYRIGHT_LINE);

    if (argc < 2) {
        printf("No Chip-8 program specified. Exiting...\n");
        exit(1);
    }

    srand(time(NULL));
    const char *prog = argv[1];
    struct Chip8 *chip8 = chip8_init();
    chip8_load_prog(chip8, prog);

    struct ui *ui = ui_init();

    /* main loop flag */
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&ui->event) != 0) {
            if (ui->event.type == SDL_QUIT)
                quit = true;
            else if (ui->event.type == SDL_KEYDOWN) {
                switch (ui->event.key.keysym.sym) {
                case SDLK_q:
                    quit = true;
                    break;
                default:
                    break;
                }
            }
        }
    }

    ui_free(ui);
    chip8_free(chip8);

    return 0;
}
