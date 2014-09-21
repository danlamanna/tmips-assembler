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

extern symbol **symbol_table;
extern int num_symbols;

extern int ln;

/**
 * Checks the global symbol table for a symbol with
 * the matching name.
 **/
symbol *has_symbol(char *name) {
  int i;

  for (i=0;i<num_symbols;i++)
    if (strcmp(symbol_table[i]->name, name) == 0)
      return symbol_table[i];

  return NULL;
}

/**
 * May or may not add a symbol, if line contains one.
 * Potentially catches a multiply defined symbol, returns the
 * integer of how much the location counter needs to increment.
 **/
int maybe_add_symbol(char *line, unsigned int lc, unsigned int ds, unsigned int ts, short second_pass) {
  char *label, *val;
  char *dupline = strdup(line);

  label = strtok(dupline, " \t");

  if (label[strlen(label)-1] != ':')
    return 1;

  label[strlen(label)-1] = '\0'; // remove colon

  if (has_symbol(label) && second_pass == 0) {
    add_error(ln, 'M', label);
    // multiply defined symbol
  } else if (label != NULL && !has_symbol(label)) {
    if (num_symbols >= MIN_SYMTABLE_LENGTH) {
      symbol_table = realloc(symbol_table, (num_symbols * REALLOC_MULTIPLIER) * sizeof(symbol *));
    }

    symbol_table[num_symbols] = malloc(sizeof(symbol));
    symbol_table[num_symbols]->name = strdup(label);
    symbol_table[num_symbols]->lc = lc;

    num_symbols++;
  }

  if (ds) {
    // data segment needs additional parsing
    val = strtok(NULL, " \t\n");
    if (strcmp(val, WORD_DIRECTIVE) == 0) {
      val = strtok(NULL, " \t\n");

      val = strtok(val, ":");
      val = strtok(NULL, ":");

      return atoi(val);
    } else if (strcmp(val, RESW_DIRECTIVE) == 0) {
      val = strtok(NULL, " \t\n");

      return atoi(val);
    }
  }

  return 1;
}
