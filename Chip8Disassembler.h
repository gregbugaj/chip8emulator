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
    void disassemble(const std::string &filename);

    /**
     * Disassembler
     * @param state
     * @param memory
     */
    void disassemble(CPU &state, Mem& memory);

    /**
     * Disassemble an instructions
     * @param pc
     * @param opcode
     */
    std::string disassemble(Word pc, Word opcode);
};

#endif //CHIP8EMULATOR_CHIP8DISASSEMBLER_H
