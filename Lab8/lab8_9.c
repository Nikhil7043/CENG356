/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
// Please finish the following functions for lab 8.
// Lab 8 will perform the following functions:
//   1. Fetch the code stored in memory
//   2. Decode the code and prepare for the execution of the code.
//   3. Setup the execution function for CPU.

// Lab 9 will perform the following functions:
//   4. Execute the code stored in the memory and print the results.

// Include header files for global definitions and lab8 function prototypes
#include "header.h"
#include "lab8header.h"

// Reference to external variables defined in other files
extern char *regNameTab[N_REG];     // Array of register names ($zero, $at, etc.)
extern unsigned int PCRegister;      // Program Counter - points to next instruction

/**
 * CPU - Main CPU control function that fetches, decodes, and executes instructions
 * @param mem - Pointer to simulated memory space
 *
 * This function implements the fetch-decode-execute cycle of a MIPS CPU:
 * 1. FETCH: Read instruction from memory at PC address
 * 2. DECODE: Determine instruction type and extract opcode
 * 3. EXECUTE: Perform the operation (Lab 9)
 */
void CPU(char *mem){
    unsigned int machineCode = 0;    // Stores the 32-bit instruction fetched from memory
    unsigned char opcode = 0;        // Stores the 6-bit opcode after decoding

    // Initialize Program Counter to the start of code section (0x0000)
    PCRegister = CODESECTION;        // CODESECTION is defined as 0x0000 in header.h

    // Main fetch-decode-execute loop (infinite loop until break)
    do{
        // Print current PC address in hexadecimal for debugging
        printf("\nPC:%x\n", PCRegister);

        // STEP 1: FETCH - Read 32-bit instruction from memory at current PC
        machineCode = CPU_fetchCode((char*)mem, PCRegister);

        // Check for end of program - machineCode 0 indicates no more instructions
        if (machineCode == 0)
            break;                   // Exit the infinite loop when program ends

        // STEP 2: UPDATE PC - Move to next instruction (each instruction is 4 bytes)
        PCRegister += 4;

        // STEP 3: DECODE - Extract opcode from the machine code instruction
        opcode = CPU_Decode(machineCode);

        // Print the decoded opcode in 2-digit hexadecimal format
        printf("Decoded Opcode is: %02X. \n", opcode);

        // STEP 4: EXECUTE - Lab 9 will implement this
        // Currently commented out - will be enabled when CPU_Decode is complete
        // CPU_Execution(opcode, machineCode, (char*)mem);

    } while (1);                     // Infinite loop - breaks when machineCode == 0

    // After program execution completes, display final CPU state
    printRegisterFiles();            // Print all 32 MIPS registers with their values
    printDataMemoryDump((char*)mem); // Print data memory contents (first 256 bytes)
}

/**
 * CPU_fetchCode - Reads a 32-bit instruction from memory
 * @param mem - Pointer to memory space
 * @param codeOffset - Address offset where instruction is stored
 * @return 32-bit machine code instruction
 *
 * MIPS instructions are 4 bytes (32 bits) long. This function reads 4 consecutive
 * bytes from memory and combines them into a single 32-bit integer.
 *
 * IMPORTANT: Uses little-endian byte ordering where Least Significant Byte (LSB)
 * is stored at the lowest memory address. This matches the write_dword() function
 * in memory.c.
 */
unsigned int CPU_fetchCode(char *mem, int codeOffset){
    unsigned int instruction = 0;    // Will hold the complete 32-bit instruction

    // Read byte at offset+0 (LSB) and place in bits 0-7
    instruction |= (unsigned char)mem[codeOffset + 0];

    // Read byte at offset+1 and shift left 8 bits -> bits 8-15
    instruction |= (unsigned char)mem[codeOffset + 1] << 8;

    // Read byte at offset+2 and shift left 16 bits -> bits 16-23
    instruction |= (unsigned char)mem[codeOffset + 2] << 16;

    // Read byte at offset+3 (MSB) and shift left 24 bits -> bits 24-31
    instruction |= (unsigned char)mem[codeOffset + 3] << 24;

    return instruction;              // Return the complete 32-bit instruction
}

/**
 * CPU_Decode - Extracts the opcode/function code from a machine code instruction
 * @param machineCode - 32-bit MIPS instruction
 * @return 6-bit opcode (for I/J-type) or function code (for R-type)
 *
 * MIPS instruction formats:
 * - R-type: opcode (bits 31-26) = 0, uses funct field (bits 5-0)
 * - I-type: opcode (bits 31-26) determines operation
 * - J-type: opcode (bits 31-26) determines jump operation
 *
 * For Lab 8, we return the 6-bit opcode from bits 31-26.
 * For Lab 9, R-type instructions (opcode=0) will need to return the funct field.
 */
unsigned char CPU_Decode(unsigned int machineCode){
    // Extract bits 31-26 by shifting right 26 bits and masking with 0x3F (binary 111111)
    // This isolates the 6-bit opcode field
    unsigned char opcode = (machineCode >> 26) & 0x3F;

    // For Lab 8, just return the opcode
    // In Lab 9, you'll use this to execute different instructions
    return opcode;
}

