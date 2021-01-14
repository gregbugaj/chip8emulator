#ifndef CHIP8EMULATOR_CHIP8_H
#define CHIP8EMULATOR_CHIP8_H

#include <string>
#include "Chip8State.h"

/**
 * Chip-8 emulator
 */
class Chip8 {
public:

    /**
     * Emulate for number of cycles
     * @param cycles
     * @param memory
     */
    void emulate(long cycles, Chip8State &memory);

    /**
     *
     * @param filename
     * @param memory
     * @return
     */
    bool load(std::string &filename, Chip8State &memory);

    /**
     * Reset state
     * @param memory
     */
    void reset(Chip8State &memory);

    /**
     * Print dissasembled (mnemonic codes) of the current memory state
     * @param pc
     * @param memory
     */
    void disassemble(int pc, Chip8State &memory);
};


#endif //CHIP8EMULATOR_CHIP8_H
