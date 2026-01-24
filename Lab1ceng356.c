// Name: Nikhil                                      // Student name
// Student ID: n01617043                             // Student ID

#include <stdio.h>                                  // Include standard input/output library

// Definition of Account structure                  // Comment explaining struct
struct Account {                                   // Start of structure
    unsigned int account_number;                   // Stores the account number
    char account_type;                             // Stores the account type
    char *last_name;                               // Pointer to last name string
    char *first_name;                              // Pointer to first name string
    float balance;                                 // Stores account balance
    char reserved[6];                              // Reserved memory bytes
};                                                  // End of structure definition

int main() {                                       // Main function starts

    char ch;                                       // Declare char variable
    short sh;                                      // Declare short variable
    int i;                                         // Declare int variable
    long l;                                        // Declare long variable
    float f;                                       // Declare float variable
    double d;                                      // Declare double variable

    int *pInt;                                     // Declare pointer to int
    long *pLong;                                   // Declare pointer to long
    float *pFloat;                                 // Declare pointer to float
    double *pDouble;                               // Declare pointer to double

    struct Account account;                        // Declare struct Account variable
    struct Account *pAccount;                      // Declare pointer to struct Account

    printf("Size of char: %zu bytes\n", sizeof(ch));       // Print size of char
    printf("Size of short: %zu bytes\n", sizeof(sh));      // Print size of short
    printf("Size of int: %zu bytes\n", sizeof(i));         // Print size of int
    printf("Size of long: %zu bytes\n", sizeof(l));        // Print size of long
    printf("Size of float: %zu bytes\n", sizeof(f));       // Print size of float
    printf("Size of double: %zu bytes\n", sizeof(d));      // Print size of double

    printf("Size of int pointer: %zu bytes\n", sizeof(pInt));       // Print size of int pointer
    printf("Size of long pointer: %zu bytes\n", sizeof(pLong));     // Print size of long pointer
    printf("Size of float pointer: %zu bytes\n", sizeof(pFloat));   // Print size of float pointer
    printf("Size of double pointer: %zu bytes\n", sizeof(pDouble)); // Print size of double pointer

    printf("Size of struct Account: %zu bytes\n", sizeof(account)); // Print size of struct Account
    printf("Size of struct Account pointer: %zu bytes\n", sizeof(pAccount)); // Print size of struct pointer

    return 0;                                       // End program successfully
}                                                   // End of main function