/**
 * CPU_Execution - Executes the decoded MIPS instruction (LAB 9)
 * @param opcode - 6-bit opcode or function code identifying the instruction
 * @param machineCode - Complete 32-bit instruction containing register and immediate fields
 * @param mem - Pointer to memory space for load/store operations
 *
 * This function implements the actual execution of MIPS instructions:
 * - Extracts register numbers and immediate values from machineCode
 * - Performs the operation (add, load, store, branch, jump, etc.)
 * - Updates register file and memory as needed
 * - Manages Program Counter (especially for branches and jumps)
 */
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem){
    unsigned char rt = 0;            // Target register (I-type: destination)
    unsigned char rs = 0;            // Source register 1
    unsigned char rd = 0;            // Destination register (R-type)
    int immediate = 0;               // 16-bit immediate value (sign-extended to 32-bit)

    // Execute different operations based on opcode value
    switch (opcode)
    {
        // la (Load Address) - Pseudo-instruction, opcode = 0b101111 (0x2F)
        case 0b101111:
            // Extract rt field from bits 20-16 (mask 0x001F0000, shift 16)
            rt = (machineCode & 0x001F0000) >> 16;
            // Extract immediate address from bits 15-0 (last 16 bits)
            regFile[rt] = machineCode & 0x0000FFFF;
            // Update PC to next instruction (normal sequential execution)
            PCRegister += 4;
            if (DEBUG_CODE){
                printf("Code Executed: %08X\n", machineCode);
                printf("****** PC Register is %08X ******\n", PCRegister);
            }
            break;

        // lb (Load Byte) - Load a byte from memory with sign extension, opcode = 0b100000 (0x20)
        case 0b100000:
            // Extract rt (destination register) from bits 20-16
            rt = (machineCode & 0x001F0000) >> 16;
            // Extract rs (base address register) from bits 25-21
            rs = (machineCode & 0x03E00000) >> 21;
            // Extract immediate offset from bits 15-0
            immediate = machineCode & 0x0000FFFF;
            // Sign extend the 16-bit immediate to 32 bits if bit 15 is set
            if (immediate & 0x8000) immediate |= 0xFFFF0000;
            // Read byte from memory (base address + offset), sign extend to 32 bits, store in rt
            regFile[rt] = (signed char)read_byte(mem, regFile[rs] + immediate);
            PCRegister += 4;         // Move to next instruction
            break;

        // bge (Branch if Greater or Equal) - Conditional branch, opcode = 0b000111 (0x07)
        case 0b000111:
            // Extract rs (first register to compare) from bits 25-21
            rs = (machineCode & 0x03E00000) >> 21;
            // Extract rt (second register to compare) from bits 20-16
            rt = (machineCode & 0x001F0000) >> 16;
            // Extract branch offset (number of instructions to branch)
            immediate = machineCode & 0x0000FFFF;
            // Sign extend the offset (branch target is relative to PC+4)
            if (immediate & 0x8000) immediate |= 0xFFFF0000;
            // Check if rs >= rt (branch condition)
            if (regFile[rs] >= regFile[rt]) {
                // Branch taken: PC += (offset * 4) because instructions are 4 bytes each
                PCRegister += (immediate << 2);
            } else {
                // Branch not taken: continue to next instruction
                PCRegister += 4;
            }
            break;

        // lw (Load Word) - Load 32-bit word from memory, opcode = 0b100011 (0x23)
        case 0b100011:
            // Extract rt (destination register) from bits 20-16
            rt = (machineCode & 0x001F0000) >> 16;
            // Extract rs (base address register) from bits 25-21
            rs = (machineCode & 0x03E00000) >> 21;
            // Extract immediate offset from bits 15-0
            immediate = machineCode & 0x0000FFFF;
            // Sign extend the offset
            if (immediate & 0x8000) immediate |= 0xFFFF0000;
            // Read 32-bit word from memory and store in rt
            regFile[rt] = read_dword(mem, regFile[rs] + immediate);
            PCRegister += 4;         // Move to next instruction
            break;

        // sw (Store Word) - Store 32-bit word to memory, opcode = 0b101011 (0x2B)
        case 0b101011:
            // Extract rt (source register containing data to store) from bits 20-16
            rt = (machineCode & 0x001F0000) >> 16;
            // Extract rs (base address register) from bits 25-21
            rs = (machineCode & 0x03E00000) >> 21;
            // Extract immediate offset from bits 15-0
            immediate = machineCode & 0x0000FFFF;
            // Sign extend the offset
            if (immediate & 0x8000) immediate |= 0xFFFF0000;
            // Write 32-bit word from rt to memory at address (rs + offset)
            write_dword(mem, regFile[rs] + immediate, regFile[rt]);
            PCRegister += 4;         // Move to next instruction
            break;

        // addi (Add Immediate) - Add signed immediate to register, opcode = 0b001000 (0x08)
        case 0b001000:
            // Extract rt (destination register) from bits 20-16
            rt = (machineCode & 0x001F0000) >> 16;
            // Extract rs (source register) from bits 25-21
            rs = (machineCode & 0x03E00000) >> 21;
            // Extract immediate value from bits 15-0
            immediate = machineCode & 0x0000FFFF;
            // Sign extend the immediate (MIPS addi uses signed immediate)
            if (immediate & 0x8000) immediate |= 0xFFFF0000;
            // Perform addition: rt = rs + sign-extended immediate
            regFile[rt] = regFile[rs] + immediate;
            PCRegister += 4;         // Move to next instruction
            break;

        // j (Jump) - Unconditional jump to target address, opcode = 0b000010 (0x02)
        case 0b000010:
            // Extract 26-bit target address from bits 25-0
            immediate = machineCode & 0x03FFFFFF;
            // Jump target = (PC+4 upper 4 bits) concatenated with (address << 2)
            // Shift left 2 because instructions are word-aligned (4 bytes each)
            PCRegister = (PCRegister & 0xF0000000) | (immediate << 2);
            break;

        // R-type instructions - All have opcode = 0b000000 (0x00)
        // Function code in bits 5-0 determines which operation to perform
        case 0b000000:
            {
                // Extract 6-bit function code from bits 5-0
                unsigned int funct = machineCode & 0x0000003F;
                // Extract rd (destination register) from bits 15-11
                rd = (machineCode & 0x0000F800) >> 11;
                // Extract rs (first source register) from bits 25-21
                rs = (machineCode & 0x03E00000) >> 21;
                // Extract rt (second source register) from bits 20-16
                rt = (machineCode & 0x001F0000) >> 16;

                // Execute based on function code
                switch(funct) {
                    case 0b100000:   // add instruction (function code 0x20)
                        // Perform addition: rd = rs + rt
                        regFile[rd] = regFile[rs] + regFile[rt];
                        PCRegister += 4;  // Move to next instruction
                        break;
                    default:
                        // Unknown R-type instruction - print error and continue
                        printf("Unknown R-type function: %02X\n", funct);
                        PCRegister += 4;
                        break;
                }
            }
            break;

        // Default case - Should never execute if all instructions are implemented
        // If reached, there's an error in the instruction set or decoding
        default:
            printf("Wrong instruction! You need to fix this instruction %02X %08X\n", opcode,  machineCode);
            system("PAUSE");         // Pause to let user see the error
            exit(3);                 // Exit program with error code 3
            break;
    }
}

