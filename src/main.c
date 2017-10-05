#include <stdio.h>
#include "Chip8Config.h"
#include "chip8.h"

int main (int argc, char **argv)
{
    printf("Chip8-emu version %d.%d\n%s\n",
           Chip8_VERSION_MAJOR, Chip8_VERSION_MINOR, Chip8_COPYRIGHT_LINE);

    struct Chip8 *chip8 = chip8_init();
    chip8_free(chip8);

    return 0;
}
