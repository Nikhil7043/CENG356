/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "header.h"

// MIPS Opcode Definitions
#define OP_LA      0b101111  // Load Address (pseudo-instruction)
#define OP_LB      0b100000  // Load Byte
#define OP_BGE     0b000111  // Branch if Greater or Equal
#define OP_LW      0b100011  // Load Word
#define OP_SW      0b101011  // Store Word
#define OP_ADDI    0b001000  // Add Immediate
#define OP_J       0b000010  // Jump
#define OP_R_TYPE  0b000000  // R-type instruction (opcode = 0)

// R-type Function Codes
#define FUNCT_ADD  0b100000  // Add function code

// Register numbers for reference
#define REG_ZERO   0         // $zero register

// This is the function to fill in the data section.
void setupDataMemory(char* base_address,
                     unsigned int offset,
                     char *datasection,
                     unsigned int numberOfBytes){
    // Copy each byte from datasection to memory starting at offset
    for (unsigned int i = 0; i < numberOfBytes; i++) {
        *(base_address + offset + i) = datasection[i];
    }
}

// This is the function to build I type instruction based on Instruction_storage
unsigned int buildIInstruction(unsigned char opcode,
                               unsigned char rs,
                               unsigned char rt,
                               int immediate){
    unsigned int machineCode = 0;
    unsigned int mask = 0;

    // I-type format: [opcode(6)][rs(5)][rt(5)][immediate(16)]
    machineCode = immediate & 0x0000FFFF;                    // immediate field
    mask = ((unsigned int)(rt & 0x0000001F)) << 16;         // rt field
    machineCode = machineCode | mask;
    mask = ((unsigned int)(rs & 0x0000001F)) << 21;         // rs field
    machineCode = machineCode | mask;
    mask = ((unsigned int)(opcode & 0x0000003F)) << 26;     // opcode field
    machineCode = machineCode | mask;

    return machineCode;  // finally return a 32-bit machine code.
}

// This is the function to build J type instruction based on Instruction_storage
unsigned int buildJInstruction(unsigned char opcode,
                               int immediate){
    unsigned int machineCode = 0;
    unsigned int mask = 0;

    // J-type format: [opcode(6)][address(26)]
    // Address is 26 bits, shift left 2 bits for word alignment
    // left 2 bits and combined with PC+4 to form the jump target
    machineCode = (immediate & 0x03FFFFFF);                  // address field (26 bits)
    mask = ((unsigned int)(opcode & 0x0000003F)) << 26;     // opcode field
    machineCode = machineCode | mask;

    return machineCode;  // finally return a 32-bit machine code.
}

// This is the function to build R type instruction based on Instruction_storage
unsigned int buildRInstruction(unsigned char opcode,
                               unsigned char rs,
                               unsigned char rt,
                               unsigned char rd,
                               unsigned char shamt,
                               unsigned char function){
    unsigned int machineCode = 0;
    unsigned int mask = 0;

    // R-type format: [opcode(6)][rs(5)][rt(5)][rd(5)][shamt(5)][funct(6)]
    machineCode = function & 0x0000003F;                     // function field
    mask = ((unsigned int)(shamt & 0x0000001F)) << 6;       // shamt field
    machineCode = machineCode | mask;
    mask = ((unsigned int)(rd & 0x0000001F)) << 11;         // rd field
    machineCode = machineCode | mask;
    mask = ((unsigned int)(rt & 0x0000001F)) << 16;         // rt field
    machineCode = machineCode | mask;
    mask = ((unsigned int)(rs & 0x0000001F)) << 21;         // rs field
    machineCode = machineCode | mask;
    mask = ((unsigned int)(opcode & 0x0000003F)) << 26;     // opcode field
    machineCode = machineCode | mask;

    return machineCode;  // finally return a 32-bit machine code.
}

