#include "chip8.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VX chip8->V[(chip8->opcode & 0x0F00) >> 8]
#define VY chip8->V[(chip8->opcode & 0x00F0) >> 8]

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
       3XNN: Skips the next instruction if VX equals NN
       4XNN: Skips the next instruction if VX is not equal to NN
       5XY0: Skips the next instruction if VX equals VY
       6XNN: Sets the value of register VX to NN
       7XNN: Adds NN to VX (carry flag not changed)
       8XY0: Sets VX to the vaule of VY
       8XY1: Sets VX to VX OR VY
       8XY2: Sets VX to VX AND VY
       8XY3: Sets VX to VX XOR VY
       8XY4: Sets VX to VX + VY with VF = carry
       8XY5: Sets Vx to VX - VY with VF = NOT borrow
       8XY6: Sets VX to VX SHR 1
       8XY7: Sets VX = VY - VX with VF = NOT borrow
       8XYE: Sets VX = SHL 1
       9XY0: Skips the next instruction if VX is not equal to VY
       ANNN: Sets register I to NNN
       BNNN: Jumps to address NNN + V0
       CXNN: Sets VX to the result of a bitwise AND on a random number and NN
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
    case 0x3000: /* 3XNN */
        if (VX == (chip8->opcode & 0x00FF))
            chip8->pc += 4;
        else
            chip8->pc += 2;
        break;
    case 0x400: /* 4XNN */
        if (VX != (chip8->opcode & 0x00FF))
            chip8->pc += 4;
        else
            chip8->pc += 2;
        break;
    case 0x5000: /* 5XY0 */
        if (VX == VY)
            chip8->pc += 4;
        else
            chip8->pc += 2;
        break;
    case 0x6000: /* 6XNN */
        VX = chip8->opcode & 0x00FF;
        chip8->pc += 2;
        break;
    case 0x7000: /* 7XNN */
        VX += (chip8->opcode & 0x00FF);
        chip8->pc += 2;
        break;
    case 0x8000:
        switch (chip8->opcode & 0x000F) {
        case 0x0000: /* 8XY0 */
            VX = VY;
            break;
        case 0x0001: /* 8XY1 */
            VX = VX | VY;
            break;
        case 0x0002: /* 8XY2 */
            VX = VX & VY;
            break;
        case 0x0003: /* 8XY3 */
            VX = VX ^ VY;
            break;
        case 0x0004: /* 8XY4 */
            chip8->V[15] = (VX + VY > 255) ? 1 : 0;
            VX += VY;
            break;
        case 0x0005: /* 8XY5 */
            chip8->V[15] = (VX > VY) ? 1 : 0;
            VX -= VY;
            break;
        case 0x0006: /* 8XY6 */
            chip8->V[15] = ((VX & 0x000F) == 1) ? 1 : 0;
            VX = VX >> 1;
            break;
        case 0x0007: /* 8XY7 */
            chip8->V[15] = (VY > VX) ? 1 : 0;
            VX = VY - VX;
            break;
        case 0x000E: /* 8XYE */
            chip8->V[15] = ((VX & 0xF000) == 1) ? 1 : 0;
            VX = VX << 1;
            break;
        }
        chip8->pc += 2;
    case 0x9000: /* 9XY0 */
        if (VX != VY)
            chip8->pc += 4;
        else
            chip8->pc += 2;
        break;
    case 0xA000: /* ANNN */
        chip8->I = (chip8->opcode & 0x0FFF);
        chip8->pc += 2;
        break;
    case 0xB000: /* BNNN */
        chip8->pc = chip8->V[0] + (chip8->opcode & 0x0FFF);
        break;
    case 0xC000: /* CXNN */
        VX = rand() & (chip8->opcode & 0x00FF);
        chip8->pc += 2;
        break;
    default:
        printf("Unknown opcode: 0x%X\n", chip8->opcode);
        chip8->pc += 2;
    }
}
