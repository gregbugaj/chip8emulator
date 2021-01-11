#include <iostream>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint8_t *memory;  // Main memory 4K
    uint8_t *screen;  // This is memory[0xF00];
    uint8_t VF;  // Carry flag
    uint16_t I;  // I is 16 bits wide
    int PC;  // Program Counter
    uint16_t SP;  // Stack Pointer
    uint8_t delay;
    uint8_t sound;
    uint8_t V[16]; //  16 general purpose 8-bit registers
    uint16_t ST[16]; // Stack, max size 16

} Chip8State;

void cycle(Chip8State *state);

void dumpstate(Chip8State *state);

void dump(unsigned char *buffer, int fsize);

void disassemble(unsigned char *buffer, int pc);

void emulate(unsigned char *buffer, int fsize);

Chip8State *init();

// ops
void OpF(Chip8State *state, uint8_t *code);

int main(int args, char **argv) {
    std::cout << "Starting CHIP 8 emulator : " << argv[1] << std::endl;

    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
        printf("error: Couldn't open %s\n", argv[1]);
        exit(1);
    }

    //Get the file size and read it into a memory buffer
    // 0x200 = 512
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    printf("ROM Size : %d\n", fsize);
    //Read the file into memory at 0x200 and close it.
    unsigned char *buffer = (unsigned char *) malloc(fsize);
    fread(buffer, fsize, 1, f);
    fclose(f);

    printf("%-10s %-10s %-10s %-10s \n", "Address", "Opcode", "Code", "Operand");
    dump(buffer, fsize);

    Chip8State *state = init();
    dumpstate(state);

    //emulate(buffer, fsize);

    return EXIT_SUCCESS;
}


void emulate(unsigned char *buffer, int fsize) {
    printf("\n Starting Emulation \n");
    /*
    int cycles = 0 ;
    while(state->PC < (fsize + 0x200))
    {
        cycle(state);
        ++cycles;
        if(cycles == 4)
            break;
    }
    dumpstate(state);
    */
}

void cycle(Chip8State *state) {
    state->PC += 2;
}

void dumpstate(Chip8State *state) {
    printf("\n Dumping State \n");
    printf("PC = %04x \t SP = %04x \t I = %04x \t VF = 0x%02x \n", state->PC, state->SP, state->I, state->VF);

/*	for(uint i = 0; i < 0xF; ++i)
	{
		printf("V[%02x] = 0x%02x  ",i, state->V[i]);
	}*/
}

void dump(unsigned char *buffer, int fsize) {
    int pc = 0x200;
    while (pc < (fsize + 0x200)) {
        disassemble(buffer, pc);
        pc += 2;    // advance by 2 bytes, 1 for opcode 1 for value
    }
}

