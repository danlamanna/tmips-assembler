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

extern symbol **symbol_table;
extern int num_symbols;

extern unsigned int instructions[];

extern int num_errors;
extern error **errors;

/**
 * Prints the output to either a .sym and .obj file, or just a .err file.
 * Takes an open file pointer to the original assembly file, the basename
 * of the file (no extension), and the location counter.
 **/
void print_output(FILE *asm_file, char *file_basename, int lc) {
  int i = 0, j = 1;
  char line[MAX_LINE_LENGTH + 1];
  char *filename = malloc(sizeof(char) * (strlen(file_basename) + FILE_EXT_LEN  + 1));
  FILE *symfile, *objfile, *errfile;

  // Error file mode
  if (num_errors) {
    rewind(asm_file);

    sprintf(filename, "%s.err", file_basename);
    errfile = fopen(filename, "w");

    while (fgets(line, MAX_LINE_LENGTH, asm_file) != NULL) {
      fprintf(errfile, "%2d   %s", j, line);
      j++;
    }

    fprintf(errfile, "\nErrors detected:\n\n");

    for (i=0;i<num_errors;i++) {
      if (errors[i]->type == 'I')
        fprintf(errfile, "  line %d:  %s\n", errors[i]->lineno, errors[i]->message);
    }

    fprintf(errfile, "\nMultiply defined symbol(s):\n\n");

    for (i=0;i<num_errors;i++) {
      if (errors[i]->type == 'M')
        fprintf(errfile, "  %s\n", errors[i]->message);
    }

    fprintf(errfile, "\nUndefined symbol(s):\n\n");

    for (i=0;i<num_errors;i++)
      if (errors[i]->type == 'U')
        fprintf(errfile, "  %s\n", errors[i]->message);


    fclose(errfile);

    // Close error file and return, skipping generation of .obj and .sym files
    return;
  }

  // Figure out filenames for .obj and .sym files
  sprintf(filename, "%s.obj", file_basename);
  objfile = fopen(filename, "w");

  sprintf(filename, "%s.sym", file_basename);
  symfile = fopen(filename, "w");

  // Print objects in hex
  for (i=0;i<lc;i++) {
    fprintf(objfile, "%4X\t%X\n", i, instructions[i]);
  }

  // Print the symbol table, in decimal
  for (i=0;i<num_symbols;i++) {
    fprintf(symfile, "     %-8s%5d\n", symbol_table[i]->name, symbol_table[i]->lc);
  }

  fclose(symfile);
  fclose(objfile);
}
