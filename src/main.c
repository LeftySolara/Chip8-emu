#include "Chip8Config.h"
#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    printf("Chip8-emu version %d.%d\n%s\n\n",
           Chip8_VERSION_MAJOR, Chip8_VERSION_MINOR, Chip8_COPYRIGHT_LINE);

    if (argc < 2) {
        printf("No Chip-8 program specified. Exiting...\n");
        exit(1);
    }

    const char *prog = argv[1];
    struct Chip8 *chip8 = chip8_init();
    chip8_load_prog(chip8, prog);
    chip8_free(chip8);

    return 0;
}
