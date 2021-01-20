#include <iostream>
#include "Chip8Disassembler.h"
#include "types.h"
#include "fileutil.h"
#include "fmt/color.h"

using namespace std;
using namespace fmt;

/**
 * Fetch next word (2 bytes) from the memory
 * @param state
 * @return
 */
word_t fetch(Chip8State &state) {
    word_t word = (state[state.PC] << 8) | state[state.PC + 1];
    state.PC += 2;
    return word;
}

void Chip8Disassembler::disassemble(const std::string &filename) {
    validateFileExists(filename);
    auto vec = readAsByteVector(filename);
    auto fsize = vec.size();
    std::cout << "ROM Size : " << fsize << std::endl;
    // Read in the rom into memory at offset of 0x200
    auto state = Chip8State::init();
    for (auto i = 0; i < fsize; ++i) {
        auto buf = vec[i];
        state->memory[Chip8State::BASE_ADDRESS + i] = buf;
        if (false) {
            cout << i
                 << "\t" << asHexString(2, buf)
                 << "\t" << asBitString(buf)
                 << "\t" << asDecString(4, buf) << endl;
        }
    }

    disassemble(*state);
}

void Chip8Disassembler::disassemble(Chip8State &state) {
    int index = 0;
    for (;;) {
        auto pc = state.PC;
        word_t word = fetch(state);
        if (0x0 == word) {
            break;
        }
        if (false)
            cout << index
                 << "\t" << asHexString(4, word)
                 << "\t" << asBitString(word)
                 << "\t" << asDecString(4, word) << endl;
        disassemble(pc, word);
        ++index;
    }
}


