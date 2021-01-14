#include <iostream>
#include <stdlib.h>
#include "Chip8State.h"
#include "Chip8Disassembler.h"

int main(int args, char **argv) {
    std::string filename = argv[1];
    std::cout << "Starting CHIP 8 emulator : " << argv[1] << std::endl;
    Chip8Disassembler diss;
    diss.dissassembly(filename);

    return EXIT_SUCCESS;
}
