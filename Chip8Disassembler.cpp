#include <iostream>
#include <iomanip>
#include "Chip8Disassembler.h"
#include "types.h"
#include "fileutil.h"

using namespace std;

void _disassemble(word_t opcode);

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


void Chip8Disassembler::dissassembly(const std::string &filename) {
    validateFileExists(filename);
    auto vec = readAsByteVector(filename);
    auto fsize = vec.size();
    std::cout << "ROM Size : " << fsize << std::endl;
    cout << showbase // show the 0x prefix
         << internal // fill between the prefix and the number
         << setfill('0'); // fill with 0s

    // Read in the rom
    auto state = Chip8State::init();
    for (auto i = 0; i < fsize; ++i) {
        auto buf = vec[i];
        state->memory[0x200 + i] = buf;
        if (false) {
            cout << i
                 << "\t" << asHexString(2, buf)
                 << "\t" << asBitString(buf)
                 << "\t" << asDecString(4, buf) << endl;
        }
    }

    dissassembly(*state);
}

void Chip8Disassembler::dissassembly(Chip8State &state) {

    for (int i = 0; i < 100; ++i) {
        word_t word = fetch(state);
        if (false)
            cout << i
                 << "\t" << asHexString(4, word)
                 << "\t" << asBitString(word)
                 << "\t" << asDecString(4, word) << endl;
        _disassemble(word);
    }
}