std::string Chip8Disassembler::disassemble(uint16_t pc, word_t opcode) {
    //    nnn or addr - A 12-bit value prefixed with $
    //    n or nibble - A 4-bit value
    //    x - A 4-bit value
    //    y - A 4-bit value
    //    kk or byte - An 8-bit value

    uint16_t code = opcode & 0xF000;
    if (false) {

        cout << "\t" << asHexString(2, opcode)
             << "\t" << asBitString(opcode)
             << "\t" << asDecString(4, opcode) << endl;
        cout << "\t" << asHexString(2, code) << endl;
    }
    std::string str;
    auto color = fg(fmt::terminal_color::bright_blue);
    switch (code) {
        case 0x00: {
            switch (opcode) {
                case 0x00E0: // CLS  clear the video
                {
                    str = fmt::format(color, "{0:<6X}${1:<8X}{2:<6};\t00E0 - CLS : Clear the screen", pc,
                                      opcode, "CLS");
                    break;
                }
                case 0x00EE: // RET return from subroutine call
                {
                    str = fmt::format(color, "{0:<6X}${1:<8X}{2:<6};\t00EE - RET : Return from subroutine call",
                                      pc,
                                      opcode, "RTS");
                    break;
                }
                default: {
                    str = fmt::format(color, "{0:<6X}${1:<8X};{2:<6}{3:<4X}", pc, opcode, "DATA", code);
                }
            }
            break;
        }
        case 0x1000: // (1nnn - JP addr )1NNN	Jumps to address NNN.
        {
            uint16_t addr = opcode & 0x0FFF;
            str = fmt::format(color, "{0:<6X}${1:<8X}{2:<6}{3:<03X};\t1nnn - JP addr : Jumps to address nnn", pc,
                              opcode, "JP",
                              addr);
            break;
        }
        case 0x2000: // 2nnn - CALL addr
        {
            uint16_t addr = opcode & 0x0FFF;
            str = fmt::format(color, "{0:<6X}${1:<8X}{2:<6}{3:<03x};\t2nnn - CALL addr", pc,
                              opcode, "CALL",
                              addr);
            break;
        }
        case 0x3000: // 3xkk - SE Vx, byte
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t kk = opcode & 0xFF;
            str = fmt::format(color,
                              "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], {4:<2x};\t3xkk - SE Vx, byte : Skip next instruction if Vx = kk",
                              pc, opcode, "SE", x, kk);
            break;
        }
        case 0x4000: // 4xkk - SNE Vx, byte
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t kk = opcode & 0xFF;
            str = fmt::format(color,
                              "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], {4:<2x};\t4xkk - SNE Vx, byte: Skip next instruction if Vx != kk",
                              pc, opcode, "SNE", x, kk);
            break;
        }

        case 0x5000: // 5xy0 - SE Vx, Vy
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t y = (opcode >> 4) & 0x0F;
            str = fmt::format(color,
                              "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<2x}];\t5xy0 - SE Vx, Vy:  Skip next instruction if Vx = Vy",
                              pc, opcode, "SNE", x, y);
            break;
        }
        case 0x6000: // 6xkk - LD Vx, byte
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t kk = opcode & 0xFF;
            str = fmt::format(color,
                              "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], {4:<02x};\t6xkk - LD Vx, byte : Store value kk into register Vx",
                              pc, opcode, "LD", x, kk);
            break;
        }
        case 0x7000: // 7xkk - ADD Vx, byte
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t kk = opcode & 0xFF;
            str = fmt::format(color,
                              "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], {4:<02x};\t7xkk - ADD Vx, byte : Adds the value kk to the value of register Vx",
                              pc, opcode, "ADD", x, kk);
            break;
        }
        case 0x8000: //8xy0 - LD Vx, Vy
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t y = (opcode >> 4) & 0x0F;
            uint8_t op = opcode & 0x0F;
            switch (op) {
                case 0x0: // 8xy0 - LD Vx, Vy
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xy0 - LD Vx, Vy : Stores the value of register Vy in register Vx",
                                      pc, opcode, "LD", x, y);
                    break;
                }
                case 0x1: // 8xy1 - OR Vx, Vy
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xy1 - OR Vx, Vy : Performs a bitwise OR on the values of Vx and Vy",
                                      pc, opcode, "OR", x, y);
                    break;
                }

                case 0x2: // 8xy2 - AND Vx, Vy
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xy1 - AND Vx, Vy : Performs a bitwise AND on the values of Vx and Vy",
                                      pc, opcode, "AND", x, y);
                    break;
                }

                case 0x3: // 8xy3 - XOR Vx, Vy
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xy3 - XOR Vx, Vy : Performs a bitwise exclusive OR on the values of Vx and Vy",
                                      pc, opcode, "XOR", x, y);
                    break;
                }
                case 0x4: // 	8xy4 - ADD Vx, Vy
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xy4 - ADD Vx, Vy : The values of Vx and Vy are added together",
                                      pc, opcode, "ADD", x, y);
                    break;
                }
                case 0x5:  // 8xy5 - SUB Vx, Vy
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xy5 - SUB Vx, Vy : The Vy is subtracted from Vx",
                                      pc, opcode, "SUB", x, y);
                    break;
                }
                case 0x6:  // 8xy6 - SHR Vx {, Vy}
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xy6 - SHR Vx , Vy : Set Vx = Vx SHR 1",
                                      pc, opcode, "SHR", x, y);
                    break;
                }
                case 0x7:  // 8xy7 - SUBN Vx, Vy
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xy7 - SUBN Vx, Vy : Set Vx = Vy - Vx, set VF = NOT borrow",
                                      pc, opcode, "SUBN", x, y);
                    break;
                }
                case 0xE:  // 8xyE - SHL Vx {, Vy}
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t8xyE - SHL Vx, Vy: Set Vx = Vx SHL 1",
                                      pc, opcode, "SHR", x, y);
                    break;
                }
                default: {
                    str = fmt::format(color, "{0:<6X}${1:<8X};{2:<6}{3:<4X}", pc, opcode, "DATA", op);
                    break;
                }
            }
            break;
        }
        case 0x9000: // 9xy0 - SNE Vx, Vy
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t y = (opcode >> 4) & 0x0F;
            str = fmt::format(color,
                              "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}];\t9xy0 - SNE Vx, Vy : Skip next instruction if Vx != Vy",
                              pc, opcode, "SNE", x, y);
            break;
        }
        case 0xA000:  // Annn - LD I, addr
        {
            uint16_t addr = opcode & 0x0FFF;
            str = fmt::format(color, "{0:<6X}${1:<8X}{2:<6}I, {3:<1X};\tAnnn - LD I, addr : Sets I to address nnn",
                              pc, opcode, "LD", addr);
            break;
        }
        case 0xB000: // Bnnn - JP V0, addr
        {
            uint16_t addr = opcode & 0x0FFF;
            str = fmt::format(color,
                              "{0:<6X}${1:<8X}{2:<6}{3:<03x};\tBnnn - JP V0, addr :  Jump to location nnn + V0",
                              pc,
                              opcode, "JP",
                              addr);
            break;
        }
        case 0xC000: // Cxkk - RND Vx, byte
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t kk = opcode & 0xFF;
            str = fmt::format(color,
                              "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], {4:<02x};\tCxkk - RND Vx, byte : Set Vx = random byte AND kk",
                              pc, opcode, "RND", x, kk);
            break;
        }
        case 0xD000: // Dxyn - DRW Vx, Vy, nibble
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t y = (opcode >> 4) & 0X0F;
            uint8_t n = opcode & 0x0F;
            str = fmt::format(color, "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], V[{4:<1X}] {5:<02x};"
                                     "\tDxyn - DRW Vx, Vy, nibble : Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision",
                              pc, opcode, "DRW", x, y, n);
            break;
        }
        case 0xE000: //  Ex9E - SKP Vx
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t op = opcode & 0xFF;
            switch (op) {
                case 0x9E: // Ex9E - SKP Vx
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}];\tEx9E - SKP Vx : Skip next instruction if key with the value of Vx is pressed",
                                      pc,
                                      opcode, "SKP", x);
                    break;
                }
                case 0xA1: // ExA1 - SKNP Vx
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}];\tExA1 - SKNP Vx :  Skip next instruction if key with the value of Vx is not pressed",
                                      pc,
                                      opcode, "SKNP", x);
                    break;
                }
                default: {
                    str = fmt::format(color, "{0:<6X}${1:<8X};{2:<6}{3:<4X}", pc, opcode, "DATA", opcode);
                }
            }
            break;
        }
        case 0xF000: //  Fx07 - LD Vx, DT
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            switch (opcode & 0xFF) {
                case 0x07: // Fx07 - LD Vx, DT
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}] DT;\tFx07 - LD Vx, DT : The value of DT is placed into Vx",
                                      pc,
                                      opcode, "LD", x);
                    break;
                }
                case 0x0A: //  Fx0A - LD Vx, K
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], K;\tFx0A - LD Vx, K : Wait for a key press, store the value of the key in Vx",
                                      pc,
                                      opcode, "LD", x);
                    break;
                }
                case 0x15: // Fx15 - LD DT, Vx
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}DT, V[{3:<1X}];\tFx15 - LD DT, Vx : Set delay timer = Vx",
                                      pc,
                                      opcode, "LD", x);
                    break;
                }
                case 0x18: // Fx18 - LD ST, Vx
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}ST, V[{3:<1X}];\tFx18 - LD ST, Vx : Set sound timer = Vx",
                                      pc,
                                      opcode, "LD", x);
                    break;
                }
                case 0x1E: // Fx1E - ADD I, Vx
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}I, V[{3:<1X}];\tFx1E - ADD I, Vx : The values of I and Vx are added",
                                      pc,
                                      opcode, "ADD", x);
                    break;
                }
                case 0x29: // Fx29 - LD F, Vx
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}F, V[{3:<1X}];\tFx29 - LD F, Vx : Set I = location of sprite for digit Vx",
                                      pc,
                                      opcode, "LD", x);
                    break;
                }
                case 0x33: // Fx33 - LD B, Vx
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}B, V[{3:<1X}];\tFx33 - LD B, Vx : Store BCD representation of Vx in memory locations I, I+1, and I+2",
                                      pc,
                                      opcode, "LD", x);
                    break;
                }
                case 0x55: // Fx55 - LD [I], Vx
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}[I], V[{3:<1X}];\tFx55 - LD [I], Vx : Store registers V0 through Vx in memory starting at location I",
                                      pc,
                                      opcode, "LD", x);
                    break;
                }
                case 0x65: // Fx65 - LD Vx, [I]
                {
                    str = fmt::format(color,
                                      "{0:<6X}${1:<8X}{2:<6}V[{3:<1X}], [I];\tFx55 - LD [I], Vx : Read registers V0 through Vx from memory starting at location I",
                                      pc,
                                      opcode, "LD", x);
                    break;
                }
                default: {
                    str = fmt::format(color, "{0:<6X}${1:<8X};{2:<6}{3:<4X}", pc, opcode, "DATA", code);
                    break;
                }
            }
            break;
        }
        default: {
            str = fmt::format(color, "{0:<6X}${1:<8X};{2:<6}{3:<4X}", pc, opcode, "DATA", code);
        }
    }
    cout << str << endl;
    return str;
}