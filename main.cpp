#include <iostream>
#include <stdlib.h>
#include "Chip8State.h"
#include "Chip8Disassembler.h"

int main(int args, char **argv) {
    std::string filename = argv[1];
    std::cout << "Starting CHIP 8 emulator : " << argv[1] << std::endl;

    Chip8Disassembler disassembler;
    disassembler.disassemble(filename);

//    disassembler.disassemble(0x200, 0xE000);
//    disassembler.disassemble(0x200, 0X017C);
    
    return EXIT_SUCCESS;
}
