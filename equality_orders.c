#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "structs.h"
#include "vm.h"
#include "equality_orders.h"


tobool_func_t toboolFuncs[] = {
    number_tobool,
    string_tobool,
    bool_tobool,
    table_tobool,
    userfunc_tobool,
    libfunc_tobool,
    nil_tobool,
    undef_tobool
};

void execute_jeq(instruction * instr){
    assert(instr-> result.type == label_a);
    
    avm_memcell rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell rv2 = avm_translate_operand(&instr->arg2, &bx);

    if(rv1->type == undef_m || rv2->type == undef_m){
        avm_error("'undef' involved in equality!");
    }else if(rv1->type == nil_m || rv2->type == nil_m){
        result = rv1->type == nil_m && rv2->type == nil_m;
    }else if(rv1->type == bool_m || rv2->type == bool_m){
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    }else if(rv1->type != rv2->type){
        avm_error(
            "%s == %s is illegal!",
            typeStrings[rv1->type],
            typeStrings[rv2->type]
        );
    }else{
        /* Equality check TODO */

        if( rv1->type == number_m){
             result = rv2->data.numVal == rv1->data.numVal;
        }else if( rv1->type == string_m){
             result = (strcmp(rv2->data.strVal, rv1->data.strVal) == 0);
        }else if(rv1->type == table_m){
             result = rv2->data.tableVal == rv1->data.tableVal;
        }else if(rv1->type == userfunc_m){
            result = rv2->data.funcVal == rv1->data.funcVal;
        }else if(rv1->type == libfunc_m){
            result = (strcmp(rv2->data.libfuncVal, rv1->data.libfuncVal) == 0);
        }else if(rv1->type == nil_m){
             assert(0);
        }else (rv1->type == undef_m){
             assert(0);
        }
    }

    if(!executionFinished && result){
        pc = instr->result.val;
    }

}
void execute_jne(instruction * instr){}
voic execute_jump(instruction * instr){}

unsigned int  execute_jlt(double x, double y){
unsigned char number_tobool(avm_memcell *m){return m->data.numVal != 0;}
unsigned char string_tobool(avm_memcell *m){return m->data.strVal[0] !=0;}
unsigned char bool_tobool(avm_memcell *m){return m->data.boolVal;}
unsigned char table_tobool(avm_memcell *m){return 1;}
unsigned char userfunc_tobool(avm_memcell *m){return 1;}
unsigned char libfunc_tobool(avm_memcell *m){return 1;}
unsigned char nil_tobool(avm_memcell* m){return 0;}
unsigned char undef_tobool(avm_memcell *m){ assert(0) return 0;}

unsigned char avm_tobool (avm_memcell* m){
    assert(m->type > = 0 && m->type < undef_m);
    return (*toboolFuncs[m->type])(m);
}

unsigned int jge(double x, double y){
    int num;
    num = x >= y;
    return num;
}
unsigned int jgt(double x, double y){
    int num;
    num = x > y;
    return num;
}
unsigned int jle(double x, double y){
    int num;
    num = x <= y;
    return num;
}
unsigned int jlt(double x, double y){
    int num;
    num = x < y;
    return num;
}