// This is the function to store the instructions.
void setupInstructionMemory(char* base_memory_address,
                            int codeOffset,  // this is the code index.
                            MIPS_Instruction *instructionStorage){
    int i = 0;
    unsigned int totalinstruction = 0;
    unsigned char opcode = 0;
    unsigned int machineCode = 0;

    do {
        // if la instruction.
        if (strcmp(instructionStorage[i].instruction, "la") == 0 ){
            // First, Answer this: what type of instruction is this? J, R or I instruction?
            // Answer: I instruction

            // Secondly, what kind of machine language data format should it be?
            // Answer: ooooooss sssttttt iiiiiiii iiiiiiii
            // needs: opcode, rs, rt and immediate number as address.

            // Thirdly, what is the opcode of this instruction?
            // Answer: 0b101111 (47 decimal) - Load Address pseudo-instruction

            opcode = OP_LA;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);
            write_dword(base_memory_address, codeOffset + i * 4, machineCode);
        }
        else if (strcmp(instructionStorage[i].instruction, "lb") == 0 ){// if lb instruction.
            // First, Answer this: what type of instruction is this? J, R or I instruction?
            // Answer: I instruction

            // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
            // Answer: ooooooss sssttttt iiiiiiii iiiiiiii
            // needs: opcode, rs (base address register), rt (destination register), immediate (offset)

            // Thirdly, what is the opcode of this instruction?
            // Answer: 0b100000 (32 decimal) - Load Byte

            opcode = OP_LB;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);
            write_dword(base_memory_address, codeOffset + i * 4, machineCode);
        }
        else if (strcmp(instructionStorage[i].instruction, "bge") == 0 ){// if bge instruction.
            // First, Answer this: what type of instruction is this? J, R or I instruction?
            // Answer: I instruction

            // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
            // Answer: ooooooss sssttttt iiiiiiii iiiiiiii
            // needs: opcode, rs (first register), rt (second register), immediate (branch offset)

            // Thirdly, what is the opcode of this instruction?
            // Answer: 0b000111 (7 decimal) - Branch if Greater or Equal

            opcode = OP_BGE;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);
            write_dword(base_memory_address, codeOffset + i * 4, machineCode);
        }
        else if (strcmp(instructionStorage[i].instruction, "lw") == 0 ){// if lw instruction.
            // First, Answer this: what type of instruction is this? J, R or I instruction?
            // Answer: I instruction

            // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
            // Answer: ooooooss sssttttt iiiiiiii iiiiiiii
            // needs: opcode, rs (base address register), rt (destination register), immediate (offset)

            // Thirdly, what is the opcode of this instruction?
            // Answer: 0b100011 (35 decimal) - Load Word

            opcode = OP_LW;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);
            write_dword(base_memory_address, codeOffset + i * 4, machineCode);
        }
        else if (strcmp(instructionStorage[i].instruction, "sw") == 0 ){// if sw instruction.
            // First, Answer this: what type of instruction is this? J, R or I instruction?
            // Answer: I instruction

            // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
            // Answer: ooooooss sssttttt iiiiiiii iiiiiiii
            // needs: opcode, rs (base address register), rt (source register), immediate (offset)

            // Thirdly, what is the opcode of this instruction?
            // Answer: 0b101011 (43 decimal) - Store Word

            opcode = OP_SW;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);
            write_dword(base_memory_address, codeOffset + i * 4, machineCode);
        }
        else if (strcmp(instructionStorage[i].instruction, "add") == 0 ){// if add instruction.
            // First, Answer this: what type of instruction is this? J, R or I instruction?
            // Answer: R instruction

            // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
            // Answer: ooooooss sssttttt dddddaaaa aaffffff
            // needs: opcode (0), rs (first source register), rt (second source register),
            //        rd (destination register), shamt (0 for add), function (add function code)

            // Thirdly, what is the opcode of this instruction?
            // Answer: 0b000000 (0 decimal) for R-type, with function code 0b100000 (32 decimal) for add

            opcode = OP_R_TYPE;
            machineCode = buildRInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].rd,
                                            0,                      // shamt = 0 for add
                                            FUNCT_ADD);             // function code for add
            write_dword(base_memory_address, codeOffset + i * 4, machineCode);
        }
        else if (strcmp(instructionStorage[i].instruction, "addi") == 0 ){// if addi instruction.
            // First, Answer this: what type of instruction is this? J, R or I instruction?
            // Answer: I instruction

            // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
            // Answer: ooooooss sssttttt iiiiiiii iiiiiiii
            // needs: opcode, rs (source register), rt (destination register), immediate (value to add)

            // Thirdly, what is the opcode of this instruction?
            // Answer: 0b001000 (8 decimal) - Add Immediate

            opcode = OP_ADDI;
            machineCode = buildIInstruction(opcode,
                                            instructionStorage[i].rs,
                                            instructionStorage[i].rt,
                                            instructionStorage[i].immediate);
            write_dword(base_memory_address, codeOffset + i * 4, machineCode);
        }
        else if (strcmp(instructionStorage[i].instruction, "j") == 0 ){// if j instruction.
            // First, Answer this: what type of instruction is this? J, R or I instruction?
            // Answer: J instruction

            // Secondly, what kind of machine language data format should it be and what registers/numbers are needed?
            // Answer: ooooooaa aaaaaaaa aaaaaaaa aaaaaaaa
            // needs: opcode, 26-bit address (target jump address)

            // Thirdly, what is the opcode of this instruction?
            // Answer: 0b000010 (2 decimal) - Jump

            opcode = OP_J;
            machineCode = buildJInstruction(opcode,
                                            instructionStorage[i].immediate);
            write_dword(base_memory_address, codeOffset + i * 4, machineCode);
        }
        else if (strcmp(instructionStorage[i].instruction, "syscall") == 0 || // if end of the code
                 (strcmp(instructionStorage[i].instruction, "END") == 0 ))
        {
            // No need to worry about this. This is the end of code indicator.
            break;  // exit here.
        }
        i++;
    } while (1);
    totalinstruction = i;
}

// load the code into the memory starts from offset.
void loadCodeToMem(char *mem){
    // Finish this function in Lab 7. This lab would
    //  1. Parse the MIPS ASM file with parse_MIPS(); (this is already done )
    //     in main function. Data is already stored in array Data_storage[];
    //     Code is already stored into array Instruction_storage[].
    //  2. Store the data section into memory space starts from offset 0x2000 (8K)
    //  2. Convert all the instructions into 32-bit binary code based on MIPS instruction format.
    //  3. Store the code into memory starts from offset 0x0000. (beginning of the memory space)
    unsigned int dataSection = DATASECTION;
    unsigned int instructionSection = CODESECTION;

    // Starting from point 2 above.
    // Step 1: refer to the code in memory.c. Write Data_storage into memory space starting from 0x2000.
    setupDataMemory(mem, dataSection, Data_storage, totalDataByte);

    // Step 2: Convert all the instructions stored in
    //         Instruction_storage[] to binary code and store them into the memory starts at 0x0000
    // Just need to compile to following instructions:
    //         la, add, lb, bge, lw, sw, addi, j
    setupInstructionMemory(mem, instructionSection, Instruction_storage);

    // Memory dump the information on the screen.
    puts("\n---- Code Section (Starting at 0x0000) ----\n");
    memory_dump(mem, CODESECTION, 256);
    puts("\n---- Data Section (Starting at 0x2000) ----\n");
    memory_dump(mem, DATASECTION, 256);
}
