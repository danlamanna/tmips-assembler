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

/**
 * Generates the correct parameters for an instruction of type 'R',
 * then passes it off to r_instruction which does the actual bitmasking.
 **/
void assemble_r_instruction(unsigned int opcode, char *args) {
  int rt = 0, rs1 = 0, rs2 = 0, sa = 0, arg_val = 0, i = 0;
  short sa_flag = 0;
  char *arg;
  symbol *tmp;

  // These 3 commands use the shift amount value
  if (opcode == get_opcode_by_mnemonic("sll") ||
      opcode == get_opcode_by_mnemonic("srl") ||
      opcode == get_opcode_by_mnemonic("sra"))
    sa_flag = 1;

  arg = strtok(args, ",");

  while (arg != NULL) {

    // Either set the arg_value to the symbols value (if it is a symbol),
    // Add an error if the symbol is undefined,
    // or just get the arg_value (immediate or register)
    if (is_symbol_arg(arg) && (tmp = has_symbol(arg))) {
      arg_val = tmp->lc;
    } else if (is_symbol_arg(arg)) {
      add_error(ln, 'U', arg);
    } else {
      arg_val = arg_value(arg);
    }

    if (i == 0)
      rt = arg_val;
    else if (i == 1)
      rs1 = arg_val;
    else if (i == TWO_ARGS && sa_flag == 1)
      sa = arg_val;
    else if (i == TWO_ARGS)
      rs2 = arg_val;

    arg = strtok(NULL, ",");
    i++;
  }

  // Assemble..
  r_instruction(ln, opcode, rs1, rs2, rt, sa);
}

/**
 * Generates the correct parameters for an instruction of type 'I',
 * then passes it off to i_instruction which does the actual bitmasking.
 **/
void assemble_i_instruction(unsigned int opcode, char *args, int num_args) {
  int rt = 0, rs = 0, imm_operand = 0,  i = 0;
  int arg_val = 0;
  char *arg, *tmparg = NULL;
  symbol *tmp;

  arg = strtok(args, ",");

  if (num_args == THREE_ARGS) {
    while (arg != NULL) {
      if (is_symbol_arg(arg) && (tmp = has_symbol(arg))) {
        arg_val = tmp->lc;
      } else if (is_symbol_arg(arg)) {
        add_error(ln, 'U', arg);
      } else {
        arg_val = arg_value(arg);
      }

      if (i == 0)
        rt = (int) arg_val;
      else if (i == 1)
        rs = (int) arg_val;
      else if (i == TWO_ARGS)
        imm_operand = (int) arg_val;

      arg = strtok(NULL, ",");
      i++;
    }
  } else if (num_args == TWO_ARGS) {
    rt = (int) (is_symbol_arg(arg) && (tmp = has_symbol(arg))) ? tmp->lc : arg_value(arg);
    arg = strtok(NULL, ",");

    if (strchr(arg, '(') == NULL) {
      if (has_symbol(arg) || arg[0] == '$') {
        rs = (int) (is_symbol_arg(arg) && (tmp = has_symbol(arg))) ? tmp->lc : arg_value(arg);
      } else {
        if (is_symbol_arg(arg) && !has_symbol(arg)) {
          add_error(ln, 'U', arg);
        }

        imm_operand = (int) (is_symbol_arg(arg) && (tmp = has_symbol(arg))) ? tmp->lc : arg_value(arg);
      }
    } else {
      // base+displacement
      tmparg = strtok(arg, "()");
      imm_operand = atoi(tmparg);

      tmparg = strtok(NULL, "()");
      // dollar sign
      rs = atoi(++tmparg);
    }
  }

  i_instruction(ln, opcode, rs, rt, imm_operand);
}

/**
 * Generates the correct parameters for an instruction of type 'J',
 * then passes it off to j_instruction which does the actual bitmasking.
 **/
void assemble_j_instruction(unsigned int opcode, char *args, int num_args) {
  int rt = 0, rs = 0, arg_val = 0, i = 0, address = 0;
  char *arg;
  symbol *tmpsym;

  arg = strtok(args, ",");

  while (arg != NULL) {
    if (is_symbol_arg(arg) && (tmpsym = has_symbol(arg))) {
      arg_val = tmpsym->lc;
    } else if (is_symbol_arg(arg)) {
      add_error(ln, 'U', arg);
    } else {
      arg_val = arg_value(arg);
    }

    if (num_args == THREE_ARGS) {
      if (i == 0)
        rt = (int) arg_val;
      else if (i == 1)
        rs = (int) arg_val;
      else if (i == TWO_ARGS)
        address = (int) arg_val;
    } else if (num_args == TWO_ARGS) {
      if (i == 0)
        rt = (int) arg_val;
      else if (i == 1)
        address = (int) arg_val;
    } else if (num_args == 1) {
      address = (int) arg_val;
    }

    arg = strtok(NULL, ",");
    i++;
  }

  j_instruction(ln, opcode, rs, rt, address);
}
