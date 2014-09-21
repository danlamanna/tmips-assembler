/**
 * Dan LaManna
 * dl954588
 * dlamanna@albany.edu
 **/

typedef struct symbol {
  char *name;
  unsigned int lc;
} symbol;

typedef struct error {
  int lineno;
  char type; // I, M, or U
  // I = Illegal Opcode
  // M = multiply defined
  // U = undefined
  char *message;
} error;
