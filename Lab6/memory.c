/*
 ============================================================================
 Name        : Lab6.c
 Author      : Austin Tian
 Revised by  : Nikhil
 Version     :
 Copyright   : Copyright 2023
 Description : Lab 6 in C, ANSI-C Style
 ============================================================================
 */

#include "header.h"   // Include header file (contains macros, libraries, constants)

#include "header.h"

// Menu for the testing.
char *menu =    "\n" \
                " ***********Please select the following options**********************\n" \
                " *    This is the memory operation menu (Lab 6)                     *\n" \
                " ********************************************************************\n" \
                " *    1. Write a double-word (32-bit) to the memory                 *\n"  \
                " ********************************************************************\n" \
                " *    2. Read a byte (8-bit) data from the memory                   *\n" \
                " *    3. Read a double-word (32-bit) data from the memory           *\n" \
                " ********************************************************************\n" \
                " *    4. Generate a memory dump from any memory location            *\n" \
                " ********************************************************************\n" \
                " *    e. To Exit, Type 'e'  or 'E'                                  *\n" \
                " ********************************************************************\n";

//---------------------------------------------------------------
// Generate a random number between 0x00 and 0xFF.
unsigned char rand_generator()   // Function to generate random byte
{
    return rand() % 256;  // Returns random value from 0 to 255 (1 byte range)
}

//---------------------------------------------------------------
char *init_memory()   // Function to allocate and initialize memory
{
    char *mem = malloc(MEM_SIZE);  // Allocate MEM_SIZE (1MB) bytes

    if (!mem) {   // Check if allocation failed
        printf("Memory allocation failed!\n");  // Print error message
        exit(EXIT_FAILURE);  // Exit program if allocation fails
    }

    // Fill memory with random bytes
    for (int i = 0; i < MEM_SIZE; i++) {  // Loop through entire memory
        mem[i] = rand_generator();  // Assign random byte to each location
    }

    return mem;  // Return base address of allocated memory
}

//---------------------------------------------------------------
void write_dword(const char *base_address, const int offset, const unsigned int dword_data)
// Function to write 32-bit data at (base_address + offset)
{
    unsigned char *ptr = (unsigned char *)(base_address + offset); // Calculate target address

    // Store in little-endian: LSB first
    ptr[0] = dword_data & 0xFF;          // Store least significant byte
    ptr[1] = (dword_data >> 8) & 0xFF;   // Store second byte
    ptr[2] = (dword_data >> 16) & 0xFF;  // Store third byte
    ptr[3] = (dword_data >> 24) & 0xFF;  // Store most significant byte

    printf("Double-word 0x%08X written to offset 0x%X\n", dword_data, offset); // Confirmation message
}

//---------------------------------------------------------------
unsigned char read_byte(const char *base_address, const int offset)
// Function to read a single byte from memory
{
    unsigned char value = *(unsigned char *)(base_address + offset); // Read byte from memory

    printf("Byte at offset 0x%X: 0x%02X\n", offset, value); // Print value in HEX

    return value;  // Return the byte value
}

//---------------------------------------------------------------
unsigned int read_dword(const char *base_address, const int offset)
// Function to read a 32-bit value from memory
{
    unsigned char *ptr = (unsigned char *)(base_address + offset); // Get pointer to location

    // Combine 4 bytes into one 32-bit integer (little-endian)
    unsigned int value = ptr[0] | (ptr[1] << 8) | (ptr[2] << 16) | (ptr[3] << 24);

    printf("Double-word at offset 0x%X: 0x%08X\n", offset, value); // Print result

    return value;  // Return the 32-bit value
}

//---------------------------------------------------------------
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize)
// Function to display memory contents in HEX and ASCII
{
    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE) // Validate dump size
        dumpsize = MIN_DUMP_SIZE;  // Set to minimum if invalid

    const unsigned char *ptr = (unsigned char *)(base_address + offset); // Starting address

    for (unsigned int i = 0; i < dumpsize; i += DUMP_LINE) { // Loop line by line

        // Print memory address
        printf("%08X  ", offset + i); // Print offset address in HEX

        // Print hex bytes
        for (unsigned int j = 0; j < DUMP_LINE; j++) { // Loop through each byte in line
            if (i + j < dumpsize) // Check bounds
                printf("%02X ", ptr[i + j]); // Print byte in HEX
            else
                printf("   "); // Print space if out of bounds
        }

        // Print ASCII representation
        printf(" |"); // Start ASCII section

        for (unsigned int j = 0; j < DUMP_LINE; j++) { // Loop again for ASCII
            if (i + j < dumpsize) { // Check bounds
                unsigned char c = ptr[i + j]; // Get byte

                // Print character if printable, else '.'
                printf("%c", (c >= 0x20 && c <= 0x7E) ? c : '.');
            } else {
                printf(" "); // Padding
            }
        }

        printf("|\n"); // End ASCII section and move to next line
    }
}

//---------------------------------------------------------------
void free_memory(char *base_address)  // Function to free allocated memory
{
    free(base_address);  // Release memory to avoid memory leaks
}

//---------------------------------------------------------------
void setup_memory()   // Main control function for memory system
{
    char *mem = init_memory();  // Initialize memory and get base address

    char options = 0;  // Variable to store user input option
    unsigned int offset, dumpsize; // Variables for memory offset and dump size
    char tempchar;  // Temporary variable for input handling
    unsigned int dword_data; // Variable for 32-bit data

    do {  // Infinite loop until user exits
        if (options != 0x0a)  // Skip newline input
        {
            puts(menu); // Display menu
            printf("\nThe base address of your memory is: %I64Xh (HEX)\n", (long long unsigned int)(mem)); // Show base address
            puts("Please make a selection:"); // Prompt user
        }

        options = getchar();  // Read user input

        switch (options) {

            case '1':  // Write double word
                puts("Please input your memory's offset address (in HEX):"); // Ask for offset
                scanf("%x", &offset); // Read offset
                puts("Please input your DOUBLE WORD data to be written (in HEX):"); // Ask for data
                scanf("%x", &dword_data); // Read data
                write_dword(mem, offset, dword_data); // Call write function
                continue;

            case '2':  // Read byte
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                read_byte(mem, offset); // Call read byte
                continue;

            case '3':  // Read double word
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset);
                read_dword(mem, offset); // Call read dword
                continue;

            case '4':  // Memory dump
                puts("Please input your memory's offset address (in HEX, multiple of 0x10h):");
                scanf("%x", &offset);
                puts("Please input the size of the memory to be dumped (256–1024):");
                scanf("%d", &dumpsize);
                memory_dump(mem, offset, dumpsize); // Call dump function
                continue;

            case 'e':  // Exit option
            case 'E':
                puts("Code finished, press any key to exit"); // Exit message
                free_memory(mem); // Free allocated memory

                while ((tempchar = getchar()) != '\n' && tempchar != EOF); // Clear buffer
                tempchar = getchar(); // Wait for key press
                return; // Exit function

            default:
                continue; // Ignore invalid input
        }

    } while (1); // Loop forever until exit
}
