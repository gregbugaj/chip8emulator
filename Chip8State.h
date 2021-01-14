#ifndef CHIP8EMULATOR_CHIP8STATE_H
#define CHIP8EMULATOR_CHIP8STATE_H

enum OPCODE {
    OP_1 = 0x22,
    OP_2 = 0x22,
    OP_3 = 0x22,
    OP_4 = 0x22,
};

class Chip8State {

public :
    // 0x200 (512) Start of most Chip-8 programs
    // 0x000 to 0x1F reserved for interpreter
    int BASE_ADDRESS = 0x200;

    uint8_t *memory;  // Main memory 4K
    uint8_t *screen;  // This is memory[0xF00];
    uint8_t VF;  // Carry flag
    uint16_t I;  // I is 16 bits wide
    int PC;  // Program Counter
    uint16_t SP;  // Stack Pointer
    uint8_t delay;
    uint8_t sound;
    uint8_t V[16]; //  16 general purpose 8-bit registers
    uint16_t ST[16]; // Stack, max size 16 The stack is an array of 16 16-bit values

    /**
     * Return memory address at specific index
     * @param index
     * @return
     */
    int operator[](int index) {
        return memory[index];
    }

    static Chip8State *init() {
        auto *s = (Chip8State *) calloc(sizeof(Chip8State), 1);
        s->memory = (uint8_t *) calloc(1024 * 4, sizeof(char)); // 4k RAM, Addressable from 0x00 - 0xF00
        s->screen = &s->memory[0xF00]; // 3840
        s->SP = 0xFA00; // 0xFA0 = 4000
        s->PC = 0x200; // Program counter starts at 0x200
        s->I = 0;

        return s;
    }

    /**
     * Dump state of memory
     */
    void dump() {
        printf("PC = 0x%04x \t SP = 0x%04x \t I = 0x%04x \t VF = 0x%02x \n", PC, SP, I, VF);
        printf("REGISTERS \t\t\t\tSTACK\n");
        for (uint i = 0; i < 0xF; ++i) {
            printf("V[%02x] = 0x%02x\t\t\tSP[%02x] = 0x%02x \n", i, V[i], i, ST[i]);
        }
    }
};


#endif //CHIP8EMULATOR_CHIP8STATE_H