void _disassemble(word_t opcode) {
    // get the high byte lower 4 bits, as this is the code
    byte_t code = ((opcode >> 8) & 0xF0) >> 4;
    if (false) {

        cout << "\t" << asHexString(2, opcode)
             << "\t" << asBitString(opcode)
             << "\t" << asDecString(4, opcode) << endl;
        cout << "\t" << asHexString(2, code) << endl;
    }

    switch (code) {
        case 0x00: {
            switch (opcode & 0x00FF) {
                case 0xE0: // clear the screen
                {
                    printf("%-30s ; %s", "CLS", "clear the screen");
                    break;
                }
                case 0xEE: // return from subroutine call
                {
                    printf("%-30s ; %s", "RTS", "return from subroutine call");
                    break;
                }
            }
            break;
        }
        case 0x01: // (1nnn - JP addr )1NNN	Jumps to address NNN.
        {
            uint16_t addr = opcode & 0x0FFF;
            printf("%-10s $%04x\t\t; Jump to address $%2$01x", "JP", addr);
            break;
        }
        case 0x02: // 2nnn - CALL addr
        {
            uint16_t addr = opcode & 0x0FFF;
            printf("%-10s $%04x", "CALL", addr);
            break;
        }
        case 0x03: // 3xkk - SE Vx, byte
        {
            byte_t x = (opcode >> 8) & 0x0F;
            byte_t kk = opcode & 0xFF;
            printf("%-10s V[%01x], %01x \t\t; Skips the next instruction if V[%2$01x] equals 0",
                   "SE", x, kk);
            break;
        }
        case 0x04: // 4xkk - SNE Vx, byte
        {
            byte_t x = (opcode >> 8) & 0x0F;
            byte_t kk = opcode & 0xFF;
            printf("%-10s V[%01x], %01x", "SNE", x, kk);
            break;
        }

        case 0x05: // 5xy0 - SE Vx, Vy
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t y = (opcode >> 4) & 0x0F;
            printf("%-10s V[%01x], V[%01x]", "SE", x, y);
            break;
        }
        case 0x06: // 6xkk - LD Vx, byte
        {
            byte_t x = (opcode >> 8) & 0x0F;
            byte_t kk = opcode & 0xFF;
            printf("%-10s V[%01x], #$%02x ;   Sets V[%2$01x] to %3$02x", "LD", x, kk);
            break;
        }
        case 0x07: // 7xkk - ADD Vx, byte
        {
            byte_t x = (opcode >> 8) & 0x0F;
            byte_t kk = opcode & 0xFF;
            printf("%-10s V[%01x], %01x\t\t; Adds %1$01x to V[%2$01x] ", "ADD", x, kk);
            break;
        }
        case 0x08: {
            //8xy0 - LD Vx, Vy
            byte_t x = (opcode >> 8) & 0x0F;
            byte_t y = (opcode >> 4) & 0x0F;
            byte_t op = opcode & 0x0F;

            switch (op) {
                case 0x0: // 8xy0 - LD Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "LD", x, y);
                    break;
                case 0x1: // 8xy1 - OR Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "OR", x, y);
                    break;
                case 0x2: // 8xy2 - AND Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "AND", x, y);
                    break;
                case 0x3: // 8xy3 - XOR Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "XOR", x, y);
                    break;
                case 0x4: // 	8xy4 - ADD Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "ADD", x, y);
                    break;
                case 0x5:  // 8xy5 - SUB Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "SUB", x, y);
                    break;
                case 0x6:  // 8xy6 - SHR Vx {, Vy}
                    printf("%-10s V[%01x], V[%01x] ", "SHR", x, y);
                    break;
                case 0x7:  // 8xy7 - SUBN Vx, Vy
                    printf("%-10s V[%01x], V[%01x] ", "SUBN", x, y);
                    break;
                case 0xE:  // 8xyE - SHL Vx {, Vy}
                    printf("%-10s V[%01x], V[%01x] ", "SHL", x, y);
                    break;
                default: {
                    printf("%-10s  0x08   %02x", "UNHANDLED", op);
                }
            }
            break;
        }
        case 0x09: // 9xy0 - SNE Vx, Vy
        {
            byte_t x = (opcode >> 8) & 0x0F;
            byte_t y = (opcode >> 4) & 0x0F;
            printf("%-10s V[%01x], V[%01x]", "SNE", x, y);
            break;
        }
        case 0x0A:  // Annn - LD I, addr
        {
            uint16_t adr = opcode & 0x0FFF;
            printf("%-10s I, $%03x  ;   Sets I to address $%2$02x", "LD", adr);
            break;
        }
        case 0x0B: // Bnnn - JP V0, addr
        {
            uint16_t adr = opcode & 0x0FFF;
            printf("%-10s V0, $%03x", "JP", adr);
            break;
        }
        case 0x0C: // Cxkk - RND Vx, byte
        {
            byte_t x = (opcode >> 8) & 0x0F;
            byte_t kk = opcode & 0xFF;
            printf("%-10s V[%01x], #$%02x", "RND", x, kk);
            break;
        }
        case 0x0D: // Dxyn - DRW Vx, Vy, nibble
        {
            uint8_t x = (opcode >> 8) & 0x0F;
            uint8_t y = (opcode >> 4) & 0X0F;
            uint8_t n = opcode & 0x0F;

            printf("%-10s V[%01x], V[%01x], #$%01x", "DRW", x, y, n);
            break;
        }
        case 0x0E: //  Ex9E - SKP Vx
        {
            byte_t x = (opcode >> 8) & 0x0F;
            switch (opcode & 0x0F) {
                case 0x9E: // Ex9E - SKP Vx
                {
                    printf("%-10s V[%01x]", "SKP", x);
                    break;
                }
                case 0xA1: // ExA1 - SKNP Vx
                {
                    printf("%-10s V[%01x]", "SKNP", x);
                    break;
                }
                default: {
                    printf("%-10s %04x", "NOT IMPLEMENTED", code);
                }
            }
            break;
        }
        case 0x0F: //  Fx07 - LD Vx, DT
        {
            byte_t x = (opcode >> 8) & 0x0F;
            switch (opcode & 0xFF) {
                case 0x07: // Fx07 - LD Vx, DT
                {
                    printf("%-10s  V[%01x], DT", "LD", x);
                    break;
                }
                case 0x0A: // Fx07 - LD Vx, DT
                {
                    printf("%-10s V[%01x], K", "LD", x);
                    break;
                }
                case 0x15: // Fx15 - LD DT, Vx
                {
                    printf("%-10s DT, V[%01x]", "LD", x);
                    break;
                }
                case 0x18: // Fx18 - LD ST, Vx
                {
                    printf("%-10s ST, V[%01x]", "LD", x);
                    break;
                }
                case 0x1E: // Fx1E - ADD I, Vx
                {
                    printf("%-10s I, V[%01x]", "ADD", x);
                    break;
                }
                case 0x29: // Fx29 - LD F, Vx
                {
                    printf("%-10s F, V[%01x]", "LD", x);
                    break;
                }
                case 0x33: // Fx33 - LD B, Vx
                {
                    printf("%-10s B, V[%01x]", "LD", x);
                    break;
                }
                case 0x55: // Fx55 - LD [I], Vx
                {
                    printf("%-10s [I], V[%01x]", "LD", x);
                    break;
                }
                case 0x65: // Fx65 - LD Vx, [I]
                {
                    printf("%-10s V[%01x], [I]", "LD", x);
                    break;
                }
                default: {
                    printf("%-10s %04x", "NOT IMPLEMENTED", code);
                }
            }
            break;
        }
        default: {
            printf("%-10s %04x", "NOT IMPLEMENTED", code);

            cout << "\t" << asHexString(2, code)
                 << "\t" << asBitString(code)
                 << "\t" << asDecString(4, code) << endl;
        }
    }

    cout << endl;
}