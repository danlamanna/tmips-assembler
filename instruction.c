/**
 * Dan LaManna
 * dl954588
 * dlamanna@albany.edu
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "struct_def.h"
#include "prototypes.h"

extern unsigned int instructions[];

/**
 * Composes the unsigned integer instruction for an r type instruction.
 **/
void r_instruction(unsigned int lc, unsigned int opcode, int rs1, int rs2, int rt, int sa) {
  instructions[lc] = opcode;
  instructions[lc] <<= R_INSTRUCTION_OPCODE_SHIFT;

  instructions[lc] |= (rs1 << R_INSTRUCTION_RS1_SHIFT);
  instructions[lc] |= (rs2 << R_INSTRUCTION_RS2_SHIFT);
  instructions[lc] |= (rt  << R_INSTRUCTION_RT_SHIFT);
  instructions[lc] |= (sa  << R_INSTRUCTION_SA_SHIFT);

  instructions[lc] <<= 1;
}

/**
 * Composes the unsigned integer instruction for a j type instruction.
 **/
void j_instruction(unsigned int lc, unsigned int opcode, int rs, int rt, int address) {
  instructions[lc] = opcode;
  instructions[lc] <<= J_INSTRUCTION_OPCODE_SHIFT;

  instructions[lc] |= (rs << J_INSTRUCTION_RS_SHIFT);
  instructions[lc] |= (rt << J_INSTRUCTION_RT_SHIFT);

  instructions[lc] <<= 1;

  instructions[lc] |= address;
}

/**
 * Composes the unsigned integer instruction for an i type instruction.
 **/
void i_instruction(unsigned int lc, unsigned int opcode, int rs, int rt, int imm_operand) {
  instructions[lc] = opcode;
  instructions[lc] <<= I_INSTRUCTION_OPCODE_SHIFT;

  instructions[lc] |= (rs << I_INSTRUCTION_RS_SHIFT);
  instructions[lc] |= (rt << I_INSTRUCTION_RT_SHIFT);

  instructions[lc] <<= 1;

  instructions[lc] |= imm_operand;
}
