#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Chip8State.h"
#include "Chip8Disassembler.h"

void cycle(Chip8State *state);

void dumpState(Chip8State *state);

void dump(unsigned char *buffer, long size);

void disassemble(unsigned char *buffer, int pc);

bool load(Chip8State *state, char *filename);

// ops
void OpF(Chip8State *state, uint8_t *code);

// 0x200 (512) Start of most Chip-8 programs
// 0x000 to 0x1F reserved for interpreter
int BASE_ADDRESS = 0x200;

int main(int args, char **argv) {
    std::string filename = argv[1];
    std::cout << "Starting CHIP 8 emulator : " << argv[1] << std::endl;
    Chip8Disassembler diss;
    diss.dissassembly(filename);
    return EXIT_SUCCESS;
}


//void dump(unsigned char *buffer, long romsize) {
//    int pc = BASE_ADDRESS;
//    while (pc < romsize) {
//        disassemble(buffer, pc);
//        pc += 2;    // advance by 2 bytes, 1 for opcode 1 for value
//    }
//}
