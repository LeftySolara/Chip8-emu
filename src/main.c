#include <stdio.h>
#include "Chip8Config.h"

int main (int argc, char **argv)
{
    printf("Chip8-emu version %d.%d\n%s\n",
           Chip8_VERSION_MAJOR, Chip8_VERSION_MINOR, Chip8_COPYRIGHT_LINE);

    return 0;
}
