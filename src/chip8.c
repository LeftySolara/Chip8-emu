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
       00EE: Returns from a subroutine
       1NNN: Jumps to address NNN
       2NNN: Calls subreoutine at address NNN
       6XNN: Sets the value of register VX to NN
       ANNN: Sets register I to NNN
     */
    chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];

    switch (chip8->opcode & 0xF000) {
    case 0x0000:
        switch (chip8->opcode & 0x000F) {
        case 0x0000: /* 00E0 */
            /* Clear the screen */
            break;
        case 0x000E: /* 00EE */
            if (chip8->sp == 0) {
                printf("Invalid return from subroutine\n");
                exit(3);
            }
            else
                chip8->pc = chip8->stack[chip8->sp--];
            break;
        }
    case 0x1000: /* 1NNN */
        chip8->pc = chip8->opcode & 0x0FFF;
        break;
    case 0x2000: /* 2NNN */
        if (chip8->sp >= STACK_SIZE) {
            printf("Stack size exceeded\n");
            exit(2);
        }
        else {
            chip8->stack[chip8->sp++] = chip8->pc;
            chip8->pc = (chip8->opcode & 0x0FFF);
        }
        break;
    case 0x6000: /* 6XNN */
        chip8->V[(chip8->opcode & 0x0F00) >> 8] = chip8->opcode & 0x00FF;
        chip8->pc += 2;
        break;
    case 0xA000: /* ANNN */
        chip8->I = (chip8->opcode & 0x0FFF);
        chip8->pc += 2;
        break;
    default:
        printf("Unknown opcode: 0x%X\n", chip8->opcode);
        chip8->pc += 2;
    }
}
