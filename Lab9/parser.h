/**
 * @file mips_asm.h
 *
 * Definitions
 *
 * @author Yooseong Kim, Aug., 2010
 */
#ifndef MIPS_ASM_H
#define MIPS_ASM_H
#include <string.h>

int n_Label = 0;
extern char *regNameTab[N_REG];
char *strlwr (char *);  // has to have this prototype to remove the warnings.
int findLabel(char *label);    // Find the offset address from the labels.
int getRegNum(char *regName);
int addLabel(char *label, int segType, int offset);
int addData(char *token, char *buffer, int offset);
void addInst(char *inst, char *operands, int offset);

#endif