/**
 * printRegisterFiles - Displays the contents of all MIPS registers
 *
 * Prints all 32 general-purpose MIPS registers ($zero through $ra)
 * along with their values in both hexadecimal and decimal formats.
 * Also prints the LO and HI registers used for multiplication/division.
 *
 * Format: 4 registers per line for easy reading
 */
void printRegisterFiles(){
    // Print header for register dump section
    printf("\n============================================================\n");
    printf("                   REGISTER FILE CONTENTS\n");
    printf("============================================================\n");

    // Loop through all 32 general-purpose MIPS registers (indices 0-31)
    for (int i = 0; i < 32; i++) {
        // Print register name and value (hex and decimal)
        // Check if register name table exists and has valid entry
        if (regNameTab != NULL && regNameTab[i] != NULL) {
            // Use the predefined register name from regNameTab
            printf("%4s = 0x%08X (%10d)", regNameTab[i], regFile[i], regFile[i]);
        } else {
            // Fallback: use $n format if name not available
            printf("$%2d   = 0x%08X (%10d)", i, regFile[i], regFile[i]);
        }

        // Format output: 4 registers per line for better readability
        if ((i + 1) % 4 == 0) {
            printf("\n");            // End of line after 4 registers
        } else {
            printf("    ");          // Space between registers on same line
        }
    }

    // Print LO and HI registers (special registers for multiply/divide)
    printf("\n------------------------------------------------------------\n");
    printf("%4s = 0x%08X (%10d)    ", "lo", regFile[REG_LO], regFile[REG_LO]);
    printf("%4s = 0x%08X (%10d)\n", "hi", regFile[REG_HI], regFile[REG_HI]);
    printf("============================================================\n");
}

/**
 * printDataMemoryDump - Displays the contents of the data memory section
 * @param mem - Pointer to the simulated memory space
 *
 * This function calls memory_dump() to display the first 256 bytes
 * of the data segment (starting at DATASECTION address).
 *
 * The data section contains initialized global variables and static data
 * from the original MIPS assembly program.
 */
void printDataMemoryDump(char *mem){
    // Print header for data memory dump section
    printf("\n============================================================\n");
    printf("              DATA MEMORY DUMP (first 256 bytes)\n");
    printf("============================================================\n");
    printf("Data section at 0x%04X:\n", DATASECTION);  // Show starting address

    // Call memory_dump to display memory contents
    // Parameters: memory pointer, starting offset, number of bytes to dump
    // DATASECTION is defined as 0x2000 in header.h (8KB into memory)
    // Dumping 256 bytes shows the initial data segment content
    memory_dump(mem, DATASECTION, 256);
}
