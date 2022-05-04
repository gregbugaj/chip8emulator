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

#include <iostream>
#include <vector>
#include "types.h"

using namespace std;
using Byte = uint8_t;
using Word = uint16_t;
using u32 = unsigned  int;


struct Mem {
    static constexpr int MEMORY_SIZE = 4 * 1024; // 4k RAM, Addressable from 0x00 - 0xF00
    static constexpr int VIDEO_SIZE = 256;

    Byte memory[MEMORY_SIZE];  // Main memory 4K
    Byte screen[VIDEO_SIZE];  // This is screen memory

    void init(){
        for(u32 i = 0; i<MEMORY_SIZE;++i){
            memory[i] = 0;
        }

        for(u32 i = 0; i<VIDEO_SIZE;++i){
            screen[i] = 0;
        }
    }

    /**
   * Return memory address at specific index
   * @param index
   * @return
   */
    Byte& operator[](int index) {
        return memory[index];
    }
};

struct CPU {

    const static int BASE_ADDRESS = 0x200; // 0x200 (512) Start of most Chip-8 programs 0x000 to 0x1F reserved for interpreter
    static constexpr int STACK_SIZE = 16;
    const static int MAX_PROGRAM_SIZE = 3584; // MEMORY_SIZE - BASE_ADDRESS

    Word S[STACK_SIZE]; // Stack, max size 16 The stack is an array of 16 16-bit values
    Byte VF;  // Carry flag
    Word I;  // I is 16 bits wide
    Word PC;  // Program Counter - Program counter starts at 0x200
    Word SP;  // Stack Pointer
    Byte V[16]; //  16 general purpose 8-bit registers (#16 is a carry flag aka VF)
    Byte DT; // Delay timer
    Byte ST; // Sound timer
    Byte key[16]; // hex based keypad (0x0-0xF)

    /**
     * Reset current state
     */
    void reset(Mem& memory) {
        SP = 0; // 0xFA0 = 4000
        PC = CPU::BASE_ADDRESS;
        I = 0;
        DT = 0;
        ST = 0;
        memory.init();
    }

    /**
     * Dump state of registers
     */
    void dump() {
        printf("PC = 0x%04x \t SP = 0x%04x \t I = 0x%04x \t VF = 0x%02x \n", PC, SP, I, VF);
        printf("REGISTERS \t\t\t\tSTACK\n");
        for (uint i = 0; i < 0xF; ++i) {
            printf("V[%02x] = 0x%02x\t\t\tSP[%02x] = 0x%02x \n", i, V[i], i, S[i]);
        }
    }

    /**
     * Load rom into memory
     * @param rom
     */
    void load(Mem& memory, std::vector<Byte> rom){
        for (auto i = 0; i < rom.size(); ++i) {
            auto buf = rom[i];
            memory[CPU::BASE_ADDRESS + i] = buf;
            if (false) {
                cout << i
                     << "\t" << asHexString(2, buf)
                     << "\t" << asBitString(buf)
                     << "\t" << asDecString(4, buf) << endl;
            }
        }
    }
};

#endif //CHIP8EMULATOR_CHIP8STATE_H
