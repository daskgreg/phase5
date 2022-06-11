#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "structs.h"
#include "vm.h"
#include "functions.h"


arithmetic_func_t airthmeticFuns[] = {
    add_impl,
    sub_impl,
    mul_impl,
    div_impl,
    mod_impl
};

void execute_arithmetic(instruction* instr){
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    assert(lv && (&stack[0] <= lv && &stack[top] > lv || lv == &retval));
    assert(rv1 && rv2);

    if(rv1->type != number_m || rv2->type != number_m){
        avm_error("not a number in arithmetic!");
        executionFinished = 1;
    }else{
        arithmetic_func_t op = arithmeticsFuncs[instr->opcode - add_v];
        avm_memcellclear(lv);
        lv->type = number_m;
        lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
    }
}

double add_impl(double x, double y){
    double num;
    num = x+y;
    return num;
}

double sub_impl(double x, double y){
    double num;
    num = x-y;
    return num;
}

double div_impl(double x, double y){
    double num;
    if(x == 0 || y == 0){
        return avm_error("On div_impl() X or Y is 0");
    }
    num = x/y;
    return num;
}
double mod_impl(double x, double y){
    double num;
    if( x == 0 || y == 0){
        return avm_error("On mod_impl() X or Y is 0");
    }
    num = x % y;
    return num;
}
