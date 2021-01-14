#ifndef CHIP8EMULATOR_CHIP8DISASSEMBLER_H
#define CHIP8EMULATOR_CHIP8DISASSEMBLER_H

#include <string>
#include "Chip8State.h"
#include "fileutil.h"

class Chip8Disassembler {
public:
    /**
     * Disassembler
     * @param filename
     */
    void dissassembly(const std::string &filename);

    /**
     * Disassembler
     * @param state
     */
    void dissassembly(Chip8State &state);
};
#endif //CHIP8EMULATOR_CHIP8DISASSEMBLER_H
