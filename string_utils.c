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

/**
 * Takes a line and removes the first newline character
 * it finds and removes anything after the first # symbol,
 * removing inline comments.
 **/
void preprocess_line(char *line) {
  char *tmp;

  tmp = strchr(line, '\n');

  if (tmp != NULL)
    *tmp = '\0';

  tmp = strchr(line, '#');

  if (tmp != NULL)
    *tmp = '\0';
}

/**
 * Removes the label from a given line.
 **/
char *strip_label(char *line) {
  char *tmp, *orig;

  preprocess_line(line);

  // no label
  if (strchr(line, ':') == NULL)
    return line;

  orig = strdup(line);
  // operate on a copy of the line
  tmp = strdup(line);

  tmp = strtok(line, " \t");

  if (tmp != NULL && tmp[strlen(tmp)-1] == ':') {
    return (strlen(tmp) + orig);
  }

  return line;
}

/**
 * Returns 1 if the line is entirely empty, 0 otherwise.
 **/
int is_empty(const char *s) {
  while (*s != '\0' && *s != '\n') {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}

/**
 * Returns 1 if the line isn't empty and isn't a comment line, 0 otherwise.
 **/
int is_valid_line(char *line) {
  return (!is_empty(line) && line[0] != '#');
}
