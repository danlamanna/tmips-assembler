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

extern unsigned int instructions[];
extern unsigned int lc;

extern int ln;
extern int actual_ln;


/**
 * Gets the number of arguments in a given string.
 **/
int num_args(char *args) {
  int i = 0;
  char *tmp;

  if (!args)
    return 0;

  tmp = strdup(args);
  tmp = strtok(tmp, ",");

  while (strtok(NULL, ",") != NULL)
    i++;

  return i+1;
}

/**
 * Parses a data segment instruction.
 **/
int parse_ds_instruction(char *instruction) {
  char *directive, *args, *arg;
  int i = 0, value = 0, words = 0;

  // Separate the directive and the arguments list
  directive = strtok(instruction, " \t");
  args = strtok(NULL, " \t");

  if (strcmp(directive, WORD_DIRECTIVE) == 0) {
    value = atoi(strtok(args, ":"));
    arg = strtok(NULL, ":");
    words = atoi(arg);

    // Initialize words elements in instructions to be value
    for (i=words;i>0;i--) {
      instructions[lc-i] = value;
    }
  }


  return 1;
}

/**
 * Parses a text segment instruction, determines whether or not the opcode is illegal,
 * determines what type of instruction it is, then passes it off the right assembling
 * function.
 **/
int parse_ts_instruction(char *instruction) {
  char *opcode, *args;
  int opcode_val, nargs;

  instruction = strtok(instruction, " \t\n");

  opcode = strdup(instruction);

  opcode_val = get_opcode_by_mnemonic(opcode);

  if (opcode_val == -1) {
    add_error(actual_ln, 'I', "Illegal opcode.");
  }

  args = strtok(NULL, " \t\n");

  nargs = num_args(args);

  switch (format_by_opcode((unsigned int) opcode_val)) {
    case 'R':
      assemble_r_instruction((unsigned int) opcode_val, args);
      break;

    case 'I':
      assemble_i_instruction((unsigned int) opcode_val, args, nargs);
      break;

    case 'J':
      assemble_j_instruction((unsigned int) opcode_val, args, nargs);
      break;
  }

  return 1;
}

/**
 * Tells if an argument is a symbol (starts with a letter, has the right format).
 **/
int is_symbol_arg(char *arg) {
  char *tmp = strdup(arg);
  int i = 0;

  // first char of a label has to be letter
  if (!isalpha(tmp[i]))
    return 0;

  for (i=0;i<strlen(tmp);i++)
    if (!isalnum(tmp[i]) && tmp[i] != '_')
      return 0;

  return 1;
}

/**
 * Calculates the value of an argument, if it's a register such as $5 it
 * returns 5, if it's an immediate value it returns it in integer form.
 **/
int arg_value(char *arg) {
  int i, is_digit=1;

  // just a register, return the value without the dollar sign
  if (arg[0] == '$') {
    return atoi(++arg);
  }

  for (i=0;i<strlen(arg);i++) {
    // negative sign nonsense...
    if (i == 0 && arg[0] == '-')
      continue;

    if (!isdigit(arg[i])) {
      is_digit = 0;
      break;
    }
  }

  // immediate value
  if (is_digit)
    return atoi(arg);

  return 0;
}
