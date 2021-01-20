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
     */
    void disassemble(Chip8State &state);

    /**
     * Disassemble an instructions
     * @param pc
     * @param opcode
     */
    std::string disassemble(uint16_t pc, word_t opcode);
};

#endif //CHIP8EMULATOR_CHIP8DISASSEMBLER_H
