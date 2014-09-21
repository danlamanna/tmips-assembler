/**
 * Dan LaManna
 * dl954588
 * dlamanna@albany.edu
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <ctype.h>

#include "constants.h"
#include "struct_def.h"
#include "prototypes.h"

/**
 * A slew of global variables:
 * symbol_table/num_symbols maintains the global array of symbols.
 * instructions is the array of assembled integers.
 * lc is the location counter.
 * ln/actual_ln are the line number, and the actual line number,
 * actual_ln includes commented and blank lines, ln does not.
 * num_errors/errors stores a global array of errors the assembler encounters.
 **/
symbol **symbol_table;
int num_symbols = 0;
unsigned int instructions[INSTRUCTIONS_MAX_LEN];
unsigned int lc = 0;
int ln = 0;
int actual_ln = 0;

int num_errors = 0;
error **errors;

/**
 * Loops through the file building the symbol_table (first pass).
 * Rewinds, and loops through the file again assembling the actual instructions
 * using the built symbol table.
 * Calls print_output which deals with which output file(s) to generate.
 **/
int main(int argc, char *argv[]) {
  char line[MAX_LINE_LENGTH + 1], *orig_line, *first_token;
  int ts = 0, ds =0;
  FILE *infile;
  char *file_basename;

  // Check for valid usage
  if (argc != REQUIRED_ARGC) {
    fprintf(stderr, "Usage: p4 infile\n");
    exit(1);
  }

  // Open .asm file
  if ((infile = fopen(argv[INFILE_ARG], "r")) == NULL) {
    fprintf(stderr, "Unable to open %s for reading.\n", argv[INFILE_ARG]);
    exit(1);
  }

  // Allocate space for the symbol_table and the errors table, these are dynamic arrays
  symbol_table = (symbol **) malloc(sizeof(symbol *) * MIN_SYMTABLE_LENGTH);
  errors       = (error **) malloc(sizeof(error *) * MIN_ERROR_LENGTH);

  // Figure out the basename of the file
  file_basename = strdup(basename(argv[INFILE_ARG]));

  if (strrchr(file_basename, '.') != NULL) {
    file_basename[strlen(file_basename) - strlen(strrchr(file_basename, '.'))] = '\0';
  }

  // First pass.
  while (fgets(line, MAX_LINE_LENGTH, infile) != NULL) {
    actual_ln++;

    // Ignore empty or entirely commented out lines
    if (!is_valid_line(line))
      continue;

    // Strips inline comments and newlines
    preprocess_line(line);

    orig_line = strdup(line);

    first_token = strtok(line, " \t\n");

    // If we're declaring a .data or .text segment, set flags, and continue to the next line
    if (first_token[0] == '.') {
      if (strcmp(first_token, DATA_SEGMENT) == 0) {
        ds = 1;
        ts = 0;
      } else if (strcmp(first_token, TEXT_SEGMENT) == 0) {
        ts = 1;
        ds = 0;
      }

      continue;
    }

    // Add a symbol if the current line has one,
    lc += maybe_add_symbol(orig_line, lc, ds, ts, 0);

    ln++;
  }

  // Go to second pass
  rewind(infile);
  lc = 0;
  ln = 0;
  actual_ln = 0;

  while (fgets(line, MAX_LINE_LENGTH, infile) != NULL) {
    actual_ln++;
    if (!is_valid_line(line))
      continue;

    // No inline comments, or newlines
    preprocess_line(line);

    orig_line = strdup(line);

    first_token = strtok(line, " \t\n");

    if (first_token[0] == '.') {
      if (strcmp(first_token, DATA_SEGMENT) == 0) {
        ds = 1;
        ts = 0;
      } else if (strcmp(first_token, TEXT_SEGMENT) == 0) {
        ts = 1;
        ds = 0;
      }

      continue;
    }

    lc += maybe_add_symbol(orig_line, lc, ds, ts, 1);

    orig_line = strip_label(orig_line);

    // Parse the instruction (either data or text segment) which kicks off assembly
    if (ds)
      parse_ds_instruction(orig_line);
    else if (ts)
      parse_ts_instruction(orig_line);

    ln++;
  }

  // Writes output to appropriate file(s)
  print_output(infile, file_basename, lc);

  fclose(infile);

  return 0;
}
