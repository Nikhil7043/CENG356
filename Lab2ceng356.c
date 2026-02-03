/*
 ============================================================================
 Name        : Lab2ceng356.c                // File name for this lab
 Author      : Austin Tian                 // Original author information
 Version     :                            // Version not specified
 Copyright   : Copyright 2020            // Copyright notice
 Student name: Nikhil                   // Name of the Student
 Student ID  : n01617043               // Humber ID
 Description : Signed number and      // Description of what the program does:
               unsigned numbers,     // works with signed and floating-point
               Ansi-style           // numbers in standard C
 ============================================================================
 */

#include <stdio.h>                    // For input/output functions like printf, scanf, puts
#include <stdlib.h>                   // For general utilities like EXIT_SUCCESS
#include <string.h>                   // For string functions like strlen
#include <math.h>                     // For math functions like powf


#define SIZE 32                       // Maximum size of the binary number is 32 bits
#define number1 "11000001010010000000000000000000"  // Test pattern for -12.5 (IEEE 754)
#define number2 "01000001010101000000000000000000"  // Test pattern for 13.25 (IEEE 754)


void convert_binary_to_signed(const char *binary);  // Function prototype for 8-bit signed conversion
void convert_binary_to_float(const char *binary);   // Function prototype for 32-bit float conversion


char *menu =
    "\n"                                             // Blank line for readability
    "\n"                                             // Another blank line
    "===================================================================\n"  // Menu border line
    "************Please select the following options********************\n"  // Menu title
    " *    1. Binary number to signed decimal number conversion.(Lab 2) *\n" // Option 1 description
    " *    2. Binary number to Floating number conversion (Lab 2)       *\n" // Option 2 description
    " *******************************************************************\n"  // Menu border line
    " *    e. To Exit, Type 'e'                                         *\n" // Exit option
    " *******************************************************************\n"; // Bottom border


int main(void) {                                   // Program entry point
    char options;                                  // Stores the user’s menu choice
    char inputs[33] = {0};                         // Buffer for binary input (32 bits + null terminator)


    do {                                           // Main loop to keep asking for options
        puts(menu);                                // Print the menu to the screen
        fflush(stdin);                             // Clear input buffer (non-standard, from template)
        options = getchar();                       // Read one character for the menu option


        switch (options) {                         // Decide what to do based on the chosen option
            case '1':                              // Option 1: signed 8-bit integer conversion
                puts("Please input your 8-bit BINARY number, I will convert it to signed decimal:");
                                                    // Prompt the user for an 8-bit binary string
                scanf("%32s", inputs);             // Read up to 32 characters into inputs
                convert_binary_to_signed(inputs);  // Convert the binary string to a signed decimal value
                continue;                          // Go back to the menu loop

            case '2':                              // Option 2: 32-bit floating-point conversion
                puts("Please input your 32-bit floating point number in binary, I will convert it to decimal:");
                                                    // Prompt the user for a 32-bit binary string
                scanf("%32s", inputs);             // Read up to 32 characters into inputs

                // Example test usage (commented out):
                // strcpy(inputs, number1);        // Use predefined bits for -12.5
                // strcpy(inputs, number2);        // Use predefined bits for 13.25

                convert_binary_to_float(inputs);   // Convert the binary string to a float value
                continue;                          // Go back to the menu loop


            case 'e':                              // Option 'e': exit the program
                puts("Code finished, exit now");   // Inform the user that the program is ending
                return EXIT_SUCCESS;               // Exit main with success status

            default:                               // Any other input that is not 1, 2, or e
                puts("Not a valid entry, exit now"); // Print an error message
                continue;                          // Loop again (template exits on default)
        }
    } while (1);                                   // Infinite loop (broken only by 'e' return)
}


/* Convert 8-bit two's complement binary string to signed decimal */
void convert_binary_to_signed(const char *binary) {
    int value = 0;                                 // Accumulator for the integer value
    int i;                                         // Loop counter

    if (strlen(binary) != 8) {                     // Check that we received exactly 8 bits
        printf("Error: need 8 bits.\n");           // Print error if length is not 8
        return;                                    // Stop the function
    }

    for (i = 0; i < 8; i++) {                      // Loop through each of the 8 characters
        value = value * 2 + (binary[i] - '0');     // Shift left and add current bit (0 or 1)
    }

    if (binary[0] == '1') {                        // If the most significant bit is 1, number is negative
        value -= 256;                              // Apply two's complement adjustment for 8 bits
    }

    printf("Signed decimal value: %d\n", value);   // Print the final signed decimal result
}


/* Convert a 32-bit IEEE 754 single-precision binary string to float*/
void convert_binary_to_float(const char *binary) {
    unsigned int bits = 0;                         // Will hold the 32-bit pattern as an unsigned int
    unsigned int sign, exponent, mantissa;         // Fields extracted from the 32-bit value
    float frac, value;                             // Fraction part and final float value
    int i, e;                                      // Loop index and exponent as signed int

    if (strlen(binary) != 32) {                    // Check that we received exactly 32 bits
        printf("Error: need 32 bits.\n");          // Print error if length is not 32
        return;                                    // Stop the function
    }

    for (i = 0; i < 32; i++) {                     // Loop through each of the 32 characters
        bits = (bits << 1) | (binary[i] - '0');    // Shift left and add current bit to build the 32-bit value
    }

    sign     = (bits >> 31) & 1;                   // Extract the sign bit (bit 31)
    exponent = (bits >> 23) & 0xFF;                // Extract the 8-bit exponent (bits 30–23)
    mantissa = bits & 0x7FFFFF;                    // Extract the 23-bit mantissa (bits 22–0)

    frac = 1.0f + (float)mantissa / (float)(1 << 23);
                                                  // Compute mantissa as 1 + fraction/2^23 (normalized IEEE 754)
    e = (int)exponent - 127;                       // Convert biased exponent to signed exponent (bias = 127)
    value = (sign ? -1.0f : 1.0f) * frac * powf(2.0f, (float)e);
                                                  // Apply sign and 2^e to get the actual float value

    printf("Floating-point value: %f\n", value);   // Print the final floating-point result
}
