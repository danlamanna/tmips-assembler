/**
 * Dan LaManna
 * dl954588
 * dlamanna@albany.edu
 **/

// assemble.c
void assemble_r_instruction(unsigned int opcode, char *args);
void assemble_i_instruction(unsigned int opcode, char *args, int num_args);
void assemble_j_instruction(unsigned int opcode, char *args, int num_args);

// errors.c
void add_error(int lineno, char type, char *message);

// instruction.c
void r_instruction(unsigned int lc, unsigned int opcode, int rs1, int rs2, int rt, int sa);
void j_instruction(unsigned int lc, unsigned int opcode, int rs, int rt, int address);
void i_instruction(unsigned int lc, unsigned int opcode, int rs, int rt, int imm_operand);

// mot.c
int get_opcode_by_mnemonic(char *mnemonic);
char format_by_opcode(unsigned int opcode);

// output.c
void print_output(FILE *asm_file, char *file_basename, int lc);

// parser.c
int num_args(char *args);
int parse_ds_instruction(char *instruction);
int parse_ts_instruction(char *instruction);
int is_symbol_arg(char *arg);
int arg_value(char *arg);

// string_utils.c
void preprocess_line(char *line);
char *strip_label(char *line);
int is_empty(const char *s);
int is_valid_line(char *line);

// symbol_table.c
symbol *has_symbol(char *name);
int maybe_add_symbol(char *line, unsigned int lc, unsigned int ds, unsigned int ts, short second_pass);
