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
    void emulate(long cycles, CPU &state, Mem& memory);

    /**
     *
     * @param filename
     * @param memory
     * @return
     */
    bool load(std::string &filename, CPU &state, Mem& memory);
};


#endif //CHIP8EMULATOR_CHIP8_H
