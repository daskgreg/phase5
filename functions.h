#include "structs.h"

#define AVM_NUMACTUALS_OFFSET 4
#define AVM_SAVEDPC_OFFSET 3
#define AVM_SAVEDTOP_OFFSET 2
#define AVM_SAVEDTOPSP_OFFSET 1

void avm_push_enviroment_arg(instruction *instr);
void avm_dec_top(void);
void avm_callsavenviroment(void);


void execute_call_enviroment(instruction *instr);
void execute_func_enter(instruction *instr);
void execute_func_exit(instruction *unused)