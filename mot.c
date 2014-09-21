/**
 * Dan LaManna
 * dl954588
 * dlamanna@albany.edu
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "constants.h"
#include "struct_def.h"
#include "prototypes.h"

// Global to this file, the massive list of *ordered* opcodes,
// with the index corresponding to the integer value of the opcode.
static char *mot[NUM_OPCODES] = { "hlt",
                                  "add",
                                  "sub",
                                  "mul",
                                  "div",
                                  "mod",
                                  "move",
                                  "and",
                                  "or",
                                  "xor",
                                  "com",
                                  "sll",
                                  "srl",
                                  "sra",
                                  "jr",
                                  "rdr",
                                  "prr",
                                  "prh",
                                  "li",
                                  "addi",
                                  "subi",
                                  "muli",
                                  "divi",
                                  "modi",
                                  "lwb",
                                  "swb",
                                  "lwa",
                                  "swa",
                                  "j",
                                  "jal",
                                  "jeq",
                                  "jne",
                                  "jlt",
                                  "jle",
                                  "jgt",
                                  "jge" };

int get_opcode_by_mnemonic(char *mnemonic) {
  int i;

  for (i=0;i<NUM_OPCODES;i++)
    if (strcmp(mnemonic, mot[i]) == 0)
      return i;

  return -1;
}

char format_by_opcode(unsigned int opcode) {
  return (opcode <= MAX_R_FORMAT_OPCODE ? 'R' : ((opcode <= MAX_I_FORMAT_OPCODE) ? 'I' : 'J'));
}
