#ifndef CHIP8EMULATOR_CHIP8STATE_H
#define CHIP8EMULATOR_CHIP8STATE_H

/**
 * Memory layout
 * 4K memory
 * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
 * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
 * 0x200-0xFFF - Program ROM and work RAM
// * 0x200-0xE9F - Program ROM and work RAM
// * 0xEA0-0xEFF - Stack memory
// * 0xF00-0xFFF - Video memory
 */
class Chip8State {

public :
    const static int BASE_ADDRESS = 0x200; // 0x200 (512) Start of most Chip-8 programs 0x000 to 0x1F reserved for interpreter
    const static int MEMORY_SIZE = 4 * 1024; // 4k RAM, Addressable from 0x00 - 0xF00
    const static int VIDEO_SIZE = 256;
    const static int STACK_SIZE = 16;
    const static int MAX_PROGRAM_SIZE = 3584; // MEMORY_SIZE - BASE_ADDRESS

    uint8_t *memory;  // Main memory 4K
    uint8_t *screen;  // This is screen
    uint16_t S[STACK_SIZE]; // Stack, max size 16 The stack is an array of 16 16-bit values
    uint8_t VF;  // Carry flag
    uint16_t I;  // I is 16 bits wide
    uint16_t PC;  // Program Counter
    uint8_t SP;  // Stack Pointer
    uint8_t V[16]; //  16 general purpose 8-bit registers
    uint8_t DT; // Delay timer
    uint8_t ST; // Sound timer
    uint8_t key[16]; // hex based keypad (0x0-0xF)

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
        s->memory = (uint8_t *) calloc(Chip8State::MEMORY_SIZE, sizeof(char));
        s->screen = (uint8_t *) calloc(Chip8State::VIDEO_SIZE, sizeof(char)); //&s->memory[0xF00]; // 3840
        s->SP = 0; // 0xFA0 = 4000
        s->PC = Chip8State::BASE_ADDRESS; // Program counter starts at 0x200
        s->I = 0;
        s->DT = 0;
        s->ST = 0;

        return s;
    }

    /**
     * Dump state of memory
     */
    void dump() {
        printf("PC = 0x%04x \t SP = 0x%04x \t I = 0x%04x \t VF = 0x%02x \n", PC, SP, I, VF);
        printf("REGISTERS \t\t\t\tSTACK\n");
        for (uint i = 0; i < 0xF; ++i) {
            printf("V[%02x] = 0x%02x\t\t\tSP[%02x] = 0x%02x \n", i, V[i], i, S[i]);
        }
    }
};

#endif //CHIP8EMULATOR_CHIP8STATE_H
