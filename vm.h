#include "structs.h"

#define AVM_STACKSIZE 2048
#define AVM_ENDING_PC codeSize
#define AVM_STACKENV_SIZE 4

avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned int top, topsp;

struct instruction{
        vmopcode opcode;
        vmarg *result;
        vmarg *arg1;
        vmarg *arg2;
        unsigned int srcLine;

};

struct avm_table;
struct avm_memcell {
    avm_memcell_t type;
    union {
        double numVal;
        char * strVal;
        unsigned char boolVal;
        avm_table* tableVal;
        unsigned funcVal;
        char* libfuncVal;
    }data;
}avm_memcell;

enum avm_memcell_t{
    number_m = 0,
    string_m = 1,
    bool_m = 2,
    table_m = 3,
    userfunc_m = 4,
    libfunc_m = 5,
    nil_m = 6,
    undef_m = 7
}avm_memcell_t;

char 	*consts_getstring(int num);
int      consts_getnumber(int num);
char    *libfuncs_getused(int num);

avm_memcell *avm_translate_operand(vmarg *arg, avm_memcell *reg);
void avm_memcellclear(avm_memcell *mem);
void avm_error(char* error);