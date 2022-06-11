typedef unsigned char (*tobool_func_t)(avm_memcell *);

unsigned char number_tobool(avm_memcell *m);
unsigned char string_tobool(avm_memcell *m);
unsigned char bool_tobool(avm_memcell *m);
unsigned char table_tobool(avm_memcell *m);
unsigned char userfunc_tobool(avm_memcell *m);
unsigned char libfunc_tobool(avm_memcell *m);
unsigned char nil_tobool(avm_memcell* m);
unsigned char undef_tobool(avm_memcell *m);

void execute_jump(instruction *instr);
void execute_jeq(instruction * instr);
void execute_jlt(instruction * instr);
void execute_jne(instruction * instr);
void execute_jgt(instruction * instr);
void execute_jge(instruction * instr);
void execute_jeq(instruction * instr);
void execute_jne(instruction * instr);
void execute_jle(instruction * instr);

unsigned int jge(double x, double y);
unsigned int jgt(double x, double y);
unsigned int jle(double x, double y);
unsigned int jlt(double x, double y);