#ifndef CHIP8_H
#define CHIP8_H

#define MEMORY_SIZE 4096
#define STACK_SIZE 16

struct Chip8 {

    /* CPU registers:

       The 16 8-bit general-purpose registers are referred to as
       Vx, where x is a hex digit. The VF register shouldn't be used by programs,
       as it's used as a flag by some instructions.

       The register I is mostly used to hold memory addresses, so usually
       just the lowest 12 bits are used.
    */
    unsigned char V[16];
    unsigned short I;


    /* Memory:

       The Chip-8 interpreter has 4K of memory. The first 512 bytes
       (0x000 to 0x1FF) are used by the interpreter and therefore
       should not be accessed by programs.
     */
    unsigned char memory[MEMORY_SIZE];


    /* Timer registers:

       When these are non-zero, they should count down at a rate of 60Hz.
       As long as the sound timer's value is greater than zero, the Chip-8
       buzzer will sound.
     */
    unsigned char delay_timer;
    unsigned char sound_timer;


    /* Display */
    unsigned char gfx[64][32];

    /* Keypad */
    unsigned char key[16];


    /* Other "pseudo registers" */
    unsigned short pc; /* program counter */
    unsigned short sp;  /* stack pointer */
    unsigned short stack[STACK_SIZE];
    unsigned short opcode;
};


struct Chip8 *chip8_init();
void chip8_free(struct Chip8 *chip8);

void chip8_load_prog(struct Chip8 *chip8, const char *prog);
void chip8_cycle(struct Chip8 *chip8);

unsigned char hex_to_bin(unsigned char n);
#endif
