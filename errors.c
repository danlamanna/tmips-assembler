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

extern int num_errors;
extern error **errors;

/**
 * Adds an error struct to the global array of errors.
 **/
void add_error(int lineno, char type, char *message) {
  // increase size of array if necessary
  if (num_errors >= MIN_ERROR_LENGTH) {
    errors = realloc(errors, (num_errors * REALLOC_MULTIPLIER) * sizeof(error *));
  }

  // Setup error
  errors[num_errors] = malloc(sizeof(error));
  errors[num_errors]->lineno = lineno;
  errors[num_errors]->type = type;
  errors[num_errors]->message = message;

  num_errors++;
}
