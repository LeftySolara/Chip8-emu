#include "chip8.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

struct Chip8 *chip8_init()
{
    struct Chip8 *chip8 = malloc(sizeof(*chip8));

    /* Clear the memory, display, and stack */
    memset(chip8->memory, 0, MEMORY_SIZE);
    memset(chip8->gfx, 0, 64 * 32);
    memset(chip8->stack, 0, STACK_SIZE * 2);

    chip8->pc = 0x200;
    chip8->sp = 0;
    chip8->I = 0;
    chip8->opcode = 0;

    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    /* load fontset */
    for (int i = 0; i < 80; ++i)
        chip8->memory[i] = chip8_fontset[i];

    return chip8;
}

void chip8_free(struct Chip8 *chip8)
{
    free(chip8);
}

/* Load the program into memory */
void chip8_load_prog(struct Chip8 *chip8, const char *prog)
{
    int buffer_size = MEMORY_SIZE - 512;
    unsigned char buffer[buffer_size];

    FILE *fp = fopen(prog, "rb");
    fread(buffer, sizeof(unsigned char), buffer_size, fp);

    for (int i = 0; i < buffer_size; ++i)
        chip8->memory[i + 512] = buffer[i];
}

void chip8_cycle(struct Chip8 *chip8)
{
    /* Find the current opcode and execute the instruction.

       List of opcodes (more will be listed as they are implemented):
       6XNN: Sets the value of register VX to NN
     */
    chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];

    switch (chip8->opcode & 0xF000) {
    case 0x6000:
        chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->opcode & 0x00FF;
        chip8->pc += 2;
        break;
    }
}