void disassemble(unsigned char *buffer, int pc) {
    uint8_t *code = &buffer[pc];
    int nibble = (*code & 0xf0) >> 4;
    printf("%04x    %02x %02x\t\t", pc, code[0], code[1]);
    return;
    switch (nibble) {

        case 0x00: {
            switch (code[1]) {
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
            uint16_t addr = ((code[0] & 0x0f) << 8) | code[1];
            printf("%-10s $%04x\t\t; Jump to address $%2$01x", "JP", addr);
            break;
        }
        case 0x02: // 2nnn - CALL addr
        {
            uint16_t addr = ((code[0] & 0x0f) << 8) | code[1];
            printf("%-10s $%04x", "CALL", addr);
            break;
        }
        case 0x03: // 3xkk - SE Vx, byte
        {
            uint8_t x = code[0] & 0x0F;
            uint8_t kk = code[1];
            printf("%-10s V[%01x], %01x \t\t; Skips the next instruction if V[%2$01x] equals 0",
                   "SE", x, kk);
            break;
        }
        case 0x04: // 4xkk - SNE Vx, byte
        {
            uint8_t x = code[0] & 0x0F;
            uint8_t kk = code[1];
            printf("%-10s V[%01x], %01x", "SNE", x, kk);
            break;
        }
        case 0x05: // 5xy0 - SE Vx, Vy
        {
            uint8_t x = code[0] & 0x0F;
            uint8_t y = (code[1] & 0xF0) >> 4;
            printf("%-10s V[%01x], V[%01x]", "SE", x, y);
            break;
        }
        case 0x06: // 6xkk - LD Vx, byte
        {
            uint8_t reg = code[0] & 0x0f;
            printf("%-10s V[%01x], #$%02x ;   Sets V[%2$01x] to %3$02x", "LD", reg,
                   code[1]);
        }
            break;
        case 0x07: // 7xkk - ADD Vx, byte
        {
            uint8_t x = code[0] & 0x0F;
            uint8_t kk = code[1];
            printf("%-10s V[%01x], %01x\t\t; Adds %3$01x to V[%2$01x] ", "ADD", x,
                   kk);
            break;
        }
        case 0x08: {
            //8xy0 - LD Vx, Vy
            int op = code[1] & 0x0F;
            uint8_t x = code[0] & 0x0F;
            uint8_t y = (code[1] & 0xF0) >> 4;

            switch (op) {
                case 0: // 8xy0 - LD Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "LD", x, y);
                    break;
                case 1: // 8xy1 - OR Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "OR", x, y);
                    break;
                case 2: // 8xy2 - AND Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "AND", x, y);
                    break;
                case 3: // 8xy3 - XOR Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "XOR", x, y);
                    break;
                case 4: // 	8xy4 - ADD Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "ADD", x, y);
                    break;
                case 5:  // 8xy5 - SUB Vx, Vy
                    printf("%-10s V[%01x], V[%01x]", "SUB", x, y);
                    break;
                case 6:  // 8xy6 - SHR Vx {, Vy}
                    printf("%-10s V[%01x], V[%01x] ", "SHR", x, y);
                    break;
                case 7:  // 8xy7 - SUBN Vx, Vy
                    printf("%-10s V[%01x], V[%01x] ", "SUBN", x, y);
                    break;
                case 0xE:  // 8xyE - SHL Vx {, Vy}
                    printf("%-10s V[%01x], V[%01x] ", "SHL", x, y);
                    break;
                default:
                    printf("%-10s  0x08   %02x", "UNHANDLED", op);
            }

            break;
        }
        case 0x09: // 9xy0 - SNE Vx, Vy
        {
            uint8_t x = code[0] & 0x0F;
            uint8_t y = (code[1] & 0xF0) >> 4;

            printf("%-10s V[%01x], V[%01x]", "SNE", x, y);
            break;
        }
        case 0x0a:  // Annn - LD I, addr
        {
            uint16_t adr = ((code[0] & 0x0f) << 8) | code[1];
            printf("%-10s I, $%03x  ;   Sets I to address $%2$02x", "LD", adr);
        }
            break;
        case 0x0b: // Bnnn - JP V0, addr
        {
            uint16_t adr = ((code[0] & 0x0f) << 8) | code[1];
            printf("%-10s V0, $%03x", "JP", adr);
            break;
        }
        case 0x0c: // Cxkk - RND Vx, byte
        {
            uint8_t x = code[0] & 0x0F;
            uint8_t kk = code[1];

            printf("%-10s V[%01x], #$%02x", "RND", x, kk);
            break;
        }
        case 0x0d: // Dxyn - DRW Vx, Vy, nibble
        {
            uint8_t x = code[0] & 0x0F;
            uint8_t y = (code[1] & 0xF0) >> 4;
            uint8_t n = code[1] & 0x0F;

            printf("%-10s V[%01x], V[%01x], #$%01x", "DRW", x, y, n);
            break;
        }
        case 0x0E: {
            int op = code[1];
            uint8_t x = code[0] & 0x0F;

            switch (op) {
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
            }

            break;
        }
        case 0x0f: {
            int op = code[1];
            uint8_t x = code[0] & 0x0F;

            switch (op) {
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
            }
        }
            printf("%-10s %04x", "NOT IMPLEMENTED", code);
    }
    printf("\n");
}

Chip8State *init() {
    Chip8State *s = (Chip8State *) calloc(sizeof(Chip8State), 1);
    s->memory = (uint8_t *) calloc(1024 * 4, 1); // 4k RAM, Addressable from 0x00 - 0xF00
    s->screen = &s->memory[0xF00]; // 3840
    s->SP = 0xfa0; // 4000
    s->PC = 0x200; // 512

    return s;
}
