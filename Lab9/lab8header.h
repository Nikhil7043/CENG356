/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   lab8header.h
 *
 * Lab 8/9 Header File - Contains function prototypes for CPU operations
 *
 * This header declares all functions needed for the MIPS CPU simulator:
 * - Instruction fetch from memory
 * - Instruction decode
 * - Instruction execution
 * - Register file printing
 * - Memory dump functionality
 */

#ifndef LAB8HEADER_H
#define LAB8HEADER_H

// Function prototypes for Lab 8 and Lab 9

/**
 * CPU_fetchCode - Reads a 32-bit instruction from memory at specified offset
 * @param mem - Pointer to the simulated memory space
 * @param codeOffset - Byte offset where instruction is located
 * @return 32-bit machine code instruction
 *
 * This function reads 4 consecutive bytes from memory and combines them
 * into a single 32-bit integer using little-endian byte ordering.
 */
unsigned int CPU_fetchCode(char *mem, int codeOffset);

/**
 * CPU_Decode - Extracts the opcode/function code from a machine code instruction
 * @param machineCode - 32-bit MIPS instruction
 * @return 6-bit opcode (for I/J-type) or function code (for R-type)
 *
 * For I-type and J-type instructions, returns bits 31-26 (opcode).
 * For R-type instructions (opcode=0), should return bits 5-0 (funct).
 */
unsigned char CPU_Decode(unsigned int machineCode);

/**
 * CPU_Execution - Executes a decoded MIPS instruction
 * @param opcode - 6-bit opcode identifying the instruction type
 * @param machineCode - Complete 32-bit instruction with register/immediate fields
 * @param mem - Pointer to memory for load/store operations
 *
 * Implements the execution phase of the fetch-decode-execute cycle.
 * Handles all MIPS instructions including R-type, I-type, and J-type.
 */
void CPU_Execution(unsigned char opcode, unsigned int machineCode, char *mem);

/**
 * printRegisterFiles - Displays the contents of all MIPS registers
 *
 * Prints all 32 general-purpose registers ($zero through $ra)
 * plus the LO and HI special registers. Output includes both
 * hexadecimal and decimal representations of register values.
 */
void printRegisterFiles();

/**
 * printDataMemoryDump - Displays data memory contents
 * @param mem - Pointer to the simulated memory space
 *
 * Calls memory_dump() to display the first 256 bytes of the
 * data segment, showing initialized variables from the MIPS program.
 */
void printDataMemoryDump(char *mem);

#endif /* LAB8HEADER_H */
